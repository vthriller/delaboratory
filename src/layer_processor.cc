/*
    delaboratory - color correction utility
    Copyright (C) 2011 Jacek Poplawski

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "layer_processor.h"
#include "main_frame.h"
#include "layer_frame_manager.h"
#include "layer_stack.h"
#include "view_manager.h"
#include <string>
#include "layer.h"
#include "channel_manager.h"
#include "str.h"
#include "memory_info_frame.h"
#include <wx/progdlg.h>
#include <iostream>
#include "action_layer.h"
#include "logger.h"
#include "renderer.h"

static wxMutex updateImagesMutex(wxMUTEX_RECURSIVE);

class deLayerProcessorWorkerThread:public wxThread
{
    private:
        void performTasks()
        {
            while (true)
            {
                processor.log("worker thread before wait");
                semaphore.Wait();
                processor.log("worker thread after wait");

                if (TestDestroy())
                {
                    return;
                }

                processor.tickWork();

                if (TestDestroy())
                {
                    return;
                }
            }
        }

        virtual void *Entry()
        {
            performTasks();
            processor.log("worker thread finished");
            return NULL;
        }
        deLayerProcessor& processor;
        wxSemaphore& semaphore;

    public:    
        deLayerProcessorWorkerThread(deLayerProcessor& _processor, wxSemaphore& _semaphore)
        :processor(_processor),
         semaphore(_semaphore)
        {
        }
        virtual ~deLayerProcessorWorkerThread()
        {
        }
};

class deRenderWorkerThread:public wxThread
{
    private:
        void performTasks()
        {
            while (true)
            {
                processor.log("render thread before wait");
                semaphore.Wait();
                processor.log("render thread after wait");

                if (TestDestroy())
                {
                    return;
                }

                if (processor.prepareImage())
                {
                    processor.sendRepaintEvent();
                }                    

                if (TestDestroy())
                {
                    return;
                }
            }
        }

        virtual void *Entry()
        {
            performTasks();
            return NULL;
        }

        deLayerProcessor& processor;
        wxSemaphore& semaphore;

    public:    
        deRenderWorkerThread(deLayerProcessor& _processor, wxSemaphore& _semaphore)
        :processor(_processor),
         semaphore(_semaphore)
        {
        }
        virtual ~deRenderWorkerThread()
        {
        }
};

class deHistogramWorkerThread:public wxThread
{
    private:
        void performTasks()
        {
            while (true)
            {
                processor.log("histogram thread before wait");
                semaphore.Wait();
                processor.log("histogram thread after wait");

                if (TestDestroy())
                {
                    return;
                }

                processor.onGenerateHistogram();
                processor.sendHistogramEvent();

                if (TestDestroy())
                {
                    return;
                }
            }
        }

        virtual void *Entry()
        {
            performTasks();
            return NULL;
        }

        deLayerProcessor& processor;
        wxSemaphore& semaphore;

    public:    
        deHistogramWorkerThread(deLayerProcessor& _processor, wxSemaphore& _semaphore)
        :processor(_processor),
         semaphore(_semaphore)
        {
        }
        virtual ~deHistogramWorkerThread()
        {
        }
};



deLayerProcessor::deLayerProcessor()
:layerProcessMutex(wxMUTEX_RECURSIVE),
removeLayerMutex(wxMUTEX_RECURSIVE)
{
    mainFrame = NULL;
    layerFrameManager = NULL;
    stack = NULL;
    viewManager = NULL;

    logger = NULL;

    workerThread = NULL;
    renderWorkerThread = NULL;

    renderer = NULL;

    firstLayerToUpdate = 0;
    lastValidLayer = -1;

    layerProcessType = deLayerProcessInvalid;

    multithreadingEnabled = true;

    closing = false;

}

void deLayerProcessor::onDestroyAll()
{
    lock();

    lastValidLayer = -1;

    unlock();
}

deLayerProcessor::~deLayerProcessor()
{
    log("destroying layer processor");
    /*
    stopWorkerThread();
    lock();
    unlock();
    */
}

void deLayerProcessor::setMainFrame(deMainFrame* _mainFrame)
{
    mainFrame = _mainFrame;
}

void deLayerProcessor::stopWorkerThread()
{
    log("stop worker thread");
    closing = true;
    workerThread->Delete();
    renderWorkerThread->Delete();
}

void deLayerProcessor::startWorkerThread()
{
    workerThread = new deLayerProcessorWorkerThread(*this, workerSemaphore);

    if ( workerThread->Create() != wxTHREAD_NO_ERROR )
    {
    }

    if ( workerThread->Run() != wxTHREAD_NO_ERROR )
    {
    }

    renderWorkerThread = new deRenderWorkerThread(*this, renderWorkerSemaphore);

    if ( renderWorkerThread->Create() != wxTHREAD_NO_ERROR )
    {
    }

    if ( renderWorkerThread->Run() != wxTHREAD_NO_ERROR )
    {
    }

    histogramWorkerThread = new deHistogramWorkerThread(*this, histogramWorkerSemaphore);

    if ( histogramWorkerThread->Create() != wxTHREAD_NO_ERROR )
    {
    }

    if ( histogramWorkerThread->Run() != wxTHREAD_NO_ERROR )
    {
    }
}

void deLayerProcessor::setLayerStack(deLayerStack* _layerStack)
{
    stack = _layerStack;
}

void deLayerProcessor::setViewManager(deViewManager* _viewManager)
{
    viewManager = _viewManager;
}

void deLayerProcessor::repaintImageInLayerProcessor(bool calcHistogram)
{
    if (closing)
    {
        log("skip repaintImage because closing");
        return;
    }

    log("repaintImage post...");

    renderWorkerSemaphore.Post();
    generateHistogram();
}

void deLayerProcessor::generateHistogram()
{
    if (closing)
    {
        log("skip generateHistogram because closing");
        return;
    }

    log("generateHistogram post...");

    histogramWorkerSemaphore.Post();
}

void deLayerProcessor::sendRepaintEvent()
{
    if (closing)
    {
        return;
    }

    if (mainFrame)
    {
        wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, DE_REPAINT_EVENT );
        wxPostEvent( mainFrame, event );
    }
}

void deLayerProcessor::sendHistogramEvent()
{
    if (closing)
    {
        return;
    }

    if (mainFrame)
    {
        wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, DE_HISTOGRAM_EVENT );
        wxPostEvent( mainFrame, event );
    }
}

void deLayerProcessor::updateAllImages(bool calcHistogram)
{
    if (stack)
    {
        updateImages(0, -1, true);
    }        
}    

void deLayerProcessor::updateImages(int a, int channel, bool action)
{
    if (closing)
    {
        return;
    }

//    log("requested update images " + str(a) + " " + str(channel) + " " + str(blend) + " " + str(action));

    //lock();

    layerProcessMutex.Lock();

    if (a < firstLayerToUpdate)
    {
        firstLayerToUpdate = a;
    }

    if (channel >= 0)
    {
        layerProcessType = deLayerProcessSingleChannel;
        layerProcessChannel = channel;
    }
    else
    {
        if (action)
        {
            layerProcessType = deLayerProcessFull;
        }
        else
        {
            layerProcessType = deLayerProcessBlend;
        }
    }

    layerProcessMutex.Unlock();

    checkUpdateImagesRequest();

//    unlock();
}    

void deLayerProcessor::updateImage()
{
    //log("update image " + str(i) + " " + str(channel) + " " + str(blend) + " " + str(action));

    removeLayerMutex.Lock();

    layerProcessMutex.Lock();

    bool ok = true;

    if (firstLayerToUpdate > getLastLayerToUpdate())
    {
        ok = false;
    }

    int i = firstLayerToUpdate;

    deLayerProcessType type = deLayerProcessInvalid;
    int channel = -1;

    stack->lock();

    deLayer* layer = stack->getLayer(i);
    if ((layer) && (ok))
    {

        layer->lock();
        stack->unlock();

        type = layerProcessType;
        channel = layerProcessChannel;

        layerProcessType = deLayerProcessFull;

        lastValidLayer = i;
        firstLayerToUpdate = i + 1;
    }            
    else
    {
        stack->unlock();
    }

    layerProcessMutex.Unlock();

    if ((layer) && (ok))
    {
        layer->process(type, channel);

        layer->unlock();
    }        

    removeLayerMutex.Unlock();

}

void deLayerProcessor::updateImagesSmart(deChannelManager& channelManager, int view, wxProgressDialog* progressDialog, deMemoryInfoFrame* memoryInfoFrame)
{
    lock();

    std::map<int, int> channelUsage;
    generateChannelUsage(channelUsage);

    unsigned int index;
    int progress = 0;
    for (index = 0; index <= (unsigned int)view; index++)
    {
        std::map<int, int>::iterator i;
        int previous = index - 1;
        if (previous >= 0)
        {
            for (i = channelUsage.begin(); i != channelUsage.end(); i++)
            {
                int c = i->first;
                int l = i->second;
                if (l == previous)
                {
                    channelManager.tryDeallocateChannel(c);
                }
            }
        }

        deLayer* layer = stack->getLayer(index);
        std::string label = str(index) + " " + layer->getName();

        progressDialog->Update(progress, wxString::FromAscii(label.c_str()));

        layer->updateImageThreadCall();

        if (memoryInfoFrame)
        {
            memoryInfoFrame->update();
        }

        if (view > 0)
        {
            progress = 100 * index / view;
        }
        else
        {
            progress = 100;
        }

        progressDialog->Update(progress, wxString::FromAscii(label.c_str()));
    }

    progressDialog->Update(100, _T("finished"));

    unlock();
}

void deLayerProcessor::generateChannelUsage(std::map<int, int>& channelUsage)
{
    channelUsage.clear();
    int i;
    int n = stack->getSize();
    for (i = 0; i < n; i++)
    {
        deLayer* layer = stack->getLayer(i);
        layer->updateChannelUsage(channelUsage);
    }
}

void deLayerProcessor::markUpdateSingleChannel(int index, int channel)
{
    log("markUpdateSingleChannel");
    updateImages(index, channel, true);
}

void deLayerProcessor::markUpdateAllChannels(int index)
{
    if (stack)
    {
        deLayer* layer = stack->getLayer(index);
        if (layer)
        {
            if (layerFrameManager)
            {
                layerFrameManager->onUpdateProperties();
            }                
        }        
    }        

    updateImages(index, -1, true);
}

void deLayerProcessor::markUpdateBlendAllChannels(int index)
{
    updateImages(index,  -1, false);
}

void deLayerProcessor::onChangeView(int a)
{
    updateImages(a + 1, -1, true);
}   

void deLayerProcessor::lock()
{
    log("layer processor before lock");
    updateImagesMutex.Lock();
    log("layer processor after lock");
}

void deLayerProcessor::unlock()
{
    log("layer processor unlock");
    updateImagesMutex.Unlock();
}

void deLayerProcessor::tickWork()
{
    bool ok = true;

    if (!stack)
    {
        ok = false;
    }

    if (ok)
    {
        updateImage();
    }

    checkUpdateImagesRequest();

    if (ok)
    {
        repaintImageInLayerProcessor(true);
    }        
}

void deLayerProcessor::onChangeViewMode()
{
    repaintImageInLayerProcessor(true);
}    

void deLayerProcessor::onGUIUpdate()
{
    repaintImageInLayerProcessor(true);
}    

void deLayerProcessor::removeTopLayer()
{
    removeLayerMutex.Lock();

    int index = stack->getSize() - 1;
    log("requested remove top layer " + str(index));
    if (index > 0)
    {
        //lock();
        log("remove top layer - step a");
        stack->removeTopLayer();
        log("remove top layer - step b");
        int view = viewManager->getView();
        log("remove top layer - step c");
        if (view >= stack->getSize())
        {
            viewManager->setView( stack->getSize() - 1 );
        }
        log("remove top layer - step d");
        repaintImageInLayerProcessor(true);
        //unlock();
    }
    log("finished remove top layer " + str(index));

    removeLayerMutex.Unlock();
}    

void deLayerProcessor::addLayer(deLayer* layer)
{
//    lock();

    log("add layer " + str(layer->getIndex()) + " " +  layer->getName());

    if (stack)
    {
        stack->addLayer(layer);

        int index = layer->getIndex();
        markUpdateAllChannels(index);
    }
    log("finished add layer " + str(layer->getIndex()) + " " +  layer->getName());

//    unlock();
}    

void deLayerProcessor::setLogger(deLogger* _logger)
{
    logger = _logger;
}

void deLayerProcessor::log(const std::string& message)
{
    if (logger)
    {
        logger->log(message);
    }
}

void deLayerProcessor::checkUpdateImagesRequest()
{
    layerProcessMutex.Lock();

    bool ok = true;

    if (firstLayerToUpdate > getLastLayerToUpdate())
    {
        ok = false;
    }

    if (!stack)
    {
        ok = false;
    }

    if (ok)
    {
        workerSemaphore.Post();
    }        

    layerProcessMutex.Unlock();
}

int deLayerProcessor::getLastLayerToUpdate()
{
    if (!viewManager)
    {
        return 0;
    }
    else
    {
        int n = viewManager->getView();
        return n;
    }        
}

void deLayerProcessor::setLayerFrameManager(deLayerFrameManager* _layerFrameManager)
{
    layerFrameManager = _layerFrameManager;
}

void deLayerProcessor::setRenderer(deRenderer* _renderer)
{
    renderer = _renderer;
}

bool deLayerProcessor::prepareImage()
{
    removeLayerMutex.Lock();
    log("prepare image start");
    lock();
    bool result = false;
    if (renderer)
    {
        result = renderer->prepareImage();
    }
    unlock();
    log("prepare image end");
    removeLayerMutex.Unlock();
    return result;
}

void deLayerProcessor::onGenerateHistogram()
{
    removeLayerMutex.Lock();
    if (mainFrame)
    {
        mainFrame->generateHistogram();
    }
    removeLayerMutex.Unlock();
}


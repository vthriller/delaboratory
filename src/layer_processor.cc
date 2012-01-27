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


class deLayerProcessorWorkerThread:public wxThread
{
    private:
        void performTasks()
        {
            while (true)
            {
                if (processor.isClosing())
                {
                    return;
                }

                logMessage("worker thread before wait");
                semaphore.Wait();
                logMessage("worker thread after wait");
                Sleep(10);

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
            logMessage("worker thread finished");
            return NULL;
        }
        deLayerProcessor& processor;
        wxSemaphore& semaphore;

    public:    
        deLayerProcessorWorkerThread(deLayerProcessor& _processor, wxSemaphore& _semaphore)
        :processor(_processor),
         semaphore(_semaphore)
        {
            logMessage("worker thread created");
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
                if (processor.isClosing())
                {
                    return;
                }

                logMessage("render thread before wait");
                semaphore.Wait();
                logMessage("render thread after wait");
                Sleep(10);

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
            logMessage("render thread finished");
            return NULL;
        }

        deLayerProcessor& processor;
        wxSemaphore& semaphore;

    public:    
        deRenderWorkerThread(deLayerProcessor& _processor, wxSemaphore& _semaphore)
        :processor(_processor),
         semaphore(_semaphore)
        {
            logMessage("render thread created");
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
                if (processor.isClosing())
                {
                    return;
                }

                logMessage("histogram thread before wait");
                semaphore.Wait();
                logMessage("histogram thread after wait");
                Sleep(10);

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
            logMessage("histogram thread finished");
            return NULL;
        }

        deLayerProcessor& processor;
        wxSemaphore& semaphore;

    public:    
        deHistogramWorkerThread(deLayerProcessor& _processor, wxSemaphore& _semaphore)
        :processor(_processor),
         semaphore(_semaphore)
        {
            logMessage("histogram thread created");
        }
        virtual ~deHistogramWorkerThread()
        {
        }
};



deLayerProcessor::deLayerProcessor(deChannelManager& _previewChannelManager, deLayerStack& _layerStack, deLayerFrameManager& _layerFrameManager)
:layerProcessMutex(wxMUTEX_RECURSIVE),
histogramMutex(wxMUTEX_RECURSIVE),
prepareImageMutex(wxMUTEX_RECURSIVE),
updateImageMutex(wxMUTEX_RECURSIVE),
updateImagesMutex(wxMUTEX_RECURSIVE),
previewChannelManager(_previewChannelManager),
layerStack(_layerStack),
layerFrameManager(_layerFrameManager)
{
    mainFrame = NULL;
    viewManager = NULL;

    workerThread = NULL;
    renderWorkerThread = NULL;

    firstLayerToUpdate = 0;
    lastValidLayer = -1;

    layerProcessType = deLayerProcessInvalid;

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
    logMessage("destroying layer processor");
}

void deLayerProcessor::setMainFrame(deMainFrame* _mainFrame)
{
    mainFrame = _mainFrame;
}

void deLayerProcessor::stopWorkerThread()
{
    logMessage("stop worker thread");
    closing = true;

    logMessage("worker thread post before delete");
    workerSemaphore.Post();
    logMessage("stop worker thread - workerThread delete");
    workerThread->Delete();

    logMessage("render worker thread post before delete");
    renderWorkerSemaphore.Post();
    logMessage("stop worker thread - renderWorkerThread delete");
    renderWorkerThread->Delete();

    logMessage("histogram worker thread post before delete");
    histogramWorkerSemaphore.Post();
    logMessage("stop worker thread - histogramWorkerThread delete");
    histogramWorkerThread->Delete();

    logMessage("stop worker thread done");
}

void deLayerProcessor::startWorkerThread()
{
    logMessage("starting worker threads...");

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

void deLayerProcessor::setViewManager(deViewManager* _viewManager)
{
    viewManager = _viewManager;
}

void deLayerProcessor::repaintImageInLayerProcessor(bool calcHistogram)
{
    if (closing)
    {
        logMessage("skip repaintImage because closing");
        return;
    }

    logMessage("repaintImage post...");

    renderWorkerSemaphore.Post();
    generateHistogram();
}

void deLayerProcessor::generateHistogram()
{
    if (closing)
    {
        logMessage("skip generateHistogram because closing");
        return;
    }

    logMessage("generateHistogram post...");

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
    updateImages(0, -1, true);
}    

void deLayerProcessor::lockLayers()
{
    lockWithLog(layerProcessMutex, "layer process mutex");
}

void deLayerProcessor::unlockLayers()
{
    logMessage("unlockLayers");
    layerProcessMutex.Unlock();
}

void deLayerProcessor::lockHistogram()
{
    lockWithLog(histogramMutex, "histogram mutex");
}

void deLayerProcessor::unlockHistogram()
{
    logMessage("unlockHistogram");
    histogramMutex.Unlock();
}

void deLayerProcessor::lockUpdateImage()
{
    lockWithLog(updateImageMutex, "update image mutex");
}

void deLayerProcessor::unlockUpdateImage()
{
    logMessage("unlockUpdateImage");
    updateImageMutex.Unlock();
}

void deLayerProcessor::lockPrepareImage()
{
    lockWithLog(prepareImageMutex, "prepare image mutex");
}

void deLayerProcessor::unlockPrepareImage()
{
    logMessage("unlockPrepare");
    prepareImageMutex.Unlock();
}

void deLayerProcessor::updateImages(int a, int channel, bool action)
{
    if (closing)
    {
        return;
    }

//    log("requested update images " + str(a) + " " + str(channel) + " " + str(blend) + " " + str(action));

    //lock();

    lockLayers();

    if (a == firstLayerToUpdate)
    {
        channel = -1;
    }

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

    unlockLayers();

    checkUpdateImagesRequest();

//    unlock();
}    

void deLayerProcessor::updateImage()
{
    lockUpdateImage();

    lockLayers();

    bool ok = true;

    if (firstLayerToUpdate > getLastLayerToUpdate())
    {
        ok = false;
    }

    int i = firstLayerToUpdate;

    deLayerProcessType type = deLayerProcessInvalid;
    int channel = -1;

    layerStack.lock();

    deLayer* layer = layerStack.getLayer(i);
    if ((layer) && (ok))
    {

        layer->lockLayer();
        layerStack.unlock();

        type = layerProcessType;
        channel = layerProcessChannel;

        layerProcessType = deLayerProcessFull;

        lastValidLayer = i;
        firstLayerToUpdate = i + 1;
    }            
    else
    {
        layerStack.unlock();
    }

    unlockLayers();

    if ((layer) && (ok))
    {
        layer->process(type, channel);

        layer->unlockLayer();
    }        

    unlockUpdateImage();

}

void deLayerProcessor::updateImagesSmart(int view, wxProgressDialog* progressDialog, deMemoryInfoFrame* memoryInfoFrame)
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
                    previewChannelManager.tryDeallocateChannel(c);
                }
            }
        }

        deLayer* layer = layerStack.getLayer(index);
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
    int n = layerStack.getSize();
    for (i = 0; i < n; i++)
    {
        deLayer* layer = layerStack.getLayer(i);
        layer->updateChannelUsage(channelUsage);
    }
}

void deLayerProcessor::markUpdateSingleChannel(int index, int channel)
{
    logMessage("markUpdateSingleChannel");
    updateImages(index, channel, true);
}

void deLayerProcessor::markUpdateAllChannels(int index)
{
    deLayer* layer = layerStack.getLayer(index);
    if (layer)
    {
        layerFrameManager.onUpdateProperties();
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
    logMessage("layer processor lock");
    lockWithLog(updateImagesMutex, "update images mutex");
}

void deLayerProcessor::unlock()
{
    logMessage("layer processor unlock");
    updateImagesMutex.Unlock();
}

void deLayerProcessor::tickWork()
{
    bool ok = true;

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
    sendRepaintEvent();
}    

void deLayerProcessor::removeTopLayer()
{
    lockHistogram();
    lockPrepareImage();
    lockUpdateImage();

    int index = layerStack.getSize() - 1;
    logMessage("requested remove top layer " + str(index));
    if (index > 0)
    {
        //lock();
        layerStack.removeTopLayer();
        int view = viewManager->getView();
        if (view >= layerStack.getSize())
        {
            viewManager->setView( layerStack.getSize() - 1 );
        }
        repaintImageInLayerProcessor(true);
        //unlock();
    }

    unlockUpdateImage();
    unlockPrepareImage();
    unlockHistogram();
}    

void deLayerProcessor::addLayer(deLayer* layer)
{
//    lock();

    logMessage("add layer " + str(layer->getIndex()) + " " +  layer->getName());

    layerStack.addLayer(layer);

    int index = layer->getIndex();
    markUpdateAllChannels(index);

//    unlock();
}    

void deLayerProcessor::checkUpdateImagesRequest()
{
    lockLayers();

    bool ok = true;

    if (firstLayerToUpdate > getLastLayerToUpdate())
    {
        ok = false;
    }

    if (ok)
    {
        workerSemaphore.Post();
    }        

    unlockLayers();
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

bool deLayerProcessor::prepareImage()
{

    bool result = false;
    lockPrepareImage();
    logMessage("prepare image start");
    lock();

    if (!closing)
    {
        if (viewManager) 
        {
            result = renderer.prepareImage(previewChannelManager, *viewManager, *this, layerStack);
        }
    }

    unlock();
    logMessage("prepare image end");
    unlockPrepareImage();
    return result;
}

void deLayerProcessor::onGenerateHistogram()
{
    lockHistogram();

    if (!closing)
    {
        if (mainFrame)
        {
            mainFrame->generateHistogram();
        }
    }        

    unlockHistogram();
}

void deLayerProcessor::setPreviewSize(const deSize& size)
{
    lockHistogram();
    lockPrepareImage();
    lockUpdateImage();

    logMessage("set preview size");

    previewChannelManager.setChannelSize(size);

    updateAllImages(false);

    unlockUpdateImage();
    unlockPrepareImage();
    unlockHistogram();
}

void deLayerProcessor::render(wxDC& dc)
{
    renderer.render(dc);
}

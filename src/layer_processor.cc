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
#include "main_window.h"
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
#include "logger.h"
#include "renderer.h"
#include "image_io.h"

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
                semaphore.wait();
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
        deSemaphore& semaphore;

    public:    
        deLayerProcessorWorkerThread(deLayerProcessor& _processor, deSemaphore& _semaphore)
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
                semaphore.wait();
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
        deSemaphore& semaphore;

    public:    
        deRenderWorkerThread(deLayerProcessor& _processor, deSemaphore& _semaphore)
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
                semaphore.wait();
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
        deSemaphore& semaphore;

    public:    
        deHistogramWorkerThread(deLayerProcessor& _processor, deSemaphore& _semaphore)
        :processor(_processor),
         semaphore(_semaphore)
        {
            logMessage("histogram thread created");
        }
        virtual ~deHistogramWorkerThread()
        {
        }
};


deLayerProcessor::deLayerProcessor(deChannelManager& _previewChannelManager, deLayerStack& _layerStack, deLayerFrameManager& _layerFrameManager, deMainWindow& _mainWindow)
:
layerStack(_layerStack),
layerFrameManager(_layerFrameManager),
workerSemaphore(1,1),
renderWorkerSemaphore(1, 1),
histogramWorkerSemaphore(1, 1),
renderer(_previewChannelManager),
previewChannelManager(_previewChannelManager),
mainWindow(_mainWindow)
{
    viewManager = NULL;

    histogramWorkerThread = NULL;
    workerThread = NULL;
    renderWorkerThread = NULL;

    firstLayerToUpdate = 0;
    lastValidLayer = -1;

    layerProcessType = deLayerProcessInvalid;
    layerProcessChannel = -1;

    closing = false;

    realtime = true;

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

void deLayerProcessor::stopWorkerThread()
{
    logMessage("stop worker thread");
    closing = true;

    logMessage("worker thread post before delete");
    workerSemaphore.post();
    logMessage("stop worker thread - workerThread delete");
    workerThread->Delete();

    logMessage("render worker thread post before delete");
    renderWorkerSemaphore.post();

    logMessage("stop worker thread - renderWorkerThread delete");
    renderWorkerThread->Delete();

    logMessage("histogram worker thread post before delete");
    histogramWorkerSemaphore.post();
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

void deLayerProcessor::repaintImageInLayerProcessor()
{

    if (closing)
    {
        logMessage("skip repaintImage because closing");
        return;
    }

    logMessage("repaintImage post...");

    renderWorkerSemaphore.post();
    generateHistogram();

}

void deLayerProcessor::generateHistogram()
{
    if (closing)
    {
        logMessage("skip generateHistogram because closing");
        return;
    }

    logMessage("generate histogram post...");

    histogramWorkerSemaphore.post();
}

void deLayerProcessor::sendRepaintEvent()
{
    if (closing)
    {
        return;
    }

    mainWindow.postEvent(DE_REPAINT_EVENT, 0 );
}

void deLayerProcessor::sendInfoEvent(int i)
{
    if (closing)
    {
        return;
    }

    mainWindow.postEvent(DE_INFO_EVENT, i );
}

void deLayerProcessor::sendHistogramEvent()
{
    if (closing)
    {
        return;
    }

    mainWindow.postEvent(DE_HISTOGRAM_EVENT, 0 );
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
    layerProcessMutex.unlock();
}

void deLayerProcessor::lockHistogram()
{
    lockWithLog(histogramMutex, "histogram mutex");
}

void deLayerProcessor::unlockHistogram()
{
    logMessage("unlockHistogram");
    histogramMutex.unlock();
}

void deLayerProcessor::lockUpdateImage()
{
    lockWithLog(updateImageMutex, "update image mutex");
}

void deLayerProcessor::unlockUpdateImage()
{
    logMessage("unlockUpdateImage");
    updateImageMutex.unlock();
}

void deLayerProcessor::lockPrepareImage()
{
    lockWithLog(prepareImageMutex, "prepare image mutex");
}

void deLayerProcessor::unlockPrepareImage()
{
    logMessage("unlockPrepare");
    prepareImageMutex.unlock();
}

void deLayerProcessor::updateImages(int a, int channel, bool action)
{
    if (closing)
    {
        return;
    }

    lockLayers();

    if (a == firstLayerToUpdate)
    {
        if (layerProcessChannel != channel)
        {
            channel = -1;
        }            
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

}    

bool deLayerProcessor::updateLayerImage()
{
    logMessage("updateLayerImage");
    lockUpdateImage();

    lockLayers();

    bool ok = true;
    bool result = false;

    if (firstLayerToUpdate > getLastLayerToUpdate())
    {
        ok = false;
    }

    int i = firstLayerToUpdate;

    deLayerProcessType type = deLayerProcessInvalid;
    int channel = -1;

    layerStack.lock();

    deBaseLayer* layer = layerStack.getLayer(i);

    if ((layer) && (ok))
    {
        logMessage("updateLayerImage layer && ok");

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
        logMessage("updateLayerImage process...");
        layer->processLayer(type, channel);

        layer->unlockLayer();

        result = true;
    }        

    unlockUpdateImage();

    updateWarning();

    logMessage("updateLayerImage DONE");

    return result;

}

void deLayerProcessor::updateWarning()
{
    if (closing)
    {
        return;
    }

    mainWindow.postEvent(DE_WARNING_EVENT, 0 );
}

bool deLayerProcessor::updateImagesSmart(int view, wxProgressDialog* progressDialog, deMemoryInfoFrame* memoryInfoFrame, const std::string& fileName, const std::string& type, bool saveAll)
{
    bool result = true;
    logMessage("updateImagesSmart");

    lock();

    std::map<int, int> channelUsage;
    generateChannelUsage(channelUsage);

    unsigned int index;
    int progress = 0;
    for (index = 0; index <= (unsigned int)view; index++)
    {
        logMessage("updateImagesSmart index: " + str(index));
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
                    logMessage("deallocate " + str(c));
                    previewChannelManager.tryDeallocateChannel(c);
                }
            }
        }

        deBaseLayer* layer = layerStack.getLayer(index);

        std::string label = str(index);

        progressDialog->Update(progress, wxString::FromAscii(label.c_str()));

        logMessage("updateImagesSmart process index: " + str(index));
        bool r = layer->processFull();
        if (r)
        {
            if (saveAll)
            {
                const deImage& image = layer->getLayerImage();
                const std::string f = insertIndex(fileName, index);
                saveImage(f, image, type, previewChannelManager);
            }                
        }
        else
        {
            result = false;
            // stop loop
            index = view + 1;
        }

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

    if ((result) && (!saveAll))
    {
        // take the final image
        deBaseLayer* layer = layerStack.getLayer(view);
        const deImage& image = layer->getLayerImage();

        // save it
        saveImage(fileName, image, type, previewChannelManager);
    }

    logMessage("updateImagesSmart DONE");

    return result;
}

void deLayerProcessor::generateChannelUsage(std::map<int, int>& channelUsage)
{
    channelUsage.clear();
    int i;
    int n = layerStack.getSize();
    for (i = 0; i < n; i++)
    {
        deBaseLayer* layer = layerStack.getLayer(i);

        layer->updateChannelUsage(channelUsage, i);
    }
}

void deLayerProcessor::markUpdateSingleChannel(int index, int channel)
{
    deBaseLayer* layer = layerStack.getLayer(index);

    if (layer)
    {
        layer->onUpdateProperties();
    }        

    logMessage("markUpdateSingleChannel");
    updateImages(index, channel, true);
}

void deLayerProcessor::markUpdateAllChannels(int index)
{
    deBaseLayer* layer = layerStack.getLayer(index);

    if (layer)
    {
        layerFrameManager.onUpdateProperties();
        layer->onUpdateProperties();
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
    updateWarning();
}   

void deLayerProcessor::lock()
{
    logMessage("layer processor lock");
    lockWithLog(updateImagesMutex, "update images mutex");
}

void deLayerProcessor::unlock()
{
    logMessage("layer processor unlock");
    updateImagesMutex.unlock();
}

void deLayerProcessor::tickWork()
{
    logMessage("tickWork");
    sendInfoEvent(DE_PROCESSING_START);

    bool result = updateLayerImage();

    if (result)
    {
        checkUpdateImagesRequest();

        repaintImageInLayerProcessor();
    }        

    sendInfoEvent(DE_PROCESSING_END);
}

void deLayerProcessor::onChangeViewMode()
{
    repaintImageInLayerProcessor();
}    

void deLayerProcessor::onGUIUpdate()
{
    sendRepaintEvent();
}    

void deLayerProcessor::removeTopLayerInLayerProcessor()
{
    lockHistogram();
    lockPrepareImage();
    lockUpdateImage();

    int index = layerStack.getSize() - 1;
    logMessage("requested remove top layer " + str(index));
    if (index > 0)
    {
        layerStack.removeTopLayer();
        int view = viewManager->getView();
        if (view >= layerStack.getSize())
        {
            viewManager->setView( layerStack.getSize() - 1 );
        }
        repaintImageInLayerProcessor();
    }

    unlockUpdateImage();
    unlockPrepareImage();
    unlockHistogram();
}    

void deLayerProcessor::removeAllLayers()
{
    lockHistogram();
    lockPrepareImage();
    lockUpdateImage();

    while (layerStack.getSize() > 0)
    {
        layerStack.removeTopLayer();
    }

    repaintImageInLayerProcessor();

    unlockUpdateImage();
    unlockPrepareImage();
    unlockHistogram();
}    

void deLayerProcessor::addLayerInLayerProcessor(deBaseLayer* layer, int layerIndex)
{
    layerStack.addLayer(layer);

    markUpdateAllChannels(layerIndex);

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
        workerSemaphore.post();
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
    sendInfoEvent(DE_RENDERING_START);

    bool result = false;
    lockPrepareImage();
    logMessage("prepare image start");
    lock();

    if (!closing)
    {
        if (viewManager) 
        {
            result = renderer.prepareImage(*viewManager, *this, layerStack);
        }
    }

    unlock();
    logMessage("prepare image end");
    unlockPrepareImage();

    sendInfoEvent(DE_RENDERING_END);

    return result;
}

void deLayerProcessor::onGenerateHistogram()
{
    sendInfoEvent(DE_HISTOGRAM_START);

    lockHistogram();

    if (!closing)
    {
        mainWindow.generateHistogram();
    }        

    unlockHistogram();

    sendInfoEvent(DE_HISTOGRAM_END);
}

void deLayerProcessor::setPreviewSize(const deSize& size)
{
    deSize oldSize = previewChannelManager.getChannelSizeFromChannelManager();
    if (oldSize == size)
    {
        return;
    }

    logMessage("set preview size...");
    lockHistogram();
    lockPrepareImage();
    lockUpdateImage();

    previewChannelManager.setChannelSize(size);

    updateAllImages(false);

    unlockUpdateImage();
    unlockPrepareImage();
    unlockHistogram();
    logMessage("set preview size done");
}

void deLayerProcessor::onImageLoad()
{
    logMessage("on image load...");
    lockHistogram();
    lockPrepareImage();
    lockUpdateImage();

    updateAllImages(false);

    unlockUpdateImage();
    unlockPrepareImage();
    unlockHistogram();
    logMessage("on image load done...");
}

void deLayerProcessor::render(deCanvas& canvas)
{
    renderer.render(canvas);
}

bool deLayerProcessor::isRealtime() const
{
    return realtime;
}

void deLayerProcessor::setRealtime(bool r)
{
    realtime = r;
}

void deLayerProcessor::setHistogramChannel(int channel)
{
    if (viewManager)
    {
        viewManager->setHistogramChannel(channel);
    }
}

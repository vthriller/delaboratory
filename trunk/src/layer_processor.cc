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
#include "base_layer.h"
#include "channel_manager.h"
#include "str.h"
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

                logInfo("worker thread wait...");
                semaphore.wait();
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
            logInfo("worker thread finished");
            return NULL;
        }
        deLayerProcessor& processor;
        deSemaphore& semaphore;

    public:    
        deLayerProcessorWorkerThread(deLayerProcessor& _processor, deSemaphore& _semaphore)
        :processor(_processor),
         semaphore(_semaphore)
        {
            logInfo("worker thread created");
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

                logInfo("render thread wait...");
                semaphore.wait();
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
            logInfo("render thread finished");
            return NULL;
        }

        deLayerProcessor& processor;
        deSemaphore& semaphore;

    public:    
        deRenderWorkerThread(deLayerProcessor& _processor, deSemaphore& _semaphore)
        :processor(_processor),
         semaphore(_semaphore)
        {
            logInfo("render thread created");
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

                logInfo("histogram thread wait...");
                semaphore.wait();
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
            logInfo("histogram thread finished");
            return NULL;
        }

        deLayerProcessor& processor;
        deSemaphore& semaphore;

    public:    
        deHistogramWorkerThread(deLayerProcessor& _processor, deSemaphore& _semaphore)
        :processor(_processor),
         semaphore(_semaphore)
        {
            logInfo("histogram thread created");
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
    logInfo("layer processor constructor");
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
    logInfo("layer processor destructor");
}

void deLayerProcessor::stopWorkerThread()
{
    closing = true;

    workerSemaphore.post();
    workerThread->Delete();

    renderWorkerSemaphore.post();

    renderWorkerThread->Delete();

    histogramWorkerSemaphore.post();
    histogramWorkerThread->Delete();

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

void deLayerProcessor::setViewManager(deViewManager* _viewManager)
{
    viewManager = _viewManager;
}

void deLayerProcessor::repaintImageInLayerProcessor()
{
    if (closing)
    {
        logInfo("skip repaintImage because closing");
        return;
    }
    logInfo("repaintImage");

    renderWorkerSemaphore.post();
    generateHistogram();

}

void deLayerProcessor::generateHistogram()
{
    if (closing)
    {
        logInfo("skip generateHistogram because closing");
        return;
    }

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

void deLayerProcessor::lockLayers() const
{
    logInfo("locking layer process mutex...");
    layerProcessMutex.lock();
    logInfo("layer process mutex locked");
}

void deLayerProcessor::unlockLayers() const
{
    logInfo("unlocking layer process mutex");
    layerProcessMutex.unlock();
}

void deLayerProcessor::lockHistogram()
{
    logInfo("locking histogram mutex...");
    histogramMutex.lock();
    logInfo("histogram mutex locked");
}

void deLayerProcessor::unlockHistogram()
{
    logInfo("unlocking histogram mutex");
    histogramMutex.unlock();
}

void deLayerProcessor::lockUpdateImage()
{
    logInfo("locking update image mutex...");
    updateImageMutex.lock();
    logInfo("update image mutex locked");
}

void deLayerProcessor::unlockUpdateImage()
{
    logInfo("unlocking update image mutex");
    updateImageMutex.unlock();
}

void deLayerProcessor::lockPrepareImage()
{
    logInfo("locking prepare image mutex...");
    prepareImageMutex.lock();
    logInfo("prepare image mutex locked");
}

void deLayerProcessor::unlockPrepareImage()
{
    logInfo("unlocking prepare image mutex");
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
    logInfo("updateLayerImage");
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
        layer->processLayer(type, channel);

        layer->unlockLayer();

        result = true;
    }        

    unlockUpdateImage();

    updateWarning();

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

bool deLayerProcessor::updateImagesSmart(int view, wxProgressDialog* progressDialog, const std::string& fileName, const std::string& type, bool saveAll)
{
    bool result = true;
    logInfo("updateImagesSmart");

    lock();

    std::map<int, int> channelUsage;
    generateChannelUsage(channelUsage);

    unsigned int index;
    int progress = 0;
    for (index = 0; index <= (unsigned int)view; index++)
    {
        logInfo("updateImagesSmart index: " + str(index));
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
                    logInfo("deallocate " + str(c));
                    previewChannelManager.tryDeallocateChannel(c);
                }
            }
        }

        deBaseLayer* layer = layerStack.getLayer(index);

        std::string label = str(index);

        progressDialog->Update(progress, wxString::FromAscii(label.c_str()));

        logInfo("updateImagesSmart process index: " + str(index));
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

    logInfo("updateImagesSmart DONE");

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
    logInfo("markUpdateSingleChannel " +str(index) + " " + str(channel));
    updateImages(index, channel, true);
}

void deLayerProcessor::markUpdateAllChannels(int index)
{
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
    logInfo("layer processor lock...");
    updateImagesMutex.lock();
    logInfo("layer processor locked");
}

void deLayerProcessor::unlock()
{
    logInfo("layer processor unlock");
    updateImagesMutex.unlock();
}

void deLayerProcessor::tickWork()
{
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
    logInfo("removeTopLayer");
    lockHistogram();
    lockPrepareImage();
    lockUpdateImage();

    int index = layerStack.getSize() - 1;
    logInfo("requested remove top layer " + str(index));
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
    logInfo("removeTopLayer DONE");
}    

void deLayerProcessor::removeAllLayers()
{
    logInfo("removeAllLayers");
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
    logInfo("removeAllLayers DONE");
}    

void deLayerProcessor::addLayerInLayerProcessor(deBaseLayer* layer)
{
    layerStack.addLayer(layer);

    int n = layerStack.getSize();

    markUpdateAllChannels(n-1);

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
    logInfo("prepare image start");

    sendInfoEvent(DE_RENDERING_START);

    bool result = false;
    lockPrepareImage();
    lock();

    if (!closing)
    {
        if (viewManager) 
        {
            result = renderer.prepareImage(*viewManager, *this, layerStack);
        }
    }

    unlock();
    unlockPrepareImage();

    sendInfoEvent(DE_RENDERING_END);
    logInfo("prepare image DONE");

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

void deLayerProcessor::forceUpdateSize()
{
    mainWindow.forceUpdateSize();
}

void deLayerProcessor::setPreviewSize(const deSize& size, bool canSkip)
{
    deSize oldSize = previewChannelManager.getChannelSizeFromChannelManager();
    if ((oldSize == size) && (canSkip))
    {
        logInfo("skip set preview size");
        return;
    }

    if (size.getW() < 0)
    {
        logError("broken size passed to setPreviewSize, w: " + str(size.getW()));
    }

    if (size.getH() < 0)
    {
        logError("broken size passed to setPreviewSize, h: " + str(size.getH()));
    }

    logInfo("setPreviewSize start");

    lockHistogram();
    lockPrepareImage();
    lockUpdateImage();

    previewChannelManager.setChannelSize(size);

    updateAllImages(false);

    unlockUpdateImage();
    unlockPrepareImage();
    unlockHistogram();
    logInfo("setPreviewSize DONE");
}

void deLayerProcessor::onImageLoad()
{
    logInfo("onImageLoad");
    lockHistogram();
    lockPrepareImage();
    lockUpdateImage();

    updateAllImages(false);

    unlockUpdateImage();
    unlockPrepareImage();
    unlockHistogram();
    logInfo("onImageLoad DONE");
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

int deLayerProcessor::getLastValidLayer() const
{
    lockLayers();
    int l = lastValidLayer;
    unlockLayers();
    return l;
}

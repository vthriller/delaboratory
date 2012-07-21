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
#include "main_frame_events.h"
#include "layer_frame_manager.h"
#include "layer_stack.h"
#include "view_manager.h"
#include <string>
#include "base_layer.h"
#include "channel_manager.h"
#include "str.h"
#include "progress_dialog.h"
#include <iostream>
#include "logger.h"
#include "renderer.h"
#include "image_io.h"
#include "layer_processor_threads.h"
#include "flatten_layers.h"
#include "gui.h"

deLayerProcessor::deLayerProcessor(deChannelManager& _previewChannelManager, deLayerStack& _layerStack, deLayerFrameManager& _layerFrameManager, deMainWindow& _mainWindow)
:
layerStack(_layerStack),
layerFrameManager(_layerFrameManager),
renderer(_previewChannelManager),
previewChannelManager(_previewChannelManager),
mainWindow(_mainWindow)
{
#ifdef DEBUG_LOG
    logInfo("layer processor constructor");
#endif    
    viewManager = NULL;

    firstLayerToUpdate = 0;
    lastValidLayer = -1;

    layerProcessType = deLayerProcessInvalid;
    layerProcessChannel = -1;

    closing = false;

    threads = new deLayerProcessorThreads(*this);

}

void deLayerProcessor::onDestroyAll()
{
    lockLayerProcessor();

    lastValidLayer = -1;

    unlockLayerProcessor();
}

deLayerProcessor::~deLayerProcessor()
{
#ifdef DEBUG_LOG
    logInfo("layer processor destructor");
#endif    
    delete threads;
}

void deLayerProcessor::stopWorkerThread()
{
    closing = true;
    threads->stopWorkerThread();
}

void deLayerProcessor::startWorkerThread()
{
    threads->startWorkerThread();
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
#ifdef DEBUG_LOG
    logInfo("repaintImage");
#endif    

    threads->renderPost();
    generateHistogram();

}

void deLayerProcessor::generateHistogram()
{
    if (closing)
    {
        logInfo("skip generateHistogram because closing");
        return;
    }

    threads->histogramPost();
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
#ifdef DEBUG_LOG
    logInfo("locking layer process mutex...");
#endif    
    layerProcessMutex.lock();
#ifdef DEBUG_LOG
    logInfo("layer process mutex locked");
#endif    
}

void deLayerProcessor::unlockLayers() const
{
#ifdef DEBUG_LOG
    logInfo("unlocking layer process mutex");
#endif    
    layerProcessMutex.unlock();
}

void deLayerProcessor::lockHistogram()
{
#ifdef DEBUG_LOG
    logInfo("locking histogram mutex...");
#endif    
    histogramMutex.lock();
#ifdef DEBUG_LOG
    logInfo("histogram mutex locked");
#endif    
}

void deLayerProcessor::unlockHistogram()
{
#ifdef DEBUG_LOG
    logInfo("unlocking histogram mutex");
#endif    
    histogramMutex.unlock();
}

void deLayerProcessor::lockUpdateImage()
{
#ifdef DEBUG_LOG
    logInfo("locking update image mutex...");
#endif    
    updateImageMutex.lock();
#ifdef DEBUG_LOG
    logInfo("update image mutex locked");
#endif    
}

void deLayerProcessor::unlockUpdateImage()
{
#ifdef DEBUG_LOG
    logInfo("unlocking update image mutex");
#endif    
    updateImageMutex.unlock();
}

void deLayerProcessor::lockPrepareImage()
{
#ifdef DEBUG_LOG
    logInfo("locking prepare image mutex...");
#endif    
    prepareImageMutex.lock();
#ifdef DEBUG_LOG
    logInfo("prepare image mutex locked");
#endif    
}

void deLayerProcessor::unlockPrepareImage()
{
#ifdef DEBUG_LOG
    logInfo("unlocking prepare image mutex");
#endif    
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
#ifdef DEBUG_LOG
    logInfo("updateLayerImage");
#endif    
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

    deBaseLayer* layer = layerStack.getLayer(i);

    if ((layer) && (ok))
    {
        layer->lockLayer();

        type = layerProcessType;
        channel = layerProcessChannel;

        layerProcessType = deLayerProcessFull;

        lastValidLayer = i;
        firstLayerToUpdate = i + 1;
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

bool deLayerProcessor::updateImagesSmart(deProgressDialog& progressDialog, const std::string& fileName, const std::string& type, bool saveAll, const deSize& size, deGUI& gui)
{
    gui.lockSize();

    lockLayerProcessor();
    lockHistogram();
    lockPrepareImage();
    lockUpdateImage();

    // remember original size of preview
    deSize originalSize = previewChannelManager.getChannelSizeFromChannelManager();

    // calculate final image in full size
    int view = viewManager->getView();

    previewChannelManager.setChannelSize(size, false);

    bool result = flattenLayers(view, progressDialog, fileName, type, saveAll, layerStack, previewChannelManager);

    // bring back original size of preview
    previewChannelManager.setChannelSize(originalSize, true);

    unlockUpdateImage();
    unlockPrepareImage();
    unlockHistogram();
    unlockLayerProcessor();

    gui.unlockSize();

    return result;
}

void deLayerProcessor::markUpdateSingleChannel(int index, int channel)
{
#ifdef DEBUG_LOG
    logInfo("markUpdateSingleChannel " +str(index) + " " + str(channel));
#endif    
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

void deLayerProcessor::lockLayerProcessor()
{
#ifdef DEBUG_LOG
    logInfo("layer processor lock...");
#endif    
    updateImagesMutex.lock();
#ifdef DEBUG_LOG
    logInfo("layer processor locked");
#endif    
}

void deLayerProcessor::unlockLayerProcessor()
{
#ifdef DEBUG_LOG
    logInfo("layer processor unlock");
#endif    
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
        threads->workerPost();
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
#ifdef DEBUG_LOG
    logInfo("prepare image start");
#endif    

    sendInfoEvent(DE_RENDERING_START);

    bool result = false;
    lockPrepareImage();
    lockLayerProcessor();

    if (!closing)
    {
        if (viewManager) 
        {
            result = renderer.prepareImage(*viewManager, *this, layerStack);
        }
    }

    unlockLayerProcessor();
    unlockPrepareImage();

    sendInfoEvent(DE_RENDERING_END);
#ifdef DEBUG_LOG
    logInfo("prepare image DONE");
#endif    

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

    previewChannelManager.setChannelSize(size, true);

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

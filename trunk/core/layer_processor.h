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

#ifndef _DE_LAYER_PROCESSOR_H
#define _DE_LAYER_PROCESSOR_H

class deMainWindow;
class deLayerStack;
class deViewManager;
class deChannelManager;
class deProgressDialog;
class deLayer;
class deLogger;
class deLayerFrameManager;
class deLayerProcessorThreads;
class deGUI;
#include <map>
#include "size.h"
#include "renderer.h"
#include "semaphore.h"
#include "base_layer.h"

enum
{
    DE_PROCESSING_START,
    DE_PROCESSING_END,
    DE_RENDERING_START,
    DE_RENDERING_END,
    DE_HISTOGRAM_START,
    DE_HISTOGRAM_END,
    DE_DCRAW_START,
    DE_DCRAW_END,
    DE_DEBUG_START,
    DE_DEBUG_END
};

class deLayerProcessor
{
    private:
        deLayerProcessorThreads* threads;
        deLayerStack& layerStack;
        deViewManager* viewManager;
        deLayerFrameManager& layerFrameManager;
        mutable deMutex layerProcessMutex;
        deMutex histogramMutex;
        deMutex prepareImageMutex;
        deMutex updateImageMutex;
        deMutex updateImagesMutex;
        deRenderer renderer;
        deChannelManager& previewChannelManager;
        deMainWindow& mainWindow;

        bool closing;

        deLayerProcessType layerProcessType;
        int layerProcessChannel;

        int firstLayerToUpdate;

        int lastValidLayer;

        void updateImages(int a, int channel, bool action);
        bool updateLayerImage();

        void repaintImageInLayerProcessor();

        deLayerProcessor(const deLayerProcessor&);
        deLayerProcessor& operator = (const deLayerProcessor&);

        void checkUpdateImagesRequest();

        int getLastLayerToUpdate();

        void lockLayers() const;
        void unlockLayers() const;

        void lockHistogram();
        void unlockHistogram();

        void lockPrepareImage();
        void unlockPrepareImage();

        void lockUpdateImage();
        void unlockUpdateImage();

        void updateWarning();

    public:
        deLayerProcessor(deChannelManager& _previewChannelManager, deLayerStack& _layerStack, deLayerFrameManager& _layerFrameManager, deMainWindow& _mainWindow);
        virtual ~deLayerProcessor();

        int getLastValidLayer() const;

        void setViewManager(deViewManager* _viewManager);

        void updateAllImages(bool calcHistogram);
        bool updateImagesSmart(deProgressDialog& progressDialog, const std::string& fileName, const std::string& type, bool saveAll, const deSize& size, deGUI& gui);

        void markUpdateSingleChannel(int index, int channel);
        void markUpdateAllChannels(int index);

        void markUpdateBlendAllChannels(int index);

        void onChangeView(int a);

        void lockLayerProcessor();
        void unlockLayerProcessor();

        void startWorkerThread();

        void tickWork();

        void onDestroyAll();

        void onChangeViewMode();

        void onGUIUpdate();

        void removeTopLayerInLayerProcessor();
        void addLayerInLayerProcessor(deBaseLayer* layer);

        void stopWorkerThread();

        void sendRepaintEvent();

        bool prepareImage();

        void generateHistogram();
        void onGenerateHistogram();
        void sendHistogramEvent();

        void setPreviewSize(const deSize& size, bool canSkip);
        void onImageLoad();
        void forceUpdateSize();

        bool isClosing() const {return closing;};

        void render(deCanvas& canvas);

        void removeAllLayers();

        void sendInfoEvent(int i);

        void setHistogramChannel(int channel);

};

#endif

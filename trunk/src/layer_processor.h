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
class wxProgressDialog;
class deMemoryInfoFrame;
class deLayer;
class deLogger;
class deLayerFrameManager;
#include <map>
#include <wx/wx.h>
#include "layer.h"
#include "size.h"
#include "renderer.h"
#include "semaphore.h"

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
        deMainWindow& mainWindow;
        deLayerStack& layerStack;
        deViewManager* viewManager;
        wxThread* workerThread;
        wxThread* renderWorkerThread;
        wxThread* histogramWorkerThread;
        deLayerFrameManager& layerFrameManager;
        deSemaphore workerSemaphore;
        deSemaphore renderWorkerSemaphore;
        deSemaphore histogramWorkerSemaphore;
        deMutex layerProcessMutex;
        deMutex histogramMutex;
        deMutex prepareImageMutex;
        deMutex updateImageMutex;
        deMutex updateImagesMutex;
        deRenderer renderer;
        deChannelManager& previewChannelManager;

        bool realtime;

        bool closing;

        deLayerProcessType layerProcessType;
        int layerProcessChannel;

        int firstLayerToUpdate;

        int lastValidLayer;

        void updateImages(int a, int channel, bool action);
        bool updateImage();

        void repaintImageInLayerProcessor();

        deLayerProcessor(const deLayerProcessor&);
        deLayerProcessor& operator = (const deLayerProcessor&);

        void checkUpdateImagesRequest();

        int getLastLayerToUpdate();

        void lockLayers();
        void unlockLayers();

        void lockHistogram();
        void unlockHistogram();

        void lockPrepareImage();
        void unlockPrepareImage();

        void lockUpdateImage();
        void unlockUpdateImage();

    public:
        deLayerProcessor(deChannelManager& _previewChannelManager, deLayerStack& _layerStack, deLayerFrameManager& _layerFrameManager, deMainWindow& _mainWindow);
        virtual ~deLayerProcessor();

        int getLastValidLayer() const {return lastValidLayer;};

        void setViewManager(deViewManager* _viewManager);

        void updateAllImages(bool calcHistogram);
        bool updateImagesSmart(int view, wxProgressDialog* progressDialog, deMemoryInfoFrame* memoryInfoFrame);
        void generateChannelUsage(std::map<int, int>& channelUsage);

        void markUpdateSingleChannel(int index, int channel);
        void markUpdateAllChannels(int index);

        void markUpdateBlendAllChannels(int index);

        void onChangeView(int a);

        void lock();
        void unlock();

        void startWorkerThread();

        void tickWork();

        void onDestroyAll();

        void onChangeViewMode();

        void onGUIUpdate();

        void removeTopLayerInLayerProcessor();
        void addLayerInLayerProcessor(deBaseLayer* layer, int layerIndex);

        void stopWorkerThread();

        void sendRepaintEvent();

        bool prepareImage();

        void generateHistogram();
        void onGenerateHistogram();
        void sendHistogramEvent();

        void setPreviewSize(const deSize& size);
        void onImageLoad();

        bool isClosing() const {return closing;};

        void render(deCanvas& canvas);

        bool isRealtime() const;
        void setRealtime(bool r);

        void removeAllLayers();

        void sendInfoEvent(int i);

};

#endif

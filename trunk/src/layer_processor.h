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

class deMainFrame;
class deLayerStack;
class deViewManager;
class deChannelManager;
class wxProgressDialog;
class deMemoryInfoFrame;
class deLayer;
class deLogger;
class deLayerFrameManager;
class deRenderer;
#include <map>
#include <wx/wx.h>
#include "layer.h"

class deLayerProcessor
{
    private:
        deMainFrame* mainFrame;
        deLayerStack* stack;
        deViewManager* viewManager;
        wxThread* workerThread;
        wxThread* renderWorkerThread;
        wxThread* histogramWorkerThread;
        deLayerFrameManager* layerFrameManager;
        wxSemaphore workerSemaphore;
        wxSemaphore renderWorkerSemaphore;
        wxSemaphore histogramWorkerSemaphore;
        wxMutex layerProcessMutex;
        wxMutex histogramMutex;
        wxMutex prepareImageMutex;
        wxMutex updateImageMutex;
        wxMutex updateImagesMutex;
        deRenderer* renderer;

        bool closing;

        deLayerProcessType layerProcessType;
        int layerProcessChannel;

        int firstLayerToUpdate;

        int lastValidLayer;

        void updateImages(int a, int channel, bool action);
        void updateImage();

        void repaintImageInLayerProcessor(bool calcHistogram);

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
        deLayerProcessor();
        virtual ~deLayerProcessor();

        int getLastValidLayer() const {return lastValidLayer;};

        void setMainFrame(deMainFrame* _mainFrame);
        void setLayerFrameManager(deLayerFrameManager* _layerFrameManager);
        void setLayerStack(deLayerStack* _layerStack);
        void setViewManager(deViewManager* _viewManager);

        void updateAllImages(bool calcHistogram);
        void updateImagesSmart(deChannelManager& channelManager, int view, wxProgressDialog* progressDialog, deMemoryInfoFrame* memoryInfoFrame);
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

        void removeTopLayer();
        void addLayer(deLayer* layer);

        void stopWorkerThread();

        void sendRepaintEvent();

        void setRenderer(deRenderer* _renderer);
        bool prepareImage();

        void generateHistogram();
        void onGenerateHistogram();
        void sendHistogramEvent();

};

#endif

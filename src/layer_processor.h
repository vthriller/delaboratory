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
        deLogger* logger;
        deLayerFrameManager* layerFrameManager;
        wxSemaphore workerSemaphore;
        wxSemaphore renderWorkerSemaphore;
        wxMutex layerProcessMutex;
        deRenderer* renderer;

        bool closing;

        deLayerProcessType layerProcessType;
        int layerProcessChannel;

        int firstLayerToUpdate;

        int lastValidLayer;

        void updateImages(int a, int channel, bool action);
        void updateImage();

        void repaintImageInLayerProcessor(bool calcHistogram);

        bool multithreadingEnabled;

        deLayerProcessor(const deLayerProcessor&);
        deLayerProcessor& operator = (const deLayerProcessor&);

        void checkUpdateImagesRequest();

        int getLastLayerToUpdate();


    public:
        deLayerProcessor();
        virtual ~deLayerProcessor();

        void setLogger(deLogger* _logger);

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

        void setMultithreadingEnabled(bool m) {multithreadingEnabled = m;};
        bool isMultithreadingEnabled() const {return multithreadingEnabled;};

        void removeTopLayer();
        void addLayer(deLayer* layer);

        void log(const std::string& message);

        void stopWorkerThread();

        void sendRepaintEvent();

        void setRenderer(deRenderer* _renderer);
        bool prepareImage();

};

#endif

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

#ifndef _DE_PROJECT_H
#define _DE_PROJECT_H

#include <string>
#include "channel_manager.h"
#include "layer_stack.h"
#include "view_manager.h"
#include "color_space.h"
#include "sampler_manager.h"

class deImagePanel;
class deHistogramPanel;
class deControlPanel;
class deViewModePanel;

class deProject
{
    private:
        deProject(const deProject& project);

        deLayerStack layerStack;
        deSamplerManager samplerManager;

        deChannelManager previewChannelManager;
        deChannelManager sourceChannelManager;

        deViewManager viewManager;

        deImagePanel* imagePanel;

        std::string imageFileName;
        std::string sourceImageFileName;

        bool receiveKeys;

        deHistogramPanel* histogramPanel;
        deViewModePanel* viewModePanel;
        deControlPanel* controlPanel;

    public:
        deProject();
        virtual ~deProject();
        void onKey(int key);
        void init(const std::string& fileName);
        void addLayer(deLayer* layer);
        void resetLayerStack();

        deChannelManager& getPreviewChannelManager();
        deChannelManager& getSourceChannelManager();
        deLayerStack& getLayerStack();

        void setPreviewSize(const deSize& size);

        const deViewManager& getViewManager() const;
        deViewManager& getViewManager();

        deSamplerManager& getSamplerManager();

        void setImagePanel(deImagePanel* _imagePanel);
        void repaintImage();

        void addLAB();
        void addRGB();

        void onChangeView(int a, int b);

        void exportTIFF(const std::string& app);

        void deleteLayer();

        void setLastView();

        bool shouldReceiveKeys() const {return receiveKeys;};

        void disableKeys();
        void enableKeys();

        void setHistogramPanel(deHistogramPanel* _histogramPanel);
        void setViewModePanel(deViewModePanel* _viewModePanel);
        void setControlPanel(deControlPanel* _controlPanel);
        void onChangeViewMode();
        void updateSamplers();

        bool samplersVisible() const;
        void save(const std::string& fileName);
        void load(const std::string& fileName);
        void newProject();

};

#endif

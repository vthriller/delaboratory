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

#include "layer_stack.h"
#include "sampler_manager.h"
#include "channel_manager.h"
#include "view_manager.h"

class deImagePanel;
class deHistogramPanel;
class deControlPanel;
class deViewModePanel;
class deHistogramModePanel;
class deImageAreaPanel;
class deMemoryInfoFrame;
class deImage;
class deMainFrame;

class deProject
{
    private:
        deProject(const deProject& project);
        deProject& operator =(const deProject& project);

        deLayerStack layerStack;
        deViewModePanel* viewModePanel;
        deControlPanel* controlPanel;
        deMemoryInfoFrame* memoryInfoFrame;
        deViewManager viewManager;
        deSamplerManager samplerManager;

        deChannelManager previewChannelManager;
        deChannelManager sourceChannelManager;

        std::string imageFileName;
        std::string sourceImageFileName;

        bool receiveKeys;

        deHistogramPanel* histogramPanel;
        deHistogramModePanel* histogramModePanel;
        deImageAreaPanel* imageAreaPanel;
        deMainFrame* mainFrame;

        void loadLayers(xmlNodePtr root);
        void loadLayer(xmlNodePtr root);

        void freeImage();

        void onScaleSet();

        void saveImage(const std::string& filename, const deImage& image, const std::string& type);

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

        void setPreviewSize(const deSize& size, bool calcHistogram);

        const deViewManager& getViewManager() const;
        deViewManager& getViewManager();

        deSamplerManager& getSamplerManager();

        void repaintImage(bool calcHistogram);

        void addLAB();
        void addRGB();

        void onChangeView(int a, int b);

        void exportFinalImage(const std::string& app, const std::string& type, const std::string& name, wxProgressDialog* progressDialog);

        void deleteLayer();

        void setLastView();

        bool shouldReceiveKeys() const {return receiveKeys;};

        void disableKeys();
        void enableKeys();

        void setHistogramPanel(deHistogramPanel* _histogramPanel);
        void setViewModePanel(deViewModePanel* _viewModePanel);
        void setHistogramModePanel(deHistogramModePanel* _histogramModePanel);
        void setControlPanel(deControlPanel* _controlPanel);
        void onChangeViewMode();
        void updateSamplers();

        deHistogramPanel* getHistogramPanel();

        bool samplersVisible() const;
        void showSamplers();
        void save(const std::string& fileName, bool image);
        void open(const std::string& fileName, bool image);
        bool openImage(const std::string& fileName);
        void newProject();
        void setTestImage(int s);

        void setImageAreaPanel(deImageAreaPanel* _imageAreaPanel);

        void openMemoryInfoFrame(wxWindow* parent);
        void closeMemoryInfoFrame();

        void updateMemoryInfo();

        void setHistogramChannel(int channel);
        void zoom(int a);
        void setViewOffset(deValue x, deValue y);

        void fullZoomOut();
        void maxZoom();

        void setMainFrame(deMainFrame* _mainFrame);


};

#endif

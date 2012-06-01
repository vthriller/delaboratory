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

#include "sampler_manager.h"
#include "view_manager.h"
#include "logger.h"
#include "image.h"
#include <wx/progdlg.h>
#include "size.h"
#include <libxml/parser.h>

class deImagePanel;
class deHistogramPanel;
class deLayerGridPanel;
class deViewModePanel;
class deHistogramModePanel;
class deImageAreaPanel;
class deMainWindow;
class deLayerProcessor;
class deLayerStack;
class deLayerFrameManager;
class deLayer;
class deStaticImage;
class deRawModule;
class deZoomManager;
class deBaseLayer;

class deProject
{
    private:
        deProject(const deProject& project);
        deProject& operator =(const deProject& project);

        deLayerProcessor& layerProcessor;
        std::string imageFileName;
        std::string sourceImageFileName;
        deViewModePanel* viewModePanel;
        deChannelManager& channelManager;
        deViewManager viewManager;
        deStaticImage& sourceImage;
        deLayerStack& layerStack;
        deLayerFrameManager& layerFrameManager;
        deLayerGridPanel* layerGridPanel;
        deHistogramModePanel* histogramModePanel;
        deImageAreaPanel* imageAreaPanel;
        deRawModule& rawModule;
        deZoomManager& zoomManager;
        deMainWindow& mainWindow;

        void onImageNameUpdate();

        void loadLayers(xmlNodePtr root);
        void loadLayer(xmlNodePtr root, int layerIndex);

        void freeImage();

        void onScaleSet();

        void updateLayerGrid();

        bool openImageRAW(const std::string& fileName);

    public:
        deProject(deLayerProcessor& _processor, deChannelManager& _channelManager, deLayerStack& _layerStack, deLayerFrameManager& _layerFrameManager, deStaticImage& _sourceImage, deRawModule& _rawModule, deZoomManager& _zoomManager, deMainWindow& _mainWindow);

        virtual ~deProject();
        void onKey(int key);
        void init(const std::string& fileName);
        void resetLayerStack(deColorSpace colorSpace);

        deChannelManager& getPreviewChannelManager();
        deSize getSourceImageSize();
        deLayerStack& getLayerStack();
        deLayerProcessor& getLayerProcessor();

        bool isSourceValid() const;

        const deViewManager& getViewManager() const;
        deViewManager& getViewManager();

        void onChangeView(int a);

        bool exportFinalImage(const std::string& app, const std::string& type, const std::string& name, wxProgressDialog* progressDialog, bool saveAll, const std::string& dir);

        void setViewModePanel(deViewModePanel* _viewModePanel);
        void setHistogramModePanel(deHistogramModePanel* _histogramModePanel);
        void setLayerGridPanel(deLayerGridPanel* _Panel);
        void onChangeViewMode();

        bool openImage(const std::string& fileName, bool raw, deColorSpace colorSpace);
        void newProject();
        void setTestImage(int s);

        void setImageAreaPanel(deImageAreaPanel* _imageAreaPanel);

        void setHistogramChannel(int channel);

        void onTimerUpdate();

        deBaseLayer* createNewLayer(const std::string& type);

        void onAddNewLayer();
        void onRemoveTopLayer();

        deLayerFrameManager& getLayerFrameManager() {return layerFrameManager;};
        deValue getSourceAspect() const;

};

#endif

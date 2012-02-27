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
class deControlPanel;
class deViewModePanel;
class deHistogramModePanel;
class deImageAreaPanel;
class deMemoryInfoFrame;
class deMainFrame;
class deLayerProcessor;
class deLayerStack;
class deLayerFrameManager;
class deLayer;
class deStaticImage;
class deRawModule;
class deZoomManager;

class deProject
{
    private:
        deProject(const deProject& project);
        deProject& operator =(const deProject& project);

        deLayerProcessor& layerProcessor;
        deViewModePanel* viewModePanel;
        deChannelManager& previewChannelManager;

        deControlPanel* controlPanel;
        deMemoryInfoFrame* memoryInfoFrame;
        deViewManager viewManager;

        deMainFrame* mainFrame;

        deStaticImage& sourceImage;

        deLayerStack& layerStack;

        deLayerFrameManager& layerFrameManager;

        std::string imageFileName;
        std::string sourceImageFileName;

        bool receiveKeys;

        deHistogramModePanel* histogramModePanel;
        deImageAreaPanel* imageAreaPanel;

        deRawModule& rawModule;

        deZoomManager& zoomManager;

        void onImageNameUpdate();

        void loadLayers(xmlNodePtr root);
        void loadLayer(xmlNodePtr root);

        void freeImage();

        void onScaleSet();

        void saveImage(const std::string& filename, const deImage& image, const std::string& type);


    public:
        deProject(deLayerProcessor& _processor, deChannelManager& _previewChannelManager, deLayerStack& _layerStack, deLayerFrameManager& _layerFrameManager, deStaticImage& _sourceImage, deRawModule& _rawModule, deZoomManager& _zoomManager);

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

        void addLAB();
        void addRGB();

        void onChangeView(int a);

        bool exportFinalImage(const std::string& app, const std::string& type, const std::string& name, wxProgressDialog* progressDialog);

        void setLastView();

        bool shouldReceiveKeys() const {return receiveKeys;};

        void disableKeys();
        void enableKeys();

        void setViewModePanel(deViewModePanel* _viewModePanel);
        void setHistogramModePanel(deHistogramModePanel* _histogramModePanel);
        void setControlPanel(deControlPanel* _controlPanel);
        void onChangeViewMode();

        void save(const std::string& fileName, bool image);
        void open(const std::string& fileName, bool image);
        bool openImage(const std::string& fileName, bool raw, deColorSpace colorSpace);
        void newProject();
        void setTestImage(int s);

        void setImageAreaPanel(deImageAreaPanel* _imageAreaPanel);

        void openMemoryInfoFrame(wxWindow* parent);
        void closeMemoryInfoFrame();

        void updateMemoryInfo();

        void setHistogramChannel(int channel);

        void setMainFrame(deMainFrame* _mainFrame);

        void addRandomLayer();

        deLayerFrameManager& getLayerFrameManager() {return layerFrameManager;};

        void log(const std::string& message);

        void addActionLayer(const std::string& action);
        void addConversionLayer(deColorSpace colorSpace);

};

#endif

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

#include "project.h"
#include "message_box.h"
#include "image_io.h"
#include <cassert>
#include <iostream>
#include "source_image_layer.h"
#include "curves_layer.h"
#include "conversion_layer.h"
#include "str.h"
#include <sstream>
#include "layer_factory.h"
#include <iostream>
#include "layer_processor.h"
#include "external_editor.h"
#include "channel_manager.h"
#include "layer_stack.h"
#include "layer_frame_manager.h"
#include "static_image.h"
#include "zoom_manager.h"
#include "color_space_utils.h"
#include "operation_processor.h"
#include "main_window.h"
#include "conversion_processor.h"
#include "test_image.h"
#include "main_frame_events.h"

#include "raw_module.h"
#include "dcraw_support.h"
#include "tmp.h"

deProject::deProject(deLayerProcessor& _processor, deChannelManager& _channelManager, deLayerStack& _layerStack, deLayerFrameManager& _layerFrameManager, deStaticImage& _sourceImage, deRawModule& _rawModule, deZoomManager& _zoomManager, deMainWindow& _mainWindow, deGUI& _gui)
:layerProcessor(_processor),
 channelManager(_channelManager),
 viewManager(*this, _processor, _zoomManager),
 sourceImage(_sourceImage),
 layerStack(_layerStack),
 layerFrameManager(_layerFrameManager),
 rawModule(_rawModule),
 zoomManager(_zoomManager),
 mainWindow(_mainWindow),
 gui(_gui)
{
    imageFileName = "";
    sourceImageFileName = "";

    logInfo("project constructor");

    layerProcessor.setViewManager(&viewManager);

    resetLayerStack(deColorSpaceRGB);
}

deProject::~deProject()
{
    logInfo("project destructor");
    layerProcessor.lockLayerProcessor();
    layerProcessor.unlockLayerProcessor();
    layerStack.clear();
}

void deProject::setHistogramChannel(int channel)
{
    logInfo("project set histogram channel " + str(channel));
    gui.updateHistogramMode(channel);
    layerProcessor.generateHistogram();
}

void deProject::onKey(int key)
{
    if (key == '`')
    {
        viewManager.setNormal();
    }
    if (key == '1')
    {
        viewManager.setSingleChannel(0);
    }
    if (key == '2')
    {
        viewManager.setSingleChannel(1);
    }
    if (key == '3')
    {
        viewManager.setSingleChannel(2);
    }
    if (key == '4')
    {
        viewManager.setSingleChannel(3);
    }
    /*
    if (key == WXK_F1)
    {
        setHistogramChannel(0);
    }
    if (key == WXK_F2)
    {
        setHistogramChannel(1);
    }
    if (key == WXK_F3)
    {
        setHistogramChannel(2);
    }
    if (key == WXK_F4)
    {
        setHistogramChannel(3);
    }
    */

    layerFrameManager.onKey(key);
}

void deProject::init(const std::string& fileName)
{
    bool ok = false;
    if (openImage(fileName, true, false, false))
    {
        ok = true;
    } else if (openImage(fileName, false, false, false))
    {
        ok = true;
    }
    if (!ok)
    {
        std::string s = "unable to open image: " + fileName;
        logError(s);
        showMessageBox(s);
        return;
    }        
}

void deProject::freeImage()
{
}

void deProject::setTestImage(int s)
{
    if (rawModule.isActive())
    {
        showMessageBox("RAW module is already active");
        return;
    }

    generateTestImage(sourceImage, s);
    gui.updateImageAreaSize();
    resetLayerStack(sourceImage.getColorSpace());
    imageFileName = "test";

}

void deProject::resetLayerStack(deColorSpace colorSpace)
{
    logInfo("reset layer stack");

    layerProcessor.removeAllLayers();

    deBaseLayer* layer = createLayer("original", -1, colorSpace, layerStack, channelManager, viewManager, sourceImage);

    if (layer)
    {
        layer->allocateChannels();
        layerProcessor.addLayerInLayerProcessor(layer);
    }        

    layerProcessor.updateAllImages(true);

    updateLayerGrid();

    viewManager.setLastView();

}

deChannelManager& deProject::getPreviewChannelManager() 
{
    return channelManager;
}

deSize deProject::getSourceImageSize() 
{
    deSize size = sourceImage.getStaticImageSize();

    deValue x1;
    deValue y1;
    deValue x2;
    deValue y2;

    zoomManager.getZoom(x1, y1, x2, y2);

    deValue w = size.getW() * (x2 - x1);
    deValue h = size.getH() * (y2 - y1);

    return deSize(w, h);
}

deValue deProject::getSourceAspect() const
{
    const deBaseLayer* layer = layerStack.startReadLayer(0);
    deValue aspect = -1;

    const deSourceImageLayer* source = dynamic_cast<const deSourceImageLayer*>(layer);
    if (source)
    {
        deValue x1;
        deValue y1;
        deValue x2;
        deValue y2;

        zoomManager.getZoom(x1, y1, x2, y2);

        deValue dx = x2 - x1;
        deValue dy = y2 - y1;
        if ((dx > 0) && (dy > 0))
        {
            deValue a = dy / dx;

            aspect = source->getAspect() / a;
        }            
    }

    layerStack.finishReadLayer(0);
    return aspect;
}    

deLayerStack& deProject::getLayerStack()
{
    return layerStack;
}

deLayerProcessor& deProject::getLayerProcessor()
{
    return layerProcessor;
}

void deProject::onChangeView(int a)
{
    logInfo("change view from " + str(a) + " START");
    layerProcessor.onChangeView(a);

    gui.updateViewModePanelNames();
    gui.updateHistogramNames();
    mainWindow.rebuild();
    logInfo("change view from " + str(a) + " DONE");
}

const deViewManager& deProject::getViewManager() const
{
    return viewManager;
}

deViewManager& deProject::getViewManager() 
{
    return viewManager;
}

bool deProject::exportFinalImage(const std::string& app, const std::string& type, const std::string& name, deProgressDialog& progressDialog, bool saveAll, const std::string& dir)
{
    // FIXME without dynamic channel allocation all channels are allocated from start - so it will crash with out of memory

    logInfo("exportFinalImage...");

    // name is taken from file dialog, it can be empty when we are exporting to external editor
    // but in this case we need correct imageFileName
    if ((name == "") && (imageFileName == ""))
    {
        showMessageBox( "exporting final image failed - no file name set");
        return false;
    }

    std::string fileName;
    
    if (name == "")
    {
        // path is a directory for temporary save, used only when exporting to external editor
        std::string path = getTmp();

        if (dir.size() > 0)
        {
            // now used also when exporting all images into one dir
            path = dir;
        }

        // we save file in the temporary directory
        fileName = path + "/" + imageFileName + "." + type;
    }        
    else
    {
        // wa save file in the location taken from file dialog
        fileName = name;
    }

    bool result = processFullSizeImage(fileName, type, saveAll, progressDialog);

    if (!result)
    {
        showMessageBox( "exporting final image failed - error during update images\n(probably out of memory)");
    }

    if (result)
    {
        // execute external editor
        if (app.size() > 0)
        {
            executeExternalEditor(fileName, app);
        }
    }

    // calculate image in preview size to continue editing
    layerProcessor.updateAllImages(true);

    return result;
}

bool deProject::processFullSizeImage(const std::string& fileName, const std::string& type, bool saveAll, deProgressDialog& progressDialog)
{
    logInfo("processFullSizeImage...");

    bool result = layerProcessor.updateImagesSmart(progressDialog, fileName, type, saveAll, sourceImage.getStaticImageSize(), gui);

    return result;
}


void deProject::onChangeViewMode()
{
    gui.updateViewModePanelMode();
}

void deProject::newProject()
{
    resetLayerStack(deColorSpaceRGB);
    viewManager.setLastView();
    updateLayerGrid();
}

bool deProject::openImageRAW(const std::string& fileName, bool srgb, bool brighten)
{

    bool valid = isRawValid(fileName);
    if (!valid)
    {
        logError("not a valid RAW " + fileName);
        return false;
    }

/*
    std::string info;
    info = getRawInfo(fileName);

    if (info.size() == 0)
    {
        logError("empty RAW info in " + fileName);
        return false;
    }
    */

    layerProcessor.sendInfoEvent(DE_DCRAW_START);
    if (rawModule.loadRAW(fileName, sourceImage, true, srgb, brighten))
    {
        logInfo("found RAW " + fileName);
        bool failure = false;
        while (!rawModule.updateRawLoading(failure))
        {
            sleep(200);
            if (failure)
            {
                logError("failed RAW load " + fileName);
                layerProcessor.sendInfoEvent(DE_DCRAW_END);
                return false;
            }
        }
        bool result = rawModule.loadRAW(fileName, sourceImage, false, srgb, brighten);
        if (!result)
        {
            return false;
        }

        mainWindow.startRawTimer();

    }
    else
    {
        logError("failed RAW " + fileName);
        layerProcessor.sendInfoEvent(DE_DCRAW_END);
        return false;
    }

    return true;
}

bool deProject::openImage(const std::string& fileName, bool raw, bool srgb, bool brighten)
{
    freeImage();

    logInfo("open image " + fileName);

    deColorSpace oldColorSpace = sourceImage.getColorSpace();

    if (raw)
    { 
        if (!openImageRAW(fileName, srgb, brighten))
        {
            return false;
        }
    }
    else
    {
        logInfo("trying JPEG/TIFF image " + fileName);
        if (!loadImage(fileName, sourceImage))
        {
            logError("load JPEG/TIFF image failed: " + fileName);
            return false;
        }
    }            


    imageFileName = removePathAndExtension(fileName);
    onImageNameUpdate();
    sourceImageFileName = fileName;

    gui.updateImageAreaSize();
    layerProcessor.updateAllImages(true);

    deColorSpace newColorSpace = sourceImage.getColorSpace();

    if (oldColorSpace != newColorSpace)
    {
        resetLayerStack(newColorSpace);
        onChangeView(0);
    }

    return true;
}

bool deProject::isSourceValid() const
{
    return (!channelManager.isImageEmpty());
}

deBaseLayer* deProject::createNewLayer(const std::string& type)
{
    logInfo("createNewLayer " + type);

    int s = viewManager.getView();

    deBaseLayer* layer = NULL;

    const deBaseLayer* vLayer = layerStack.startReadLayer(s);
    if (vLayer)
    {
        deColorSpace colorSpace = vLayer->getColorSpace();

        layer = createLayer(type, s, colorSpace, layerStack, channelManager, viewManager, sourceImage);
    }
    layerStack.finishReadLayer(s);

    if (!layer)
    {
        deColorSpace colorSpace = colorSpaceFromString(type);

        if (colorSpace != deColorSpaceInvalid)
        {
            layer = createLayer("conversion", s, colorSpace, layerStack, channelManager, viewManager, sourceImage);
        }            
    }

    if (layer)
    {
        logInfo("allocate channels in new layer " + type);
        layer->allocateChannels();
    }

    return layer;
}

void deProject::onImageNameUpdate()
{
    mainWindow.setImageName(imageFileName, sourceImage.getStaticImageSize());
}

void deProject::onTimerUpdate()
{
    bool failure = false;

    bool result = rawModule.updateRawLoading(failure);

    if ((result) || (failure))
    {
        layerProcessor.sendInfoEvent(DE_DCRAW_END);
        mainWindow.stopRawTimer();
    }

    if (result)
    {
        mainWindow.postEvent(DE_IMAGE_LOAD_EVENT, 0);
    }

}

void deProject::updateLayerGrid()
{
    gui.updateLayerGrid();
}

void deProject::onAddNewLayer()
{
    viewManager.setLastView();
    updateLayerGrid();
}        

void deProject::onRemoveTopLayer()
{
    viewManager.setLastView();
    updateLayerGrid();
}        

deSize deProject::onImageAreaChangeSize(const deSize& ps, bool canSkip)
{
    deValue aspect = getSourceAspect();

    if (aspect <= 0)
    {
        logInfo("image area panel update size skipped, aspect is 0");
        return deSize(0,0);
    }

    deSize fit = fitInside(ps, aspect);

    layerProcessor.setPreviewSize(fit, canSkip);

    return fit;
}    


void deProject::openLayerFrame(int index)
{
    deBaseLayer* layer = layerStack.getLayer(index);
    if (layer)
    {
        gui.openLayerFrame(*layer, layerProcessor, layerFrameManager, index);
    }        

}            

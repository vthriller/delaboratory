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
#include "image_io.h"
#include <cassert>
#include <wx/wx.h>
#include <iostream>
#include "source_image_layer.h"
#include "curves_layer.h"
#include "conversion_layer.h"
#include "conversion_functions.h"
#include "image_panel.h"
#include "str.h"
#include "histogram_panel.h"
#include "histogram_mode_panel.h"
#include "control_panel.h"
#include "view_mode_panel.h"
#include <sstream>
#include "layer_factory.h"
#include "convert_image.h"
#include "fractal.h"
#include "xml.h"
#include "image_area_panel.h"
#include "memory_info_frame.h"
#include <iostream>
#include "main_frame.h"
#include "layer_processor.h"
#include "external_editor.h"
#include "channel_manager.h"
#include "layer_stack.h"
#include "layer_frame_manager.h"

#define ICC_MESSAGE 0

const std::string LOG_FILE_NAME = "debug.log";
const std::string LOG_LOCKS_FILE_NAME = "locks.log";

deProject::deProject(deLayerProcessor& _processor, deChannelManager& _previewChannelManager, deChannelManager& _sourceChannelManager, deLayerStack& _layerStack, deLayerFrameManager& _layerFrameManager)
:layerProcessor(_processor),
 previewChannelManager(_previewChannelManager),
 sourceChannelManager(_sourceChannelManager),
 viewModePanel(NULL),
 controlPanel(NULL),
 memoryInfoFrame(NULL),
 viewManager(*this, _processor),
 samplerManager(_processor),
 mainFrame(NULL),
 sourceImage(deColorSpaceRGB, sourceChannelManager),
 layerStack(_layerStack),
 layerFrameManager(_layerFrameManager)
{
    imageFileName = "";
    sourceImageFileName = "";
    receiveKeys = true;
    showSamplers = false;

    sourceImage.enableChannel(0);
    sourceImage.enableChannel(1);
    sourceImage.enableChannel(2);

    deLogger::getLogger().setFile(LOG_FILE_NAME);
    deLogger::getLogger().setLocksFile(LOG_LOCKS_FILE_NAME);

    log("project started");

    layerProcessor.setViewManager(&viewManager);

    resetLayerStack();

}

void deProject::disableKeys()
{
    receiveKeys = false;
}

void deProject::enableKeys()
{
    receiveKeys = true;
}

deProject::~deProject()
{
    log("closing project");
    layerProcessor.lock();
    layerProcessor.unlock();
    layerStack.clear();
    log("closed project");
}

void deProject::setHistogramChannel(int channel)
{
    histogramModePanel->updateMode(channel);
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

    layerFrameManager.onKey(key);
    if (controlPanel)
    {
        controlPanel->onKey(key);
    }        
}

void deProject::init(const std::string& fileName)
{
    logMessage("project init " + fileName);
    if (!openImage(fileName))
    {
        open(fileName, true);
    }
}

void deProject::freeImage()
{
}

void deProject::setTestImage(int s)
{
    freeImage();

    deSize size(s, s);

    sourceChannelManager.setChannelSize(size);

    deChannel* channelR = sourceChannelManager.getChannel(sourceImage.getChannelIndex(0));
    deChannel* channelG = sourceChannelManager.getChannel(sourceImage.getChannelIndex(1));
    deChannel* channelB = sourceChannelManager.getChannel(sourceImage.getChannelIndex(2));

    generateFractal(channelR->getPixels(), channelG->getPixels(), channelB->getPixels(), size);

    imageFileName = "delaboratory_test_image";

    previewChannelManager.destroyAllChannels();
    imageAreaPanel->updateSize(true);
    layerProcessor.updateAllImages(true);
}

void deProject::resetLayerStack()
{
    logMessage("reset layer stack");

    layerStack.clear();

    deLayer* layer = createLayer("source_image", -1, deColorSpaceRGB, layerStack, previewChannelManager, viewManager, "source image", sourceChannelManager, sourceImage);

    if (layer)
    {
        layerProcessor.addLayer(layer);
    }        

    previewChannelManager.destroyAllChannels();
    layerProcessor.updateAllImages(true);
}

deChannelManager& deProject::getPreviewChannelManager() 
{
    return previewChannelManager;
}

deChannelManager& deProject::getSourceChannelManager() 
{
    return sourceChannelManager;
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
    logMessage("change view from " + str(a) + " start");
    layerProcessor.onChangeView(a);

    if (controlPanel)
    {
        controlPanel->onChangeView();
    }
    if (viewModePanel)
    {
        viewModePanel->updateNames();
    }
    if (histogramModePanel)
    {
        histogramModePanel->updateNames();
    }
    updateMemoryInfo();
    if (mainFrame)
    {
        mainFrame->rebuild();
    }
    logMessage("change view from " + str(a) + " end");
}

const deViewManager& deProject::getViewManager() const
{
    return viewManager;
}

deViewManager& deProject::getViewManager() 
{
    return viewManager;
}

deSamplerManager& deProject::getSamplerManager() 
{
    return samplerManager;
}

void deProject::saveImage(const std::string& fileName, const deImage& image, const std::string& type)
{
    if (image.getColorSpace() == deColorSpaceRGB)
    {
        deChannel* r = previewChannelManager.getChannel(image.getChannelIndex(0));
        deChannel* g = previewChannelManager.getChannel(image.getChannelIndex(1));
        deChannel* b = previewChannelManager.getChannel(image.getChannelIndex(2));

        if (type == "tiff")
        {
            saveTIFF(fileName, *r, *g, *b, previewChannelManager.getChannelSize());
        }            
        if (type == "jpeg")
        {
            saveJPEG(fileName, *r, *g, *b, previewChannelManager.getChannelSize());
        }            
    }
    else
    {
        deImage finalImage(deColorSpaceRGB, previewChannelManager);
        finalImage.enableAllChannels();
        convertImage(image, finalImage, previewChannelManager);
        deChannel* r = previewChannelManager.getChannel(finalImage.getChannelIndex(0));
        deChannel* g = previewChannelManager.getChannel(finalImage.getChannelIndex(1));
        deChannel* b = previewChannelManager.getChannel(finalImage.getChannelIndex(2));
        if (type == "tiff")
        {
            saveTIFF(fileName, *r, *g, *b, previewChannelManager.getChannelSize());
        }            
        if (type == "jpeg")
        {
            saveJPEG(fileName, *r, *g, *b, previewChannelManager.getChannelSize());
        }            
    }
}

void deProject::exportFinalImage(const std::string& app, const std::string& type, const std::string& name, wxProgressDialog* progressDialog)
{
    // name is taken from file dialog, it can be empty when we are exporting to external editor
    // but in this case we need correct imageFileName
    if ((name == "") && (imageFileName == ""))
    {
        wxMessageBox( _T("exporting final image failed - no file name set"));
        return;
    }

    std::string fileName;
    
    if (name == "")
    {
        // path is a directory for temporary save, used only when exporting to external editor
        std::string path = "";

        wxString temp;
        if (wxGetEnv(_T("TEMP"), &temp))
        {
            // on Windows $TEMP should be set
            path = str(temp);
        }
        else
        {
            path = "/tmp/";
        }            
        // we save file in the temporary directory
        fileName = path + imageFileName + "." + type;
    }        
    else
    {
        // wa save file in the location taken from file dialog
        fileName = name;
    }

    // remember original size of preview
    deSize originalSize = previewChannelManager.getChannelSize();

    // calculate final image in full size
    int view = viewManager.getView();
    previewChannelManager.setChannelSize(sourceChannelManager.getChannelSize());
    layerProcessor.updateImagesSmart(view, progressDialog, memoryInfoFrame);

    // take the final image
    deLayer* layer = layerStack.getLayer(view);
    const deImage& image = layer->getImage();

    // save it
    saveImage(fileName, image, type);

    // bring back original size of preview
    previewChannelManager.setChannelSize(originalSize);

    // execute external editor
    if (app.size() > 0)
    {
        executeExternalEditor(fileName, app);
    }

    // calculate image in preview size to continue editing
    layerProcessor.updateAllImages(true);
}

void deProject::setLastView()
{
    int n = layerStack.getSize();
    n--;
    viewManager.setView(n);
}

void deProject::setViewModePanel(deViewModePanel* _viewModePanel)
{
    viewModePanel = _viewModePanel;
}

void deProject::setHistogramModePanel(deHistogramModePanel* _histogramModePanel)
{
    histogramModePanel = _histogramModePanel;
}


void deProject::setControlPanel(deControlPanel* _controlPanel)
{
    controlPanel = _controlPanel;
}

void deProject::onChangeViewMode()
{
    if (viewModePanel)
    {
        viewModePanel->updateMode();
    }
}

bool deProject::samplersVisible() const
{
    if (showSamplers)
    {
        return true;
    }
    return false;
}

void deProject::save(const std::string& fileName, bool image)
{
    std::string f = fileName;

    size_t pos = f.rfind(".delab");
    if (pos != f.size() - 6) 
    {
        f += ".delab";
    }

    xmlDocPtr doc = xmlNewDoc(BAD_CAST("1.0"));

    xmlNodePtr root = xmlNewNode(NULL, BAD_CAST("project"));
    xmlDocSetRootElement(doc, root);

    {
        xmlNodePtr child = xmlNewChild(root, NULL, BAD_CAST("layer_stack"), NULL);
        layerStack.save(child);

        if (image)
        {
            saveChild(root, "source_image", sourceImageFileName);
        }
    }

    xmlSaveFormatFile (f.c_str(), doc, 1); 
}

void deProject::loadLayer(xmlNodePtr root)
{
    xmlNodePtr child = root->xmlChildrenNode;

    std::string type = "";
    std::string name = "";
    int source = -1;
    deColorSpace colorSpace = deColorSpaceInvalid;

    while (child)
    {
        if ((!xmlStrcmp(child->name, BAD_CAST("source")))) 
        {
            source = getInt(getContent(child));
        }

        if ((!xmlStrcmp(child->name, BAD_CAST("color_space")))) 
        {
            colorSpace = colorSpaceFromString(getContent(child));
        }

        if (type == "")
        // this is fix for multiple "type" node in single layer
        {
            if ((!xmlStrcmp(child->name, BAD_CAST("type")))) 
            {
                type = getContent(child);
            }
        }
        
        if ((!xmlStrcmp(child->name, BAD_CAST("name")))) 
        {
            name = getContent(child);
        }

        child = child->next;
    }
       
    deLayer* layer = createLayer(type, source, colorSpace, layerStack, previewChannelManager, viewManager, name, sourceChannelManager, sourceImage);

    if (layer)
    {
        layerProcessor.addLayer(layer);
        layer->load(root);
    }        


}

void deProject::loadLayers(xmlNodePtr root)
{
    layerStack.clear();

    xmlNodePtr child = root->xmlChildrenNode;

    while (child)
    {
        if ((!xmlStrcmp(child->name, BAD_CAST("layer")))) 
        {
            loadLayer(child);
        }

        child = child->next;
    }

    previewChannelManager.destroyAllChannels();
    layerProcessor.updateAllImages(true);
}

void deProject::open(const std::string& fileName, bool image)
{
//    std::cout << "open " << fileName << std::endl;

    xmlDocPtr doc = xmlParseFile(fileName.c_str());

    if (!doc)
    {
        return;
    }

    xmlNodePtr root = xmlDocGetRootElement(doc);

    if (!root)
    {
        return;
    }

    xmlNodePtr child = root->xmlChildrenNode;

    std::string imageFile = "";

    while (child)
    {
        if ((!xmlStrcmp(child->name, BAD_CAST("layer_stack")))) 
        {
            loadLayers(child);
        }

        if ((!xmlStrcmp(child->name, BAD_CAST("source_image")))) 
        {
            imageFile = getContent(child);
        }

        child = child->next;
    }

    if (image)
    {
        openImage(imageFile);
    }        

    setLastView();
    controlPanel->updateLayerGrid();
}

void deProject::newProject()
{
    resetLayerStack();
    setLastView();
    controlPanel->updateLayerGrid();
}

void deProject::setImageAreaPanel(deImageAreaPanel* _imageAreaPanel)
{
    imageAreaPanel = _imageAreaPanel;
}

bool deProject::openImage(const std::string& fileName)
{
    freeImage();

    logMessage("open image " + fileName);

    bool tiff = checkTIFF(fileName);
    bool jpeg = checkJPEG(fileName);

    if ((!tiff) && (!jpeg))
    {
        return false;
    }

    if ((tiff) && (jpeg))
    {
        assert(false);
    }

    deSize size(0,0);

    if (tiff)
    {
        size = getTIFFSize(fileName);
    }

    if (jpeg)
    {
        size = getJPEGSize(fileName);
    }

    sourceChannelManager.setChannelSize(size);

    deChannel* channelR = sourceChannelManager.getChannel(sourceImage.getChannelIndex(0));
    if (!channelR)
    {
        return false;
    }
    deChannel* channelG = sourceChannelManager.getChannel(sourceImage.getChannelIndex(1));
    if (!channelG)
    {
        return false;
    }
    deChannel* channelB = sourceChannelManager.getChannel(sourceImage.getChannelIndex(2));
    if (!channelB)
    {
        return false;
    }

    bool status = false;

    if (tiff)
    {
        bool icc;
        loadTIFF(fileName, *channelR, *channelG, *channelB, icc);
#if ICC_MESSAGE
        if (icc)
        {
            wxMessageBox( _T("Warning! this TIFF file contains ICC profile which is ignored by delaboratory\n\ndelaboratory expects sRGB - colors may be not accurate\n\nThis problem happens (for instance) when tiff is created by dcraw\nyou can fix it by calling tifficc command, by default it converts tiff to sRGB"), _T("ICC profile ignored"), wxOK | wxICON_INFORMATION, NULL );
        }
#endif        
        status = true;
    }

    if (jpeg)
    {
        loadJPEG(fileName, *channelR, *channelG, *channelB);
        status = true;
    }

    imageFileName = removePathAndExtension(fileName);
    sourceImageFileName = fileName;

    previewChannelManager.destroyAllChannels();
    imageAreaPanel->updateSize(true);
    layerProcessor.updateAllImages(true);

    return status;
}

void deProject::openMemoryInfoFrame(wxWindow* parent)
{
    if (!memoryInfoFrame)
    {
        memoryInfoFrame = new deMemoryInfoFrame(parent, *this);
        memoryInfoFrame->Show();
    }
}

void deProject::closeMemoryInfoFrame()
{
    memoryInfoFrame = NULL;
}

void deProject::updateMemoryInfo()
{
    if (memoryInfoFrame)
    {
        memoryInfoFrame->update();
    }
}

void deProject::setMainFrame(deMainFrame* _mainFrame)
{
    log("set main frame in project");
    mainFrame = _mainFrame;
}

void deProject::setShowSamplers(bool show)
{
    showSamplers = show;
    samplerManager.setMoving(show);
}

bool deProject::isSourceValid() const
{
    return (previewChannelManager.getChannelSize().getN() > 0);
}

void deProject::log(const std::string& message)
{
    logMessage(message);
}

void deProject::addActionLayer(const std::string& action)
{
    int s = viewManager.getView();

    deColorSpace colorSpace = layerStack.getLayer(s)->getColorSpace();

    std::string actionDescription = getActionDescription(action);

    log("creating action " + action + " layer");

    deLayer* layer = createLayer(action, s, colorSpace, layerStack, previewChannelManager, viewManager, actionDescription, sourceChannelManager, sourceImage);

    if (layer)
    {
        layerProcessor.addLayer(layer);
        setLastView();

        controlPanel->updateLayerGrid();
    }
}    

void deProject::addConversionLayer(deColorSpace colorSpace)
{
    int s = viewManager.getView();

    std::string name = getColorSpaceName(colorSpace);

    log("creating conversion to " + name + " layer");

    deLayer* layer = createLayer("conversion", s, colorSpace, layerStack, previewChannelManager, viewManager, name, sourceChannelManager, sourceImage);

    if (layer)
    {
        layerProcessor.addLayer(layer);
        setLastView();

        controlPanel->updateLayerGrid();
        
    }
}        

void deProject::addRandomLayer()
{
    int view = layerStack.getSize() - 1;

    if (view > 0)
    {
        if (rand() % view > 3)
        {
            layerProcessor.removeTopLayer();
            controlPanel->updateLayerGrid();
            return;
        }
    }

    if (rand() % 10 > 2)
    {
        std::vector<std::string> actions;
        getSupportedActions(actions);

        int n = actions.size();
        int r = rand() % n;

        std::string a = actions[r];
        addActionLayer(a);
    }
    else
    {
        deLayer* layer = layerStack.getLayer(view);
        if (!layer)
        {
            return;
        }

        deColorSpace currentColorSpace = layer->getColorSpace();

        bool valid = false;
        deColorSpace c = deColorSpaceInvalid;

        while (!valid)
        {
            std::vector<deColorSpace> colorSpaces;
            getSupportedColorSpaces(colorSpaces);

            int n = colorSpaces.size();
            int r = rand() % n;

            c = colorSpaces[r];

            valid = checkConversion(currentColorSpace, c);

            if (c == currentColorSpace)
            {
                valid = false;
            }
        }            

        addConversionLayer(c);
    }

    int n = layerStack.getSize();
    int i;
    for (i = 0 ; i < n ; i++)
    {
        deLayer* layer = layerStack.getLayer(i);
        if (layer->randomize())
        {
            layerProcessor.markUpdateAllChannels(i);
        }            
    }
}


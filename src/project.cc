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
#include "image_panel.h"
#include "str.h"
#include "histogram_panel.h"
#include "control_panel.h"
#include "view_mode_panel.h"
#include <sstream>
#include "layer_factory.h"
#include "convert_image.h"
#include "fractal.h"
#include "xml.h"

deProject::deProject()
:viewModePanel(NULL),
 controlPanel(NULL),
 viewManager(*this),
 samplerManager(*this)
{
    sourceR = -1;
    sourceG = -1;
    sourceB = -1;
    imageFileName = "";
    sourceImageFileName = "";
    imagePanel = NULL;
    resetLayerStack();
    histogramPanel = NULL;

    receiveKeys = true;
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
    layerStack.clear();
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
        histogramPanel->setChannel(0);
        histogramPanel->generate();
        histogramPanel->paint();
    }
    if (key == WXK_F2)
    {
        histogramPanel->setChannel(1);
        histogramPanel->generate();
        histogramPanel->paint();
    }
    if (key == WXK_F3)
    {
        histogramPanel->setChannel(2);
        histogramPanel->generate();
        histogramPanel->paint();
    }
    if (key == WXK_F4)
    {
        histogramPanel->setChannel(3);
        histogramPanel->generate();
        histogramPanel->paint();
    }

    layerStack.onKey(key);
    if (controlPanel)
    {
        controlPanel->onKey(key);
    }        
}

void deProject::init(const std::string& fileName)
{
    bool tiff = checkTIFF(fileName);
    bool jpeg = checkJPEG(fileName);

    if ((!tiff) && (!jpeg))
    {
        return;
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
    sourceR = sourceChannelManager.allocateNewChannel();
    sourceG = sourceChannelManager.allocateNewChannel();
    sourceB = sourceChannelManager.allocateNewChannel();

    deChannel* channelR = sourceChannelManager.getChannel(sourceR);
    deChannel* channelG = sourceChannelManager.getChannel(sourceG);
    deChannel* channelB = sourceChannelManager.getChannel(sourceB);

    if (tiff)
    {
        bool icc;
        loadTIFF(fileName, *channelR, *channelG, *channelB, icc);
        if (icc)
        {
            wxMessageBox( _T("Warning! this TIFF file contains ICC profile which is ignored by delaboratory\n\ndelaboratory expects sRGB - colors may be not accurate\n\nThis problem happens (for instance) when tiff is created by dcraw\nyou can fix it by calling tifficc command, by default it converts tiff to sRGB"), _T("ICC profile ignored"), wxOK | wxICON_INFORMATION, NULL );
        }
    }

    if (jpeg)
    {
        loadJPEG(fileName, *channelR, *channelG, *channelB);
    }

    imageFileName = removePathAndExtension(fileName);
    sourceImageFileName = fileName;

    setSource();
}


void deProject::setTestImage()
{
    if (sourceR > 0)
    {
        sourceChannelManager.freeChannel(sourceR);
    }
    if (sourceG > 0)
    {
        sourceChannelManager.freeChannel(sourceG);
    }
    if (sourceB > 0)
    {
        sourceChannelManager.freeChannel(sourceB);
    }

    deSize size(800,600);

    sourceChannelManager.setChannelSize(size);
    sourceR = sourceChannelManager.allocateNewChannel();
    sourceG = sourceChannelManager.allocateNewChannel();
    sourceB = sourceChannelManager.allocateNewChannel();

    deChannel* channelR = sourceChannelManager.getChannel(sourceR);
    deChannel* channelG = sourceChannelManager.getChannel(sourceG);
    deChannel* channelB = sourceChannelManager.getChannel(sourceB);

    generateFractal(channelR->getPixels(), channelG->getPixels(), channelB->getPixels(), size);

    imageFileName = "delaboratory_test_image";

    setSource();
    layerStack.updateImages();
    repaintImage();
}

void deProject::setSource()
{
    deChannel* channelR = sourceChannelManager.getChannel(sourceR);
    deChannel* channelG = sourceChannelManager.getChannel(sourceG);
    deChannel* channelB = sourceChannelManager.getChannel(sourceB);

    deSourceImageLayer* l = dynamic_cast<deSourceImageLayer*>(layerStack.getLayer(0));

    l->setSource(sourceR, sourceG, sourceB, &sourceChannelManager);

    previewChannelManager.destroyAllChannels();

}

void deProject::resetLayerStack()
{
    layerStack.clear();

    deLayer* layer = createLayer("source_image", -1, deColorSpaceRGB, layerStack, previewChannelManager, viewManager, "source image");

    if (layer)
    {
        addLayer(layer);
    }        

    setSource();
    layerStack.updateImages();
}

void deProject::addLayer(deLayer* layer)
{
    layerStack.addLayer(layer);
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

void deProject::setPreviewSize(const deSize& size)
{
    deSize oldSize = previewChannelManager.getChannelSize();
    if (oldSize == size)
    {
        return;
    }

    previewChannelManager.setChannelSize(size);

    layerStack.updateImages();

    if (histogramPanel)
    {
        histogramPanel->generate();
        histogramPanel->paint();
    }
}

void deProject::onChangeView(int a, int b)
{
    layerStack.updateImages(a + 1, b);
    if (controlPanel)
    {
        controlPanel->onChangeView();
    }
    if (viewModePanel)
    {
        viewModePanel->updateNames();
    }
    updateSamplers();
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

void deProject::setImagePanel(deImagePanel* _imagePanel)
{
    imagePanel = _imagePanel;
}

void deProject::repaintImage()
{
    if (imagePanel)
    {
        imagePanel->paint();
    }        
    if (histogramPanel)
    {
        histogramPanel->generate();
        histogramPanel->paint();
    }
    updateSamplers();
}

void deProject::exportTIFF(const std::string& app)
{
    if (imageFileName == "")
    {
        return;
    }

    std::string path = "";

    if (app.size() > 0)
    {
        path = "/tmp/";
    }

    std::string fileName = path + imageFileName + ".tiff";

    deSize originalSize = previewChannelManager.getChannelSize();

    int view = viewManager.getView();

    previewChannelManager.setChannelSize(sourceChannelManager.getChannelSize());
    layerStack.updateImagesSmart(previewChannelManager, view);

    deLayer* layer = layerStack.getLayer(view);
    const deImage& image = layer->getImage();

    if (image.getColorSpace() == deColorSpaceRGB)
    {
        deChannel* r = previewChannelManager.getChannel(image.getChannelIndex(0));
        deChannel* g = previewChannelManager.getChannel(image.getChannelIndex(1));
        deChannel* b = previewChannelManager.getChannel(image.getChannelIndex(2));
        saveTIFF(fileName, *r, *g, *b, previewChannelManager.getChannelSize());
    }
    else
    {
        deImage finalImage(deColorSpaceRGB, previewChannelManager);
        finalImage.enableAllChannels();
        convertImage(image, finalImage, previewChannelManager);
        deChannel* r = previewChannelManager.getChannel(finalImage.getChannelIndex(0));
        deChannel* g = previewChannelManager.getChannel(finalImage.getChannelIndex(1));
        deChannel* b = previewChannelManager.getChannel(finalImage.getChannelIndex(2));
        saveTIFF(fileName, *r, *g, *b, previewChannelManager.getChannelSize());

    }


    previewChannelManager.setChannelSize(originalSize);

    if (app.size() > 0)
    {
        const char* c = fileName.c_str();
        wxString s(c, wxConvUTF8);
        const wxString command = wxString::FromAscii(app.c_str()) + _T(" ") + s;
        wxExecute(command);
    }

    layerStack.updateImages();

}

void deProject::deleteLayer()
{
    if (layerStack.getSize() < 2)
    {
        return;
    }
    layerStack.removeTopLayer();
    int view = viewManager.getView();
    if (view >= layerStack.getSize())
    {
        viewManager.setView( layerStack.getSize() - 1 );
    }
}

void deProject::setLastView()
{
    int a = viewManager.getView();
    int n = layerStack.getSize();
    n--;
    viewManager.setView(n);
    onChangeView(a, n);
}

void deProject::setHistogramPanel(deHistogramPanel* _histogramPanel)
{
    histogramPanel = _histogramPanel;
}

void deProject::setViewModePanel(deViewModePanel* _viewModePanel)
{
    viewModePanel = _viewModePanel;
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

void deProject::updateSamplers()
{
    if (controlPanel)
    {
        controlPanel->updateSamplerManagerFrame();
    }
}

bool deProject::samplersVisible() const
{
    if (controlPanel)
    {
        return controlPanel->samplersVisible();
    }
    return false;
}

void deProject::save(const std::string& fileName)
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
    }

    xmlSaveFormatFile (f.c_str(), doc, 1); 
}

void deProject::loadLayer(xmlNodePtr root)
{
    xmlNodePtr child = root->xmlChildrenNode;

    int index = -1;
    std::string type = "";
    std::string name = "";
    int source = -1;
    deColorSpace colorSpace = deColorSpaceInvalid;

    while (child)
    {
        if ((!xmlStrcmp(child->name, BAD_CAST("index")))) 
        {
            std::istringstream iss(getContent(child));
            iss >> index;
        }

        if ((!xmlStrcmp(child->name, BAD_CAST("source")))) 
        {
            std::istringstream iss(getContent(child));
            iss >> source;
        }

        if ((!xmlStrcmp(child->name, BAD_CAST("color_space")))) 
        {
            colorSpace = colorSpaceFromString(getContent(child));
        }

        if ((!xmlStrcmp(child->name, BAD_CAST("type")))) 
        {
            type = getContent(child);
        }
        
        if ((!xmlStrcmp(child->name, BAD_CAST("name")))) 
        {
            name = getContent(child);
        }

        child = child->next;
    }
        
    deLayer* layer = createLayer(type, source, colorSpace, layerStack, previewChannelManager, viewManager, name);

    if (layer)
    {
        addLayer(layer);
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

    setSource();
    layerStack.updateImages();
}

void deProject::open(const std::string& fileName)
{
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

    while (child)
    {
        if ((!xmlStrcmp(child->name, BAD_CAST("layer_stack")))) 
        {
            loadLayers(child);
        }

        child = child->next;
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

void deProject::showSamplers()
{
    if (controlPanel)
    {
        controlPanel->showSamplers();
    }
}

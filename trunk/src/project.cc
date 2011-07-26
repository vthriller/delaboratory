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
#include "source_image_layer.h"
#include "exception.h"
#include "image_panel.h"
#include "final_image.h"
#include "sampler_list_panel.h"
#include "info_bar_panel.h"
#include "source_image.h"
#include "logger.h"
#include "str.h"

#include <libxml/parser.h>

#include <wx/progdlg.h>

deProject::deProject()
:sourceImageSize(0,0), previewSize(0,0)
{
#ifdef DE_LOGGER
    logger.setFile("delaboratory.log");
    logMessage("starting project...");
#endif    
    sourceFileName = "";
    view = -1;
    previewStack.setProject(this);
    samplerList.setProject(this);
    resetLayerStack();
}

deProject::~deProject()
{
}

void deProject::resetLayerStack()
{
    layerStack.clear();
    previewStack.clear();
    deSourceImageLayer* layer = new deSourceImageLayer(layerStack, 0, "source image");
    layer->setSourceImage(&sourceImage);
    addLayer(layer);
}

void deProject::setSourceImageSize(const deSize& size)
{
    sourceImageSize = size;
}

const deSize& deProject::getSourceImageSize() const
{
    return sourceImageSize;
}

void deProject::setPreviewSize(const deSize& size)
{
    logMessage("set preview size " + size.str() + "...");

    previewSize = size;

    deLayer* layer = layerStack.getLayer(0);
    deSourceImageLayer* sourceImageLayer = dynamic_cast<deSourceImageLayer*>(layer);
    if (!sourceImageLayer)
    {
        throw deException("no source image layer in project");
    }

    sourceImageLayer->setPreviewSize(size);

    previewStack.updatePreviews(0);
}

void deProject::addLayer(deLayer* layer)
{
    logMessage("add new layer...");
    try
    {
        layerStack.addLayer(layer);
        previewStack.addPreview();
        setView (layerStack.getSize() - 1);
        previewStack.updatePreviews(previewStack.getSize() - 1);
        gui.updateLayerListPanel();
    }
    catch (deException& e)
    {
    }
    logMessage("add new layer done");
}

void deProject::removeTopLayer()
{
    logMessage("remove top layer...");
    layerStack.removeTopLayer();
    if (view > layerStack.getSize() - 1)
    {
        setView (layerStack.getSize() - 1);
    }
    gui.updateLayerListPanel();
}

deLayer* deProject::getVisibleLayer()
{
    if (view < 0)
    {
        return NULL;
    }
    return layerStack.getLayer(view);
}

dePreview* deProject::getVisiblePreview()
{
    if (view < 0)
    {
        return NULL;
    }
    return previewStack.getPreview(view);
}

int deProject::getVisibleLayerID() const
{
    if (view < 0)
    {
        return -1;
    }
    return view;
}

const deLayerStack& deProject::getLayerStack() const
{
    return layerStack;
}

deLayerStack& deProject::getLayerStack() 
{
    return layerStack;
}

const dePreviewStack& deProject::getPreviewStack() const
{
    return previewStack;
}

dePreviewStack& deProject::getPreviewStack() 
{
    return previewStack;
}

deFinalImage* deProject::generateFinalImage()
{
    logMessage("generate final image...");

    wxProgressDialog* progressDialog = new wxProgressDialog(_T("generate final image"), _T("generate final image"), 100, NULL, wxPD_CAN_ABORT);

    const dePreview* preview = getVisiblePreview();
    if (!preview)
    {
        return NULL;
    }
    //deSize previousSize = preview->getSize();
    deSize previousSize = previewSize;

    setPreviewSize(sourceImageSize);

    const dePreview* finalPreview = previewStack.generateFinalPreview(progressDialog, view);
    deFinalImage* finalImage = NULL;

    if (finalPreview)
    {
        finalImage = new deFinalImage(*finalPreview);
    }        

    setPreviewSize(previousSize);
    previewStack.updatePreviews(0);
    delete progressDialog;

    return finalImage;
}

void deProject::setView(int v)
{
    logMessage("set view...");

    int old = view;
    view = v;
    previewStack.updatePreviews(old + 1);

    deLayer* layer = getVisibleLayer();
    if (!layer)
    {
        return;
    }

    const deColorSpace colorSpace = layer->getColorSpace();
    gui.updateAfterSetView(colorSpace);

    logMessage("set view done");
}

deSamplerList& deProject::getSamplerList() 
{
    return samplerList;
}

void deProject::init(const std::string& fileName)
{
    const std::string ext = getExtension(fileName);
    if (ext == "delab")
    {
        open(fileName, true);
    }
    else
    {
        loadSourceImage(fileName);
    }
}

void deProject::loadSourceImage(const std::string& fileName)
{
    logMessage("loading source image " + fileName + "...");
    sourceImageFileName = fileName;

    size_t posDot = fileName.rfind(".");
    size_t posSlash = fileName.rfind("/");
    int posStart;
    if (posSlash > fileName.size())
    {
        posStart= 0;
    }
    else
    {
        posStart = posSlash + 1;
    }
    sourceFileName = fileName.substr(posStart, posDot - posStart );

    sourceImage.load(fileName);
    setSourceImageSize(sourceImage.getRawSize());
    gui.updateCenterPanel();
    previewStack.updatePreviews(0);
    gui.refreshView();
}

deGUI& deProject::getGUI()
{
    return gui;
}

const std::string deProject::getSourceFileName() const
{
    return sourceFileName;
}

void deProject::logMessage(const std::string& message)
{
    // FIXME it should be implemented differently, there should be LOG macro instead function
#ifdef DE_LOGGER
    logger.log(message);
#endif    
}

void deProject::open(const std::string& fileName, bool image)
{


    layerStack.clear();
    previewStack.clear();

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
        if ((!xmlStrcmp(child->name, xmlCharStrdup("layer_stack")))) 
        {
            layerStack.load(child, layerFactory);
        }

        if ((!xmlStrcmp(child->name, xmlCharStrdup("source_image_file_name")))) 
        {
            xmlChar* s = xmlNodeGetContent(child);            
            sourceImageFileName = (char*)(s);
            xmlFree(s);
        }

        child = child->next;
    }

    deSourceImageLayer* layer = dynamic_cast<deSourceImageLayer*>(layerStack.getLayer(0));
    if (layer)
    {
        layer->setSourceImage(&sourceImage);
    }

    int n = layerStack.getSize();
    int i;
    for (i = 0; i < n; i++)
    {
        previewStack.addPreview();
    }

    setView (layerStack.getSize() - 1);

    if (image)
    {
        loadSourceImage(sourceImageFileName);
    }        

}

void deProject::save(const std::string& fileName)
{
    std::string f = fileName;

    size_t pos = f.rfind(".delab");
    if (pos != f.size() - 6) 
    {
        f += ".delab";
    }

    xmlDocPtr doc = xmlNewDoc(xmlCharStrdup("1.0"));

    xmlNodePtr root = xmlNewNode(NULL, xmlCharStrdup("project"));
    xmlDocSetRootElement(doc, root);

    {
        xmlNodePtr node = xmlNewChild(root, NULL, xmlCharStrdup("source_image_file_name"), NULL);
        xmlNodeSetContent(node, xmlCharStrdup(sourceImageFileName.c_str()));
    }

    {
        xmlNodePtr child = xmlNewChild(root, NULL, xmlCharStrdup("layer_stack"), NULL);
        layerStack.save(child);
    }

    xmlSaveFormatFile (f.c_str(), doc, 1); 
}

const deSize& deProject::getPreviewSize() const
{
    return previewSize;
}

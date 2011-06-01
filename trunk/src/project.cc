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

deProject::deProject()
:sourceImageSize(0,0)
{
    sourceFileName = "";
    view = -1;
    previewStack.setProject(this);
    samplerList.setProject(this);
    addLayer(new deSourceImageLayer(sourceImage, "source image"));
}

deProject::~deProject()
{
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
}

void deProject::removeTopLayer()
{
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
    const dePreview* preview = getVisiblePreview();
    if (!preview)
    {
        return NULL;
    }
    deSize previousSize = preview->getSize();

    setPreviewSize(sourceImageSize);
    previewStack.updatePreviews(0);

    const dePreview* finalPreview = getVisiblePreview();
    deFinalImage* finalImage = new deFinalImage(*finalPreview);

    setPreviewSize(previousSize);
    previewStack.updatePreviews(0);

    return finalImage;
}

void deProject::setView(int v)
{
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
}

deSamplerList& deProject::getSamplerList() 
{
    return samplerList;
}

void deProject::loadSourceImage(const std::string& fileName)
{
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
    setSourceImageSize(sourceImage.getSize());
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

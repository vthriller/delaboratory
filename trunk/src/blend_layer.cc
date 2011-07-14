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

#include "blend_layer.h"
#include "blend.h"
#include "layer_stack.h"
#include "exception.h"
#include <iostream>
#include "preview_stack.h"
#include "preview.h"
#include "project.h"
#include <sstream>

deBlendLayer::deBlendLayer(deLayerStack& _stack, int _index, const std::string& _name)
:deLayer(_stack, _index, _name), alpha(*this, "alpha", 0.0, 1.0), blendMode(*this), channels(*this), overlayLayer(*this, "overlay"), blendMask(*this, "mask")
{
    alpha.setValue(0.5);
    blendMode.setBlendMode(deBlendNormal);
    overlayLayer.setIndex(-1);
}

deBlendLayer::~deBlendLayer()
{
}


void deBlendLayer::changeAlpha(deValue _alpha)
{
    alpha.setValue(_alpha);
}

dePreview* deBlendLayer::createPreview(dePreviewStack& previewStack)
{
    const dePreview* sourcePreview = previewStack.getPreview(sourceLayer.getIndex());

    if (!sourcePreview)
    {
        return NULL;
    }

    const deSize& sourceSize = sourcePreview->getSize();

    dePreview* preview = new dePreview(colorSpace.getColorSpace(), sourceSize);

    const dePreview* overlayPreview = previewStack.getPreview(overlayLayer.getIndex());
    if (!overlayPreview)
    {
        throw deException("no overlayPreview in blend layer");
        return NULL;
    }

/*
    int oc;
    if (singleOverlayChannel)
    {
        oc = overlayChannel;
    }
    else
    {
        oc = -1;
    }*/
    /*
    int dc;
    if (singleDestinationChannel)
    {
        dc = destinationChannel;
    }
    else
    {
        dc = -1;
    }*/

    blend(*sourcePreview, *overlayPreview, alpha.getValue(), *preview, -1, channels.getChannels(), blendMode.getBlendMode());

    return preview;
}

/*
void deBlendLayer::setOverlayChannel(int _channel)
{
    overlayChannel = _channel;
}

int deBlendLayer::getOverlayChannel() const
{
    return overlayChannel;
}

void deBlendLayer::setSingleOverlayChannel(bool _singleChannel)
{
    singleOverlayChannel = _singleChannel;
}

bool deBlendLayer::isSingleOverlayChannel() const
{
    return singleOverlayChannel;
}
*/

deBlendMode deBlendLayer::getBlendMode() const
{
    return blendMode.getBlendMode();
}

void deBlendLayer::setBlendMode(deBlendMode mode)
{
    blendMode.setBlendMode(mode);
}

void deBlendLayer::saveSpecific(xmlNodePtr node)
{
    alpha.save(node, "alpha");
    blendMode.save(node, "blend_mode");
    channels.save(node, "channels");
    overlayLayer.save(node, "overlay_layer");
}

void deBlendLayer::loadSpecific(xmlNodePtr node)
{
    xmlNodePtr child = node->xmlChildrenNode;

    std::string overlayLayerString = "";

    while (child)
    {
        if ((!xmlStrcmp(child->name, xmlCharStrdup("alpha")))) 
        {
            alpha.load(child);
        }

        if ((!xmlStrcmp(child->name, xmlCharStrdup("blend_mode")))) 
        {
            blendMode.load(child);
        }

        if ((!xmlStrcmp(child->name, xmlCharStrdup("channels")))) 
        {
            channels.load(child);
        }

        if ((!xmlStrcmp(child->name, xmlCharStrdup("overlay_layer")))) 
        {
            xmlChar* s = xmlNodeGetContent(child);            
            overlayLayerString = (char*)(s);
            xmlFree(s);
        }

        child = child->next;
    }

    {
        std::istringstream iss(overlayLayerString);
        int s;
        iss >> s;

        overlayLayer.setIndex(s);
    }        
}

void deBlendLayer::onChangeSourceLayer()
{
    if (overlayLayer.getIndex() < 0)
    {
        overlayLayer.setIndex(sourceLayer.getIndex());
    }
}

void deBlendLayer::onChangeColorSpace()
{
    channels.fill();
}


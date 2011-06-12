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
#include "blend_frame.h"
#include "preview_stack.h"
#include "preview.h"
#include "project.h"
#include "layer_dialog.h"

deBlendLayer::deBlendLayer(deLayerStack& _stack, const std::string& _name)
:deLayer(_stack, _name)
{
    alpha = 0.5;
    overlayChannel = -1;
    destinationChannel = -1;
    singleOverlayChannel = false;
    singleDestinationChannel = false;
    blendMode = deBlendNormal;
}

deBlendLayer::~deBlendLayer()
{
}


void deBlendLayer::changeAlpha(deValue _alpha)
{
    alpha = _alpha;
}

deActionFrame* deBlendLayer::createActionFrame(wxWindow* parent, int layerNumber, deProject* project)
{
    return new deBlendFrame(parent, *this, project->getPreviewStack(), layerNumber);
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

    int oc;
    if (singleOverlayChannel)
    {
        oc = overlayChannel;
    }
    else
    {
        oc = -1;
    }
    int dc;
    if (singleDestinationChannel)
    {
        dc = destinationChannel;
    }
    else
    {
        dc = -1;
    }

    blend(*sourcePreview, *overlayPreview, alpha, *preview, oc, dc, blendMode);

    return preview;
}

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

void deBlendLayer::setDestinationChannel(int _channel)
{
    destinationChannel = _channel;
}

int deBlendLayer::getDestinationChannel() const
{
    return destinationChannel;
}

void deBlendLayer::setSingleDestinationChannel(bool _singleChannel)
{
    singleDestinationChannel = _singleChannel;
}

bool deBlendLayer::isSingleDestinationChannel() const
{
    return singleDestinationChannel;
}

deBlendMode deBlendLayer::getBlendMode() const
{
    return blendMode;
}

void deBlendLayer::setBlendMode(deBlendMode mode)
{
    blendMode = mode;
}

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

#include "apply_color_layer.h"
#include "apply_channels.h"
#include "preview_stack.h"
#include "blend.h"
#include "preview.h"
#include "converter.h"
#include "channel.h"
#include <iostream>


deApplyColorLayer::deApplyColorLayer(deLayerStack& _stack, int _index, const std::string& _name)
:deLayer(_stack, _index, _name), overlayLayer(*this, "overlay")
{
    overlayLayer.setLayerIndex(-1);
}

deApplyColorLayer::~deApplyColorLayer()
{
}

bool deApplyColorLayer::updatePreview(const dePreview* sourcePreview, const dePreview* overlayPreview, dePreview* preview)
{
    bool a[4];
    a[0] = false;
    a[1] = true;
    a[2] = true;
    applyChannels(sourcePreview, overlayPreview, preview, deColorSpaceLAB, a);

}

void deApplyColorLayer::saveSpecific(xmlNodePtr node)
{
}

void deApplyColorLayer::loadSpecific(xmlNodePtr node)
{
}

void deApplyColorLayer::updatePreview(dePreviewStack& previewStack)
{
    const dePreview* sourcePreview = previewStack.getPreview(getSourceLayerID());
    const dePreview* overlayPreview = previewStack.getPreview(overlayLayer.getLayerIndex());
    dePreview* preview = previewStack.getPreview(index);

    if ((sourcePreview) && (preview) && (overlayPreview))
    {
        updatePreview(sourcePreview, overlayPreview, preview);
    }
}

void deApplyColorLayer::onChangeSourceLayer()
{
    if (overlayLayer.getLayerIndex() < 0)
    {
        overlayLayer.setLayerIndex(sourceLayer.getLayerIndex());
    }
}

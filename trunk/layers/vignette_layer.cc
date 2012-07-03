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

#include "vignette_layer.h"
#include "vignette.h"
#include "preset.h"
#include "view_manager.h"
#include "property_numeric.h"

deVignetteLayer::deVignetteLayer(deColorSpace _colorSpace, deChannelManager& _channelManager, int _sourceLayer, deLayerStack& _layerStack, deViewManager& _viewManager)
:deLayerWithBlending(_colorSpace, _channelManager, _sourceLayer, _layerStack), viewManager(_viewManager)
{
    if (colorSpace == deColorSpaceCMYK)
    {
        setBlendMode(deBlendScreen);
    }
    else
    {
        setBlendMode(deBlendMultiply);
    }

    dePreset* reset = createPreset("reset");

    createPropertyNumeric("radius", 0.3, 3.0);
    reset->addNumericValue("radius", 1.5);

    createPropertyNumeric("aspect", -0.9, 0.9);
    reset->addNumericValue("aspect", 0.0);

    createPropertyNumeric("center_x", -1, 1);
    reset->addNumericValue("center_x", 0.0);
    createPropertyNumeric("center_y", -1, 1);
    reset->addNumericValue("center_y", 0.0);

    applyPreset("reset");
    disableNotLuminance();
}

deVignetteLayer::~deVignetteLayer()
{
}

bool deVignetteLayer::isChannelNeutral(int channel)
{   
    return false;
}

bool deVignetteLayer::updateMainImageSingleChannel(int channel)
{
    if (!isChannelEnabled(channel))
    {
        copySourceChannel(channel);
        return true;
    }

    deSize size = mainLayerImage.getChannelSize();

    deValue* destination = mainLayerImage.startWrite(channel);

    deValue light = 1.0;
    deValue darkness = 0.0;
    deValue spot = 0.5;

    deValue x1;
    deValue y1;
    deValue x2;
    deValue y2;

    viewManager.getZoom(x1, y1, x2, y2);

    deValue r = getNumericValue("radius");
    deValue a = getNumericValue("aspect");

    deValue rx = r;
    if (a < 0.0)
    {
        rx *= (a + 1);
    }
    deValue ry = r;
    if (a > 0.0)
    {
        ry *= (1 - a);
    }
    deValue cx = getNumericValue("center_x");
    deValue cy = getNumericValue("center_y");

    deEllipse ellipse = calcEllipse(rx, ry, cx, cy, x1, y1, x2, y2);

    vignetteChannel(destination, size, ellipse, light, darkness, spot);

    mainLayerImage.finishWrite(channel);

    return true;
}

bool deVignetteLayer::onImageClick(deValue x, deValue y)
{
    dePropertyNumeric* cx = getPropertyNumeric("center_x");
    cx->set(2 * x - 1);
    dePropertyNumeric* cy = getPropertyNumeric("center_y");
    cy->set(2 * y - 1);

    return true;
}

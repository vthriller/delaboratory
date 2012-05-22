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

#include "gradient_layer.h"
#include "gradient.h"
#include "preset.h"
#include "view_manager.h"
#include "property_numeric.h"

deGradientLayer::deGradientLayer(deColorSpace _colorSpace, deChannelManager& _channelManager, int _sourceLayer, deLayerStack& _layerStack, deViewManager& _viewManager)
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

    createPropertyNumeric("length", -3.0, 3.0);
    reset->addNumericValue("length", 1.0);

    createPropertyNumeric("direction", 0, 360);
    reset->addNumericValue("direction", 0.0);

    createPropertyNumeric("center_x", -1, 1);
    reset->addNumericValue("center_x", 0.0);
    createPropertyNumeric("center_y", -1, 1);
    reset->addNumericValue("center_y", 0.0);

    applyPreset("reset");
    disableNotLuminance();
}

deGradientLayer::~deGradientLayer()
{
}

bool deGradientLayer::isChannelNeutral(int channel)
{   
    return false;
}

bool deGradientLayer::updateMainImageSingleChannel(int channel)
{
    if ((isChannelNeutral(channel)) || (!isChannelEnabled(channel)))
    {
        int s = getSourceImage().getChannelIndex(channel);
        mainLayerImage.disableChannel(channel, s);
        return true;
    }

    deSize size = mainLayerImage.getChannelSize();

    mainLayerImage.enableChannel(channel);
    deValue* destination = mainLayerImage.startWrite(channel);

    deValue x1;
    deValue y1;
    deValue x2;
    deValue y2;

    viewManager.getZoom(x1, y1, x2, y2);

    deValue r = getNumericValue("length");
    deValue a = getNumericValue("direction");

    deValue cx = getNumericValue("center_x");
    deValue cy = getNumericValue("center_y");

    gradientChannel(destination, size, cx, cy, r, a);

    mainLayerImage.finishWrite(channel);

    return true;
}

bool deGradientLayer::onImageClick(deValue x, deValue y)
{
    dePropertyNumeric* cx = getPropertyNumeric("center_x");
    cx->set(2 * x - 1);
    dePropertyNumeric* cy = getPropertyNumeric("center_y");
    cy->set(2 * y - 1);

    return true;
}

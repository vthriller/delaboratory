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

#include "exposure_layer.h"
#include "preset.h"
#include "view_manager.h"
#include "color_space_utils.h"
#include "copy_channel.h"
#include "logger.h"
#include "str.h"
#include "curve.h"

deExposureLayer::deExposureLayer(deColorSpace _colorSpace, deChannelManager& _channelManager, int _sourceLayer, deLayerStack& _layerStack)
:deLayerWithBlending(_colorSpace, _channelManager, _sourceLayer, _layerStack)
{
    dePreset* reset = createPreset("reset");

    createPropertyNumeric("black", 0, 1);
    reset->addNumericValue("black", 0.0);

    createPropertyNumeric("shadows", -1, 1);
    reset->addNumericValue("shadows", 0.0);

    createPropertyNumeric("brightness", -1, 1);
    reset->addNumericValue("brightness", 0.0);

    createPropertyNumeric("lights", -1, 1);
    reset->addNumericValue("lights", 0.0);

    createPropertyNumeric("highlights", 0, 1);
    reset->addNumericValue("highlights", 1.0);

    applyPreset("reset");
    disableNotLuminance();
}

deExposureLayer::~deExposureLayer()
{
}

bool deExposureLayer::updateMainImageSingleChannel(int channel)
{
    if ((isChannelNeutral(channel)) || (!isChannelEnabled(channel)))
    {
        int s = getSourceImage().getChannelIndex(channel);
        mainLayerImage.disableChannel(channel, s);
        return true;
    }

    deValue black = getNumericValue("black");
    deValue shadows = getNumericValue("shadows");
    deValue brightness = getNumericValue("brightness");
    deValue lights = getNumericValue("lights");
    deValue highlights = getNumericValue("highlights");

    deValue x0 = black;
    deValue x4 = highlights;
    deValue x2 = (x0 + x4) / 2.0;
    deValue x1 = (x0 + x2) / 2.0;
    deValue x3 = (x2 + x4) / 2.0;

    deValue y0 = 0.0;
    deValue y1 = 0.25 + shadows/4.0 + brightness;
    deValue y2 = 0.5 + brightness;
    deValue y3 = 0.75 + lights/4.0 + brightness;
    deValue y4 = 1.0;

    const deValue* source = getSourceImage().startRead(channel);
    mainLayerImage.enableChannel(channel);
    deValue* target = mainLayerImage.startWrite(channel);
    int n = mainLayerImage.getChannelSize().getN();

    deBaseCurve curve;

    curve.addPoint(0, 0);
    if (colorSpace == deColorSpaceCMYK)
    {
        curve.addPoint(1 - x4, 1 - y4);
        curve.addPoint(1 - x3, 1 - y3);
        curve.addPoint(1 - x2, 1 - y2);
        curve.addPoint(1 - x1, 1 - y1);
        curve.addPoint(1 - x0, 1 - y0);
    }
    else
    {
        curve.addPoint(x0, y0);
        curve.addPoint(x1, y1);
        curve.addPoint(x2, y2);
        curve.addPoint(x3, y3);
        curve.addPoint(x4, y4);
    }
    curve.addPoint(1, 1);

    curve.build();

    curve.process(source, target, n);

    getSourceImage().finishRead(channel);
    mainLayerImage.finishWrite(channel);

    return true;
}

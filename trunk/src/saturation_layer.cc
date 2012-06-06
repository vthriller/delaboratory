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

#include "saturation_layer.h"
#include "preset.h"
#include "view_manager.h"
#include "color_space_utils.h"
#include "copy_channel.h"
#include "logger.h"
#include "str.h"
#include "curve.h"

deSaturationLayer::deSaturationLayer(deColorSpace _colorSpace, deChannelManager& _channelManager, int _sourceLayer, deLayerStack& _layerStack)
:deLayerWithBlending(_colorSpace, _channelManager, _sourceLayer, _layerStack)
{
    dePreset* reset = createPreset("reset");

    createPropertyNumeric("saturation", -1, 1);
    reset->addNumericValue("saturation", 0.0);

    createPropertyNumeric("green / magenta", -1, 1);
    reset->addNumericValue("green / magenta", 0.0);

    createPropertyNumeric("blue / yellow", -1, 1);
    reset->addNumericValue("blue / yellow", 0.0);

    applyPreset("reset");
    if (colorSpace != deColorSpaceLAB)
    {
        disableAll();
    }
    else
    {
        disableChannel(0);
    }
}

deSaturationLayer::~deSaturationLayer()
{
}

bool deSaturationLayer::updateMainImageSingleChannel(int channel)
{
    bool e = true;

    if (colorSpace != deColorSpaceLAB)
    {
        e = false;
    }

    if (channel == 0)
    {
        e = false;
    }

    if ((isChannelNeutral(channel)) || (!isChannelEnabled(channel)))
    {
        e = false;
    }

    deValue s = getNumericValue("saturation");
    deValue s2 = 0.0;
    if (channel == 1)
    {
        s2 = getNumericValue("green / magenta");
    }
    if (channel == 2)
    {
        s2 = getNumericValue("blue / yellow");
    }

    s = s + s2;

    if (s == 0.0)
    {
        e = false;
    }

/*
    if (!e)
    {
        int s = getSourceImage().getChannelIndex(channel);
        mainLayerImage.disableChannel(channel, s);
        return true;
    }
    */

    const deValue* source = getSourceImage().startRead(channel);
//    mainLayerImage.enableChannel(channel);
    deValue* target = mainLayerImage.startWrite(channel);
    int n = mainLayerImage.getChannelSize().getN();

    deBaseCurve curve;

    if (s > 0)
    {
        if (s > 1)
        {
            s = 1.0;
        }
        s = 0.99 * s;
        curve.addPoint(0, 0);
        curve.addPoint(s / 2.0, 0);
        curve.addPoint(0.5, 0.5);
        curve.addPoint(1 - s/2.0, 1);
        curve.addPoint(1, 1);
    }
    else
    {
        if (s < -1)
        {
            s = -1;
        }
        s = -s;
        curve.addPoint(0, s / 2.0);
        curve.addPoint(0.5, 0.5);
        curve.addPoint(1, 1 - s / 2.0);
    }

    curve.build();

    curve.process(source, target, n);

    getSourceImage().finishRead(channel);
    mainLayerImage.finishWrite(channel);

    return true;
}

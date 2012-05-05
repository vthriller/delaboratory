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

#include "fill_layer.h"
#include "fill_channel.h"
#include "preset.h"
#include "color_space_utils.h"

deFillLayer::deFillLayer(deColorSpace _colorSpace, deChannelManager& _channelManager, int _sourceLayer, deLayerStack& _layerStack)
:deLayerWithBlending(_colorSpace, _channelManager, _sourceLayer, _layerStack)
{
    dePreset* reset = createPreset("reset");
    int n = getColorSpaceSize(colorSpace);

    int i;
    for (i = 0; i < n; i++)
    {
        std::string n = "fill " + getChannelName(colorSpace, i);
        createPropertyNumeric(n, 0, 1);
        reset->addNumericValue(n, 0.5);
    }

    applyPreset("reset");
    setOpacity(0.5);
}

deFillLayer::~deFillLayer()
{
}

bool deFillLayer::updateMainImageSingleChannel(int channel)
{
    if ((isChannelNeutral(channel)) || (!isChannelEnabled(channel)))
    {
        int s = getSourceImage().getChannelIndex(channel);
        mainLayerImage.disableChannel(channel, s);
        return true;
    }

    mainLayerImage.enableChannel(channel);

    std::string p = "fill " + getChannelName(colorSpace, channel);

    deValue value = getNumericValue(p);
    deValue* destination = mainLayerImage.startWrite(channel);
    int n = mainLayerImage.getChannelSize().getN();

    fillChannel(destination, n, value);

    mainLayerImage.finishWrite(channel);

    return true;
}


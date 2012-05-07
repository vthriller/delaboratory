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

#include "tone_layer.h"
#include "preset.h"
#include "view_manager.h"
#include "color_space_utils.h"
#include "copy_channel.h"
#include "logger.h"
#include "str.h"

deToneLayer::deToneLayer(deColorSpace _colorSpace, deChannelManager& _channelManager, int _sourceLayer, deLayerStack& _layerStack)
:deLayerWithBlending(_colorSpace, _channelManager, _sourceLayer, _layerStack)
{
    dePreset* reset = createPreset("reset");

    createPropertyChoice("channel", getChannelNames(colorSpace));

    int n = getColorSpaceSize(colorSpace);

    int i;
    for (i = 0; i < n; i++)
    {
        std::string nd = "dark " + getChannelName(colorSpace, i);
        createPropertyNumeric(nd, 0, 1);
        reset->addNumericValue(nd, 0.0);
    }

    for (i = 0; i < n; i++)
    {
        std::string nl = "light " + getChannelName(colorSpace, i);
        createPropertyNumeric(nl, 0, 1);
        reset->addNumericValue(nl, 1.0);
    }

    applyPreset("reset");
}

deToneLayer::~deToneLayer()
{
}
bool deToneLayer::updateMainImageNotThreadedWay()
{
    deSize size = mainLayerImage.getChannelSize();
    int n = size.getN();

    int nc = getColorSpaceSize(colorSpace);

    int c = getPropertyChoice("channel")->getIndex();

    const deValue* source = getSourceImage().startRead(c);

    if (!source)
    {
        logError("broken source in tone layer");
        return false;
    }

    int channel;
    for (channel = 0; channel < nc; channel++)
    {
        std::string nd = "dark " + getChannelName(colorSpace, channel);
        std::string nl = "light " + getChannelName(colorSpace, channel);
        deValue dark = getNumericValue(nd);
        deValue light = getNumericValue(nl);

        mainLayerImage.enableChannel(channel);

        deValue* destination = mainLayerImage.startWrite(channel);

        int i;
        for (i = 0; i < n; i++)
        {
            deValue v = source[i];
            v = (light - dark) * v + dark;
            destination[i] = v;
        }

        mainLayerImage.finishWrite(channel);
    }        

    getSourceImage().finishRead(c);

    return true;
}    


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
#include "curve.h"
#include "property_boolean.h"

deToneLayer::deToneLayer(deColorSpace _colorSpace, deChannelManager& _channelManager, int _sourceLayer, deLayerStack& _layerStack)
:deLayerWithBlending(_colorSpace, _channelManager, _sourceLayer, _layerStack)
{
    dePreset* reset = createPreset("reset");

    createPropertyChoice("channel", getChannelNames(colorSpace));

    createPropertyBoolean("dark");
    createPropertyBoolean("half");
    createPropertyBoolean("light");

    int n = getColorSpaceSize(colorSpace);

    int i;
    for (i = 0; i < n; i++)
    {
        std::string nd = "dark " + getChannelName(colorSpace, i);
        createPropertyNumeric(nd, 0, 1);
        reset->addNumericValue(nd, 0.25);
    }

    for (i = 0; i < n; i++)
    {
        std::string nl = "half " + getChannelName(colorSpace, i);
        createPropertyNumeric(nl, 0, 1);
        reset->addNumericValue(nl, 0.5);
    }

    for (i = 0; i < n; i++)
    {
        std::string nl = "light " + getChannelName(colorSpace, i);
        createPropertyNumeric(nl, 0, 1);
        reset->addNumericValue(nl, 0.75);
    }

    getPropertyBoolean("half")->set(true);

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

    bool useD = getPropertyBoolean("dark")->get();
    bool useH = getPropertyBoolean("half")->get();
    bool useL = getPropertyBoolean("light")->get();

    int channel;
    for (channel = 0; channel < nc; channel++)
    {
        std::string nd = "dark " + getChannelName(colorSpace, channel);
        std::string nh = "half " + getChannelName(colorSpace, channel);
        std::string nl = "light " + getChannelName(colorSpace, channel);
        deValue dark = getNumericValue(nd);
        deValue light = getNumericValue(nl);
        deValue half = getNumericValue(nh);

        //mainLayerImage.enableChannel(channel);

        deValue* destination = mainLayerImage.startWrite(channel);

        deBaseCurve curve;

        curve.addPoint(0, 0);
        if (useD)
        {
            curve.addPoint(0.25, dark);
        }
        if (useH)
        {
            curve.addPoint(0.5, half);
        }
        if (useL)
        {
            curve.addPoint(0.75, light);
        }            
        curve.addPoint(1, 1);

        curve.build();

        curve.process(source, destination, n);

        mainLayerImage.finishWrite(channel);
    }        

    getSourceImage().finishRead(c);

    return true;
}    


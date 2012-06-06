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

#include "mixer_layer.h"
#include "project.h"
#include <iostream>
#include "color_space_utils.h"
#include "channel_manager.h"
#include "property_mixer.h"
#include "preset.h"

deMixerLayer::deMixerLayer(deColorSpace _colorSpace, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager)
:deLayerWithBlending(_colorSpace, _channelManager,  _sourceLayer, _layerStack)
{
    dePreset* reset = createPreset("reset");
    reset->addOperation("reset");

    int n = getColorSpaceSize(colorSpace);

    properties.push_back(new dePropertyMixer("mixer", n));

    applyPreset("reset");

}

deMixerLayer::~deMixerLayer()
{
}

bool deMixerLayer::updateMainImageSingleChannel(int channel)
{
/*
    if (!isChannelEnabled(channel))
    {
        mainLayerImage.disableChannel(channel, getSourceImage().getChannelIndex(channel));
        return true;
    }
    */

    const deImage& sourceImage = getSourceImage();

    //mainLayerImage.enableChannel(channel);

    //int c = mainLayerImage.getChannelIndex(channel);

    dePropertyMixer* p = dynamic_cast<dePropertyMixer*>(getProperty("mixer"));
    if (!p)
    {
        logError("no property mixer");
        return false;
    }

    int channelSize = mainLayerImage.getChannelSize().getN();
    deValue* destination = mainLayerImage.startWrite(channel);
    p->getMixer(channel)->process(sourceImage, destination, channelSize);
    mainLayerImage.finishWrite(channel);

    return true;
}


deMixer* deMixerLayer::getMixer(int index)
{
    dePropertyMixer* p = dynamic_cast<dePropertyMixer*>(getProperty("mixer"));
    if (!p)
    {
        return NULL;
    }
    return p->getMixer(index);
}

bool deMixerLayer::isChannelNeutral(int index)
{
    dePropertyMixer* p = dynamic_cast<dePropertyMixer*>(getProperty("mixer"));
    if (!p)
    {
        return false;
    }
    return p->getMixer(index)->isNeutral(index);
}    

void deMixerLayer::setWeight(int s, int d, deValue value)
{
    dePropertyMixer* p = dynamic_cast<dePropertyMixer*>(getProperty("mixer"));
    if (!p)
    {
        return;
    }
    p->getMixer(d)->setWeight(s, value);
}

deValue deMixerLayer::getWeight(int s, int d)
{
    dePropertyMixer* p = dynamic_cast<dePropertyMixer*>(getProperty("mixer"));
    if (!p)
    {
        return -1;
    }
    return p->getMixer(d)->getWeight(s);
}

void deMixerLayer::executeOperation(const std::string& operation)
{
    int n = getColorSpaceSize(colorSpace);

    int i;
    for (i = 0; i < n; i++)
    {
        int j;
        for (j = 0; j < n; j++)
        {
            deValue v = 0.0;
            if (i == j)
            {
                v = 1.0;
            }
            setWeight(i, j, v);
        }
    }
}

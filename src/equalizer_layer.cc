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

#include "equalizer_layer.h"
#include "project.h"
#include <iostream>
#include "frame_factory.h"
#include "equalizer.h"
#include "color_space_utils.h"
#include "channel_manager.h"

deEqualizerLayer8::deEqualizerLayer8(deColorSpace _colorSpace, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager)
:deEqualizerLayer(_colorSpace, _sourceLayer, _layerStack, _channelManager, _viewManager, 8)
{
}

deEqualizerLayer8::~deEqualizerLayer8()
{
}

deEqualizerLayer16::deEqualizerLayer16(deColorSpace _colorSpace, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager)
:deEqualizerLayer(_colorSpace, _sourceLayer, _layerStack, _channelManager, _viewManager, 16)
{
}

deEqualizerLayer16::~deEqualizerLayer16()
{
}

deEqualizerLayer::deEqualizerLayer(deColorSpace _colorSpace, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager, int _bands)
:deLayer(_colorSpace, _sourceLayer, _layerStack, _channelManager),
 bands(_bands)
{
    int n = getColorSpaceSize(colorSpace);
    int i;

    for (i = 0; i < n; i++)
    {
        equalizers.push_back( new deEqualizer(bands));
    }

}

deEqualizerLayer::~deEqualizerLayer()
{
    int n = getColorSpaceSize(colorSpace);
    int i;
    for (i = 0; i < n; i++)
    {
        delete equalizers[i];
    }
}

bool deEqualizerLayer::updateMainImageSingleChannel(int channel)
{
    const deImage& sourceImage = getSourceImage();

    int e = getEqualizerChannel(colorSpace);

    mainLayerImage.enableChannel(channel);

    int c = mainLayerImage.getChannelIndex(channel);
    deChannel* destination = channelManager.getChannel(c);

    const deValue* eChannel = sourceImage.getValues(e);
    const deValue* sChannel = sourceImage.getValues(channel);

    bool wrap = isChannelWrapped(colorSpace, e);

    if ((eChannel) && (sChannel) && (destination))
    {
        int channelSize = mainLayerImage.getChannelSize().getN();
        equalizers[channel]->process(sChannel, eChannel, *destination, channelSize, wrap);
    }

    return true;
}

bool deEqualizerLayer::isChannelNeutral(int index)
{
    return false;
}    


void deEqualizerLayer::save(xmlNodePtr root)
{
    saveCommon(root);
    saveBlend(root);

    int n = getColorSpaceSize(colorSpace);
    int i;
    for (i = 0; i < n; i++)
    {
    }
}

void deEqualizerLayer::load(xmlNodePtr root)
{
    loadBlend(root);

    xmlNodePtr child = root->xmlChildrenNode;

    while (child)
    {

        child = child->next;
    }
}

void deEqualizerLayer::reset()
{
    std::vector<deEqualizer*>::iterator i;
    for (i = equalizers.begin(); i != equalizers.end(); i++)
    {
        (*i)->reset();
    }
}

deEqualizer* deEqualizerLayer::getEqualizer(int index)
{
    int n = getColorSpaceSize(colorSpace);
    if ((index >= 0) && ( index < n))
    {
        return equalizers[index];
    }
    return NULL;
}



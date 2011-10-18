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

#include "source_image_layer.h"
#include "channel_manager.h"
#include "scale_channel.h"
#include <iostream>

deSourceImageLayer::deSourceImageLayer(int _index, deChannelManager& _previewChannelManager)
:deLayer("source image", deColorSpaceRGB, _index, -1), 
previewChannelManager(_previewChannelManager),
sourceR(-1), 
sourceG(-1), 
sourceB(-1), 
image(deColorSpaceRGB, _previewChannelManager) 
{
    sourceChannelManager = NULL;
}

deSourceImageLayer::~deSourceImageLayer()
{
}

void deSourceImageLayer::setSource(int r, int g, int b, deChannelManager* _sourceChannelManager)
{
    sourceChannelManager = _sourceChannelManager;
    sourceR = r;
    sourceG = g;
    sourceB = b;
}

void deSourceImageLayer::updateImage()
{
    if (!sourceChannelManager)
    {
        return;
    }

    deChannel* sourceChannelR = sourceChannelManager->getChannel(sourceR);
    deChannel* sourceChannelG = sourceChannelManager->getChannel(sourceG);
    deChannel* sourceChannelB = sourceChannelManager->getChannel(sourceB);

    if (!sourceChannelR)
    {
        return;
    }

    image.enableChannel(0);
    image.enableChannel(1);
    image.enableChannel(2);

    deChannel* channelR = previewChannelManager.getChannel(image.getChannelIndex(0));
    deChannel* channelG = previewChannelManager.getChannel(image.getChannelIndex(1));
    deChannel* channelB = previewChannelManager.getChannel(image.getChannelIndex(2));

    const deSize ss = sourceChannelManager->getChannelSize();
    const deSize ds = previewChannelManager.getChannelSize();

    scaleChannel(*sourceChannelR, *channelR, ss, ds);
    scaleChannel(*sourceChannelG, *channelG, ss, ds);
    scaleChannel(*sourceChannelB, *channelB, ss, ds);

}

const deImage& deSourceImageLayer::getImage() const
{
    return image;
}
 
void deSourceImageLayer::updateChannelUsage(std::map<int, int>& channelUsage) const
{
    image.updateChannelUsage(channelUsage, index);
}

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
#include "project.h"
#include "scale_channel.h"
#include <iostream>

deSourceImageLayer::deSourceImageLayer(int _index, deProject& _project)
:deLayer("source image", deColorSpaceRGB, _index, -1), 
project(_project),
sourceR(-1), 
sourceG(-1), 
sourceB(-1), 
image(deColorSpaceRGB, _project.getPreviewChannelManager()) 
{
    //image.setInvalid();
}

deSourceImageLayer::~deSourceImageLayer()
{
}

void deSourceImageLayer::setSource(int r, int g, int b)
{
    sourceR = r;
    sourceG = g;
    sourceB = b;
}

void deSourceImageLayer::updateImage()
{
    deChannelManager& previewChannelManager = project.getPreviewChannelManager();
    deChannelManager& sourceChannelManager = project.getSourceChannelManager();

/*
    std::cout << "deSourceImageLayer::updatePreview sourceR: " << sourceR << std::endl;
    std::cout << "deSourceImageLayer::updatePreview sourceG: " << sourceG << std::endl;
    std::cout << "deSourceImageLayer::updatePreview sourceB: " << sourceB << std::endl;
*/
    deChannel* sourceChannelR = sourceChannelManager.getChannel(sourceR);
    deChannel* sourceChannelG = sourceChannelManager.getChannel(sourceG);
    deChannel* sourceChannelB = sourceChannelManager.getChannel(sourceB);

    if (!sourceChannelR)
    {
        return;
    }

/*
    std::cout << "sourceImage r: " << image.getChannel(0) << std::endl;
    std::cout << "sourceImage g: " << image.getChannel(1) << std::endl;
    std::cout << "sourceImage b: " << image.getChannel(2) << std::endl;
*/

    image.enableChannel(0);
    image.enableChannel(1);
    image.enableChannel(2);

    deChannel* channelR = previewChannelManager.getChannel(image.getChannelIndex(0));
    deChannel* channelG = previewChannelManager.getChannel(image.getChannelIndex(1));
    deChannel* channelB = previewChannelManager.getChannel(image.getChannelIndex(2));

    const deSize ss = sourceChannelManager.getChannelSize();
    const deSize ds = previewChannelManager.getChannelSize();

    scaleChannel(*sourceChannelR, *channelR, ss, ds);
    scaleChannel(*sourceChannelG, *channelG, ss, ds);
    scaleChannel(*sourceChannelB, *channelB, ss, ds);

    //image.setValid();

//    std::cout << "deSourceImageLayer::updatePreview done" << std::endl;

    /*

    for (i = 0; i < n; i++)
    {
        const deChannel* sourceChannel = dynamic_cast<const deTrueChannel*>(sourceImage->getChannel(i));
        if (!sourceChannel)
        {
            throw deException("no sourceChannel in deSourceImageLayer::generatePreview");
        }
        deTrueChannel* destinationChannel = dynamic_cast<deTrueChannel*>(preview->getChannel(i));
        if (!destinationChannel)
        {
            throw deException("no destinationChannel in deSourceImageLayer::generatePreview");
        }
        destinationChannel->scale(sourceChannel);
    }*/

}

const deImage& deSourceImageLayer::getImage() const
{
    return image;
}
 
void deSourceImageLayer::updateChannelUsage(std::map<int, int>& channelUsage) const
{
    image.updateChannelUsage(channelUsage, index);
}

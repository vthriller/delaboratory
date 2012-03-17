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
#include "view_manager.h"
#include "logger.h"
#include "static_image.h"
#include "copy_channel.h"

deSourceImageLayer::deSourceImageLayer(int _index, deChannelManager& _previewChannelManager, deViewManager& _viewManager, deStaticImage& _sourceImage, deColorSpace _colorSpace)
:deLayer("source image", _colorSpace, _index, -1),
previewChannelManager(_previewChannelManager),
viewManager(_viewManager),
image(_colorSpace, _previewChannelManager),
sourceImage(_sourceImage)
{
}

deSourceImageLayer::~deSourceImageLayer()
{
}

bool deSourceImageLayer::updateImage()
{
    logMessage("source image layer update image start");

    deChannel* sourceChannelR = sourceImage.getChannel(0);
    deChannel* sourceChannelG = sourceImage.getChannel(1);
    deChannel* sourceChannelB = sourceImage.getChannel(2);

    if (!sourceChannelR)
    {
        std::cout << "deSourceImageLayer::updateImage !sourceChannelR" << std::endl;
        return false;
    }

    logMessage("enabling channels...");
    image.enableChannel(0);
    image.enableChannel(1);
    image.enableChannel(2);
    logMessage("enabled channels");

    deChannel* channelR = previewChannelManager.getChannel(image.getChannelIndex(0));
    deChannel* channelG = previewChannelManager.getChannel(image.getChannelIndex(1));
    deChannel* channelB = previewChannelManager.getChannel(image.getChannelIndex(2));

    sourceChannelR->lockRead();
    sourceChannelG->lockRead();
    sourceChannelB->lockRead();
    channelR->lockWrite();
    channelG->lockWrite();
    channelB->lockWrite();

    const deSize ss = sourceImage.getSize();

    const deSize ds = previewChannelManager.getChannelSize();

    deValue z_x1;
    deValue z_y1;
    deValue z_x2;
    deValue z_y2;
    viewManager.getZoom(z_x1, z_y1, z_x2, z_y2);

    int x1 = ss.getW() * z_x1;
    int y1 = ss.getH() * z_y1;
    int x2 = ss.getW() * z_x2;
    int y2 = ss.getH() * z_y2;

    int w = ds.getW();
    int h = ds.getH();

    int ws = ss.getW();

    if (ss == ds)
    {
        copyChannel(sourceChannelR->getPixels(), channelR->getPixels(), ss);
        copyChannel(sourceChannelG->getPixels(), channelG->getPixels(), ss);
        copyChannel(sourceChannelB->getPixels(), channelB->getPixels(), ss);
    }
    else
    {
        scaleChannel(sourceChannelR->getPixels(), channelR->getPixels(), x1, y1, x2, y2, w, h, ws);
        scaleChannel(sourceChannelG->getPixels(), channelG->getPixels(), x1, y1, x2, y2, w, h, ws);
        scaleChannel(sourceChannelB->getPixels(), channelB->getPixels(), x1, y1, x2, y2, w, h, ws);
    }

    sourceChannelR->unlockRead();
    sourceChannelG->unlockRead();
    sourceChannelB->unlockRead();
    channelR->unlockWrite();
    channelG->unlockWrite();
    channelB->unlockWrite();

    logMessage("source image layer update image end");

    return true;
}

const deImage& deSourceImageLayer::getImage() const
{
    return image;
}
 
void deSourceImageLayer::updateChannelUsage(std::map<int, int>& channelUsage) const
{
    image.updateChannelUsage(channelUsage, index);
}


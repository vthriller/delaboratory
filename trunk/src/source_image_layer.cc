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

deSourceImageLayer::deSourceImageLayer(int _index, deChannelManager& _previewChannelManager, deViewManager& _viewManager, deStaticImage& _sourceImage)
:deLayer("source image", deColorSpaceRGB, _index, -1),
previewChannelManager(_previewChannelManager),
viewManager(_viewManager),
image(deColorSpaceRGB, _previewChannelManager),
sourceImage(_sourceImage)
{
}

deSourceImageLayer::~deSourceImageLayer()
{
}

void deSourceImageLayer::updateImage()
{
    logMessage("source image layer update image start");

    deChannel* sourceChannelR = sourceImage.getChannel(0);
    deChannel* sourceChannelG = sourceImage.getChannel(1);
    deChannel* sourceChannelB = sourceImage.getChannel(2);


    if (!sourceChannelR)
    {
        std::cout << "deSourceImageLayer::updateImage !sourceChannelR" << std::endl;
        return;
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

    deValue scale = 1.0;
    deValue offsetX = 0.0;
    deValue offsetY = 0.0;

    deValue centerX = (offsetX + 1.0) / 2.0;
    deValue centerY = (offsetY + 1.0) / 2.0;

    int ws = ss.getW();
    int hs = ss.getH();
    deValue sAspect = ss.getAspect();
    deValue dAspect = ds.getAspect();

    centerX *= ws;
    centerY *= hs;

    int wd = ds.getW();
    int hd = ds.getH();

    float scaleW = (float) ws / wd;
    float scaleH = (float) hs / hd;

    scaleH *= (sAspect / dAspect);

    scaleW /= scale;
    scaleH /= scale;

    int ox = centerX - scaleW * (wd / 2);
    int oy = centerY - scaleH * (hd / 2);

    int maxox = ws - scaleW * ( wd - 1);
    int maxoy = hs - scaleH * ( hd - 1);

    if (ox > maxox)
    {
        ox = maxox;
    }
    if (oy > maxoy)
    {
        oy = maxoy;
    }
    if (ox < 0)
    {
        ox = 0;
    }
    if (oy < 0)
    {
        oy = 0;
    }

    int maxy = hd;
    int maxyy = (hs - oy) / scaleH;
    if (maxyy < maxy)
    {
        maxy = maxyy;
    }

    int maxx = wd;
    int maxxx = (ws - ox) / scaleW;
    if (maxxx < maxx)
    {
        maxx = maxxx;
    }

    scaleChannel(sourceChannelR->getPixels(), channelR->getPixels(), maxx, ox, scaleW, maxy, oy, scaleH, ws, hs, wd);
    scaleChannel(sourceChannelG->getPixels(), channelG->getPixels(), maxx, ox, scaleW, maxy, oy, scaleH, ws, hs, wd);
    scaleChannel(sourceChannelB->getPixels(), channelB->getPixels(), maxx, ox, scaleW, maxy, oy, scaleH, ws, hs, wd);

    sourceChannelR->unlockRead();
    sourceChannelG->unlockRead();
    sourceChannelB->unlockRead();
    channelR->unlockWrite();
    channelG->unlockWrite();
    channelB->unlockWrite();

    logMessage("source image layer update image end");
}

const deImage& deSourceImageLayer::getImage() const
{
    return image;
}
 
void deSourceImageLayer::updateChannelUsage(std::map<int, int>& channelUsage) const
{
    image.updateChannelUsage(channelUsage, index);
}


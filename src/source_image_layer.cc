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
#include "str.h"

deSourceImageLayer::deSourceImageLayer(deChannelManager& _previewChannelManager, deViewManager& _viewManager, deStaticImage& _sourceImage, deColorSpace _colorSpace)
:deBaseLayer(_colorSpace, _previewChannelManager) ,
viewManager(_viewManager),
sourceImage(_sourceImage)
{
}

deSourceImageLayer::~deSourceImageLayer()
{
}

bool deSourceImageLayer::updateImage()
{
    const deSize ss = sourceImage.getSize();

    const deSize ds = channelManager.getChannelSize();

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
    int nn = ss.getN();

    int i;
    for (i = 0; i < 3; i++)
    {
        deChannel* sourceChannel = sourceImage.getChannel(i);
        mainLayerImage.enableChannel(i);
        deChannel* channel = channelManager.getChannel(mainLayerImage.getChannelIndex(i));
        sourceChannel->lockRead();
        channel->lockWrite();
        if (ss == ds)
        {
            copyChannel(sourceChannel->getPixels(), channel->getPixels(), nn);
        }
        else
        {
            scaleChannel(sourceChannel->getPixels(), channel->getPixels(), x1, y1, x2, y2, w, h, ws);
        }
        sourceChannel->unlockRead();
        channel->unlockWrite();
    }

    return true;
}
 

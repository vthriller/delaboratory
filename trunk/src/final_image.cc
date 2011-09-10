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

#include "final_image.h"
#include "preview.h"
#include "color_space.h"
#include "channels.h"
#include "channel.h"
#include "converter.h"
#include <tiffio.h>
#include <iostream>
#include "image_io.h"

deFinalImage::deFinalImage(const dePreview& preview)
:dePreview(deColorSpaceRGB, preview.getChannel(0)->getSize())
{
    deConverter converter;
    converter.setSource(&preview);
    converter.setDestination(this);
    converter.convert();
}

deFinalImage::~deFinalImage()
{
}

void deFinalImage::saveJPEG(const std::string& fileName)
{
    const deBaseChannel* channelR = getChannel(DE_CHANNEL_RED);
    const deBaseChannel* channelG = getChannel(DE_CHANNEL_GREEN);
    const deBaseChannel* channelB = getChannel(DE_CHANNEL_BLUE);
    ::saveJPEG(fileName, *channelR, *channelG, *channelB, size);
}

void deFinalImage::saveTIFF(const std::string& fileName)
{
    const deBaseChannel* channelR = getChannel(DE_CHANNEL_RED);
    const deBaseChannel* channelG = getChannel(DE_CHANNEL_GREEN);
    const deBaseChannel* channelB = getChannel(DE_CHANNEL_BLUE);
    ::saveTIFF(fileName, *channelR, *channelG, *channelB, size);
}

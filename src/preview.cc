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

#include "preview.h"
#include "color_space.h"
#include "channel.h"
#include "empty_channel.h"
#include <iostream>

dePreview::dePreview(const deColorSpace& _colorSpace, const deSize& _size)
:colorSpace(_colorSpace), size(_size)
{
    int i;
    for (i = 0; i < 4; i++)
    {
        channels[i] = NULL;
    }
    createChannels(size);
}

dePreview::dePreview(const deColorSpace& _colorSpace)
:colorSpace(_colorSpace), size(0,0)
{
    int i;
    for (i = 0; i < 4; i++)
    {
        channels[i] = NULL;
    }
}


dePreview::~dePreview()
{
    destroyChannels();
}

deBaseChannel* dePreview::getChannel(int id)
{
    if ((id <0) || (id >= 4))
    {
        return NULL;
    }
    return channels[id];
}

const deBaseChannel* dePreview::getChannel(int id) const
{
    if ((id <0) || (id >= 4))
    {
        return NULL;
    }
    return channels[id];
}

void dePreview::createChannels(const deSize& size)
{
    int i;
    for (i = 0; i < 4; i++)
    {
        deBaseChannel* oldChannel = channels[i];
        if (oldChannel)
        {
            delete oldChannel;
            channels[i] = NULL;
        }
    }

    int n = getColorSpaceSize(colorSpace);
    for (i = 0; i < n; i++)
    {
        deValue min;
        deValue max;
        getColorSpaceChannelRanges(colorSpace, i, min, max);
        channels[i] = new deChannel(size, min, max);
    }
    for (i = n; i < 4; i++)
    {
        channels[i] = new deEmptyChannel(size);
    }
}

void dePreview::destroyChannels()
{
    int i;
    for (i = 0; i < 4; i++)
    {
        delete channels[i];
        channels[i] = NULL;
    }
}

const deColorSpace& dePreview::getColorSpace() const
{
    return colorSpace;
}

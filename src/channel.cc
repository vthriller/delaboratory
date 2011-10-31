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

#include "channel.h"
#include <string>
#include <cassert>

deChannel::deChannel(deImage& _image)
:image(_image)
{
    pixels = NULL;
}

deChannel::~deChannel()
{
    if (pixels)
    {
        deallocate();
    }
}

deValue* deChannel::getPixels()
{
    return pixels;
}

const deValue* deChannel::getPixels() const
{
    return pixels;
}

deValue deChannel::getValue(int pos) const
{
    return pixels[pos];
}

void deChannel::setValue(int pos, const deValue& value)
{
    pixels[pos] = value;
}

void deChannel::setValueClip(int pos, const deValue& value)
{
    if (value < 0)
    {
        pixels[pos] = 0;
        return;
    }
    if (value > 1)
    {
        pixels[pos] = 1;
        return;
    }
    pixels[pos] = value;
}

void deChannel::deallocate()
{
    assert(pixels);
    delete [] pixels;
    pixels = NULL;
}

void deChannel::allocate(int size)
{
    assert(!pixels);
    pixels = new deValue [size];
}

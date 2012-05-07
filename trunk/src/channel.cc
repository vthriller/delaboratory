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
#include "logger.h"
#include "str.h"

#define CHANNEL_LOCKING 1

deChannel::deChannel()
{
    pixels = NULL;
    magicSize = -1;
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
    if (pos < 0) 
    {
        return 0.0;
    }
    if (pos >= magicSize) 
    {
        return 0.0;
    }
    deValue v = pixels[pos];
    return v;
}

void deChannel::setValue(int pos, const deValue& value)
{
    if (pos < 0) 
    {
        return;
    }
    if (pos >= magicSize) 
    {
        return;
    }
    pixels[pos] = value;
}

void deChannel::setValueClip(int pos, const deValue& value)
{
    if (pos < 0) 
    {
        return;
    }
    if (pos >= magicSize) 
    {
        return;
    }
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
    logInfo("deallocate channel");
    delete [] pixels;
    pixels = NULL;
    magicSize = 0;
}

void deChannel::allocate(int size)
{
    assert(!pixels);
    try
    {
        pixels = new deValue [size];
        magicSize = size;
    }
    catch (std::bad_alloc)
    {
        logError("ERROR allocating channel");
        pixels = NULL;
        magicSize = 0;
    }

}


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
:readSemaphore(4, 4)
{
    pixels = NULL;
    maxReaders = 4;
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
    lockWrite();
    assert(pixels);
    logMessage("deallocate channel");
    delete [] pixels;
    pixels = NULL;
    magicSize = 0;
    unlockWrite();
}

void deChannel::allocate(int size)
{
    lockWrite();
    assert(!pixels);
    try
    {
        pixels = new deValue [size];
        magicSize = size;
    }
    catch (std::bad_alloc)
    {
        logMessage("ERROR allocating channel");
        pixels = NULL;
        magicSize = 0;
    }

    unlockWrite();
}

void deChannel::lockRead() const
{
#if CHANNEL_LOCKING
    readSemaphore.Wait();
#endif    
}

void deChannel::unlockRead() const
{
#if CHANNEL_LOCKING
    readSemaphore.Post();
#endif    
}

void deChannel::lockWrite()
{
#if CHANNEL_LOCKING
    int i;
    for (i = 0; i < maxReaders; i++)
    {
        readSemaphore.Wait();
    }
    lockWithLog(writeMutex, "channel write mutex");
#endif    
}

void deChannel::unlockWrite()
{
#if CHANNEL_LOCKING
    writeMutex.unlock();
    int i;
    for (i = 0; i < maxReaders; i++)
    {
        readSemaphore.Post();
    }
#endif    
}

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

#include "static_image.h"
#include "channel.h"
#include "logger.h"
#include "color_space_utils.h"
#include "str.h"


deStaticImage::deStaticImage()
:colorSpace(deColorSpaceInvalid),
 size(0,0)
{
    int n = 3;
    int i;
    for (i = 0; i < n; i++)
    {
        deChannel* c = new deChannel();
        channels.push_back(c);
        mutexes.push_back(new deMutexReadWrite(4));
        channels[i]->allocate(size.getN());
    }
}

void deStaticImage::setColorSpace(deColorSpace c)
{
    if (getColorSpaceSize(c) == 3)
    {
        colorSpace = c;
    }
}    

deStaticImage::~deStaticImage()
{
    unsigned int i;
    for (i = 0; i < channels.size(); i++)
    {
        delete channels[i];
        delete mutexes[i];
    }
}

deColorSpace deStaticImage::getColorSpace() const
{
    return colorSpace;
}

void deStaticImage::setSize(const deSize& _size)
{
    if (size == _size)
    {
        return;
    }
    size = _size;
    int n = 3;
    int i;
    for (i = 0; i < n; i++)
    {
        channels[i]->deallocate();
        channels[i]->allocate(size.getN());
    }
}

deSize deStaticImage::getSize() const
{
    return size;
}

void deStaticImage::lock()
{
    mutex.lock();
}    

void deStaticImage::unlock()
{
    mutex.unlock();
}    

void deStaticImage::setInfo(const std::string& s)
{
    info = s;
}

const deValue* deStaticImage::startReadStatic(int index)
{
    int n = 3;
    if (index < 0)
    {
        logError("deStaticImage::startReadStatic index: " + str(index));
        return NULL;
    }
    if (index >= n)
    {
        logError("deStaticImage::startReadStatic index: " + str(index));
        return NULL;
    }
    mutexes[index]->lockRead();
    return channels[index]->getPixels();
}

deValue* deStaticImage::startWriteStatic(int index)
{
    int n = 3;
    if (index < 0)
    {
        logError("deStaticImage::startWriteStatic index: " + str(index));
        return NULL;
    }
    if (index >= n)
    {
        logError("deStaticImage::startWriteStatic index: " + str(index));
        return NULL;
    }
    mutexes[index]->lockWrite();
    return channels[index]->getPixels();
}

void deStaticImage::finishReadStatic(int index)
{
    int n = 3;
    if (index < 0)
    {
        logError("deStaticImage::finishReadStatic index: " + str(index));
        return;
    }
    if (index >= n)
    {
        logError("deStaticImage::finishReadStatic index: " + str(index));
        return;
    }
    mutexes[index]->unlockRead();
}

void deStaticImage::finishWriteStatic(int index)
{
    int n = 3;
    if (index < 0)
    {
        logError("deStaticImage::finishWriteStatic index: " + str(index));
        return;
    }
    if (index >= n)
    {
        logError("deStaticImage::finishWriteStatic index: " + str(index));
        return;
    }
    mutexes[index]->unlockWrite();
}

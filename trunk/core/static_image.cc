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
#include "logger.h"
#include "color_space_utils.h"
#include "str.h"
#include "copy_channel.h"
#include "scale_channel.h"

deStaticImage::deStaticImage()
:colorSpace(deColorSpaceInvalid),
 size(0,0)
{
    lastRotate = -1;

    int n = 3;
    int i;
    for (i = 0; i < n; i++)
    {
        channels.push_back(NULL);
        mutexes.push_back(new deMutexReadWrite(4));
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
        if (channels[i])
        {
            delete [] channels[i];
        }
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
    logInfo("static image set size " + str(size.getW()) + "x" + str(size.getH())); 
    int n = 3;
    int i;
    for (i = 0; i < n; i++)
    {
        if (channels[i])
        {
            delete [] channels[i];
        }            
        channels[i] = new deValue[size.getN()];
    }
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
    return channels[index];
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
    return channels[index];
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

bool deStaticImage::isEmpty() const
{
    return size.isEmpty();
}

void deStaticImage::copyToChannel(int channel, deValue* destination, deValue z_x1, deValue z_y1, deValue z_x2, deValue z_y2, deSize ds, bool mirrorX, bool mirrorY, int rotate)
{
    lastRotate = rotate;

    int w = ds.getW();
    int h = ds.getH();
    if (w <= 0)
    {
        logError("can't copy static image w: " + str(w));
        return;
    }
    if (h <= 0)
    {
        logError("can't copy static image h: " + str(h));
        return;
    }
    const deValue* source = startReadStatic(channel);

    int ws = size.getW();
    int hs = size.getH();

    scaleChannel(source, destination, z_x1, z_y1, z_x2, z_y2, w, h, mirrorX, mirrorY, rotate, ws, hs);

    finishReadStatic(channel);
}

deSize deStaticImage::getStaticImageSize() const
{
    if ((lastRotate == 90) || (lastRotate==270))
    {
        return size.rotated();
    }
    else
    {
        return size;
    }
}

deValue deStaticImage::getAspect() const
{
    deValue aspect = size.getAspect();
    return aspect;
}

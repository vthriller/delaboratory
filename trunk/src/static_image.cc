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
#include "copy_channel.h"


deStaticImage::deStaticImage()
:colorSpace(deColorSpaceInvalid),
 size(0,0)
{
    rotation = 0;
    lastRotate = -1;

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
    logInfo("static image set size " + str(size.getW()) + "x" + str(size.getH())); 
    int n = 3;
    int i;
    for (i = 0; i < n; i++)
    {
        channels[i]->deallocate();
        channels[i]->allocate(size.getN());
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

    scaleChannel(source, destination, z_x1, z_y1, z_x2, z_y2, w, h, mirrorX, mirrorY, rotate);

    finishReadStatic(channel);
}

deValue deStaticImage::samplePixel(const deValue* src, int xx1, int xx2, int yy1, int yy2, bool mirrorX, bool mirrorY)
{
    int ws = size.getW();
    int hs = size.getH();

    if (xx1 < 0)
    {
        xx1 = 0;
    }
    else if (xx1 >= ws)
    {
        xx1 = ws - 1;
    }
    if (xx2 < 0)
    {
        xx2 = 0;
    }
    else if (xx2 >= ws)
    {
        xx2 = ws - 1;
    }
    if (yy1 < 0)
    {
        yy1 = 0;
    }
    else if (yy1 >= hs)
    {
        yy1 = hs - 1;
    }
    if (yy2 < 0)
    {
        yy2 = 0;
    }
    else if (yy2 >= hs)
    {
        yy2 = hs - 1;
    }

    int n = 0;
    int x0;
    int y0;
    deValue value = 0.0;

    if ((!mirrorX) && (!mirrorY))
    {
        for (x0 = xx1; x0 <= xx2; x0++)
        {
            for (y0 = yy1; y0 <= yy2; y0++)
            {
                value += src[x0 + y0 * ws];
                n++;
            }
        }
    }
    if ((mirrorX) && (!mirrorY))
    {
        for (x0 = xx1; x0 <= xx2; x0++)
        {
            for (y0 = yy1; y0 <= yy2; y0++)
            {
                value += src[(ws - 1 - x0) + y0 * ws];
                n++;
            }
        }
    }
    if ((!mirrorX) && (mirrorY))
    {
        for (x0 = xx1; x0 <= xx2; x0++)
        {
            for (y0 = yy1; y0 <= yy2; y0++)
            {
                value += src[x0 + (hs - 1 - y0) * ws];
                n++;
            }
        }
    }
    if ((mirrorX) && (mirrorY))
    {
        for (x0 = xx1; x0 <= xx2; x0++)
        {
            for (y0 = yy1; y0 <= yy2; y0++)
            {
                value += src[(ws - 1 - x0) + (hs - 1 - y0) * ws];
                n++;
            }
        }
    }

    if (n == 0)
    {
        logError("sample pixel failed, no pixels inside box");
        return 0.0;
    }

    return value / n;
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

void deStaticImage::scaleChannel(const deValue* src, deValue* dst, deValue z_x1, deValue z_y1, deValue z_x2, deValue z_y2, int w, int h, bool mirrorX, bool mirrorY, int rotate)
{
    logInfo("scale channel START");

    int ws = size.getW();
    int hs = size.getH();

    int x1;
    int y1;
    int x2;
    int y2;
    x1 = ws * z_x1;
    y1 = hs * z_y1;
    x2 = ws * z_x2;
    y2 = hs * z_y2;
    if ((rotate == 90) || (rotate == 270))
    {
        x1 = hs * z_x1;
        y1 = ws * z_y1;
        x2 = hs * z_x2;
        y2 = ws * z_y2;
    }

    deValue scaleW;
    deValue scaleH;

    deValue dx = x2 - x1;
    deValue dy = y2 - y1;

    scaleW = dx / w;
    scaleH = dy / h;

    if (scaleW <= 0)
    {
        logError("scaleW: " + str(scaleW));
    }
    if (scaleH <= 0)
    {
        logError("scaleH: " + str(scaleH));
    }

    logInfo("scaleW: " + str(scaleW));
    logInfo("scaleH: " + str(scaleH));

    int yy1;
    int yy2;
    int xx1;
    int xx2;

    int x;
    for (x = 0; x < w; x++)
    {
        xx1 = scaleW * x;
        xx2 = scaleW * (x + 1);

        int y;
        for (y = 0; y < h; y++)
        {
            yy1 = scaleH * y;
            yy2 = scaleH * (y + 1);

            deValue v = 1;

            if (rotate == 0)
            {
                v = samplePixel(src, x1 + xx1, x1 + xx2, y1 + yy1, y1 + yy2, mirrorX, mirrorY);
            }   
            if (rotate == 90)
            {
                v = samplePixel(src, ws - 1 - yy2 - y1, ws - 1 - yy1 - y1, xx1 + x1, xx2 + x1,  mirrorX, mirrorY);
            }   
            if (rotate == 180)
            {
                v = samplePixel(src, ws - 1 - xx2 - x1, ws - 1 - xx1 - x1, hs - 1 - yy2 - y1, hs - 1 - yy1 - y1, mirrorX, mirrorY);
            }   
            if (rotate == 270)
            {
                v = samplePixel(src, y1 + yy1, y1 + yy2, hs - 1 - xx2 - x1, hs - 1 - xx1 - x1, mirrorX, mirrorY);
            }   

            dst[y * w + x] = v;
        }

    }        

    logInfo("scale channel DONE");
}


deValue deStaticImage::getAspect() const
{
    deValue aspect = size.getAspect();
    return aspect;
}

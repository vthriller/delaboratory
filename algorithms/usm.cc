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

#include "usm.h"
#include "logger.h"
#include "blur.h"
#include "blend_channel.h"
#include <cmath>
#include "image.h"
#include "color_space_utils.h"

void unsharpMask(const deValue* source, const deValue* mask, deValue* destination, int n, deValue a)
{
    int i;
    for (i = 0; i < n; i++)
    {
        deValue src = source[i];
        deValue u = src - mask[i];

        deValue d = src + a * u;

        if (d < 0)
        {
            destination[i] = 0;
        }
        else if (d > 1)
        {
            destination[i] = 1;
        }
        else
        {    
            destination[i] = d;
        }               
    }
}        

void unsharpMask(const deValue* source, const deValue* mask, deValue* destination, int n, deValue a, deValue t)
{
    int i;
    for (i = 0; i < n; i++)
    {
        deValue src = source[i];
        deValue u = src - mask[i];

        if (fabs(2 * u) >= t)
        {
            deValue d = src + a * u;

            if (d < 0)
            {
                destination[i] = 0;
            }
            else if (d > 1)
            {
                destination[i] = 1;
            }
            else
            {    
                destination[i] = d;
            }               
        }
        else
        {
            destination[i] = src;
        }
    }
}        

bool unsharpMask(const deValue* source, deValue* destination, deSize& size, deValue a, deValue r, deValue t, deBlurType type)
{
    logInfo("unsharp mask start");
    int n = size.getN();

    deValue* mask = NULL;
    try
    {
        mask = new deValue [n];
    }
    catch (std::bad_alloc)
    {
        logError("allocating memory in USM");
        if (mask)
        {
            delete [] mask;
        }
        return false;
    }

    if (mask)
    {
        logInfo("mask allocated");
    }
    else
    {
        logError("unable to allocate mask");
    }

    deValue b_t = 0.0;

    if (blurChannel(source, mask, size, r, r, type, b_t))
    {
        if (t > 0)
        {
            unsharpMask(source, mask, destination, n, a, t);
        }
        else
        {
            unsharpMask(source, mask, destination, n, a);
        }
    }

    delete [] mask;

    logInfo("unsharp mask DONE");

    return true;
}        

bool autoDodgeBurn(const deValue* source, deValue* destination, deSize& size, deValue r1, deValue r2, deValue t, bool burn)
{
    int n = size.getN();

    deValue* mask1 = NULL;
    try
    {
        mask1 = new deValue [n];
    }
    catch (std::bad_alloc)
    {
        logError("allocating memory");
        if (mask1)
        {
            delete [] mask1;
        }
        return false;
    }

    blurChannel(source, mask1, size, r1, r1, deBoxBlur, 0.0);
    int i;

    t = 1 - t;

    if (burn)
    {

        deValue d = 1.0 - t;
        deValue s = 1.0 / d;

        for (i = 0; i < n; i++)
        {
            deValue v = 1.0 - mask1[i];
            if (v < t)
            {
                v = 0.0;
            }
            else
            {
                v = s * (v - t);
                if (v < 0)
                {
                    v = 0;
                }
            }
            mask1[i] = v;
        }
    }
    else
    {
        deValue d = 1.0 - t;
        deValue s = 1.0 / d;

        for (i = 0; i < n; i++)
        {
            deValue v = mask1[i];
            if (v < t)
            {
                v = 0.0;
            }
            else
            {
                v = s * (v - t);
                if (v > 1)
                {
                    v = 1;
                }
            }
            mask1[i] = v;
        }
    }


    blurChannel(mask1, mask1, size, r2, r2, deBoxBlur, 0.0);

    deBlendMode mode = deBlendDodge;
    if (burn)
    {
        mode = deBlendBurn;
    }

    blendChannel(source, source, destination, mask1, mode, 1.0, n);

    delete [] mask1;

    return true;
}

bool shadowsHighlights(deValue r, int channel, const deImage& sourceImage, deImage& mainLayerImage, bool shadows)
{
    deSize size = mainLayerImage.getChannelSize();
    int n = size.getN();

    deValue* mask = NULL;
    try
    {
        mask = new deValue [n];
    }
    catch (std::bad_alloc)
    {
        logError("allocating memory");
        if (mask)
        {
            delete [] mask;
        }
        return false;
    }

    const deValue* source = sourceImage.startRead(channel);
    blurChannel(source, mask, size, r, r, deBoxBlur, 0.0);
    sourceImage.finishRead(channel);

    int i;
    int nc = getColorSpaceSize(sourceImage.getColorSpace());
    for (i = 0; i < nc; i++)
    {
        deValue* d = mainLayerImage.startWrite(i);
        const deValue* s = sourceImage.startRead(i);
        int j;
        if (shadows)
        {
            for (j = 0; j < n; j++)
            {
                deValue v = s[j];
                deValue m = mask[j];
                deValue r = calcBlendResult(v, m, deBlendOverlayInvert);
                if (r > v)
                {
                    v = r;
                }
                d[j] = v;
            }
        }
        else
        {
            for (j = 0; j < n; j++)
            {
                deValue v = s[j];
                deValue m = mask[j];
                deValue r = calcBlendResult(v, m, deBlendOverlayInvert);
                if (r < v)
                {
                    v = r;
                }
                d[j] = v;
            }
        }
        sourceImage.finishRead(i);
        mainLayerImage.finishWrite(i);
    }

    delete [] mask;

    return true;
}    

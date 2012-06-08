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

#include "histogram.h"
#include <iostream>
#include "logger.h"

deHistogram::deHistogram(int _size)
:size(_size)
{
    bars.reserve(size);
    clear();
}

deHistogram::~deHistogram()
{
}

void deHistogram::clear()
{
    int i;
    for (i = 0; i < size; i++)
    {
        bars[i] = 0 ;
    }
}

void deHistogram::put(deValue value)
{
    if ((value < 0) || (value > 1))
    {
        return;
    }

    int bar = (size-1) * value;

    bars[bar] ++;    
}


int deHistogram::getMax() const
{
    int i;
    int m = 0;
    for (i = 0; i < size; i++)
    {
        if (bars[i] > m)
        {
            m = bars[i];
        }
    }
    return m;
}

int deHistogram::get(int bar) const
{
    return bars[bar];
}

int deHistogram::getSize() const
{
    return size;
}

void deHistogram::calc(const deValue* pixels, int n)
{
    static int counter = 0;

    int j;
    int scale = size - 1;
    for (j = 0; j < n; j++)
    {
        deValue value = pixels[j];
        counter++;
        int bar = scale * value;
        if ((bar >= 0) && (bar < size))
        {
            bars[bar] ++;    
        }            
    }

}

bool deHistogram::render(unsigned char* data, int sizeW, int sizeH, unsigned char g1, unsigned char g2, int margin)
{
    int mm = getMax();
    if (mm <= 0)
    {
        return false;
    }

//    mm *= 0.1;

    unsigned char g3 = 230;

    int x;
    int y;
    for (x = 0; x < sizeW; x++)
    {
        int xx = x - margin;

        if ((xx >= 0) && (xx < size))
        {
            int hh = get(xx);
            int h = sizeH * hh / mm;
            if (h > sizeH)
            {
                h = sizeH;
            }
            for (y = 0; y < sizeH - h; y++)
            {
                data[3*(y*sizeW+x)] = g1;
                data[3*(y*sizeW+x)+1] = g1;
                data[3*(y*sizeW+x)+2] = g1;
            }
            for (y = sizeH - h; y < sizeH; y++)
            {
                data[3*(y*sizeW+x)] = g2;
                data[3*(y*sizeW+x)+1] = g2;
                data[3*(y*sizeW+x)+2] = g2;
            }
        }
        else
        {
            for (y = 0; y < sizeH; y++)
            {
                data[3*(y*sizeW+x)] = g3;
                data[3*(y*sizeW+x)+1] = g3;
                data[3*(y*sizeW+x)+2] = g3;
            }
        }
    }

    return true;
}    

void deHistogram::calcLevels(deValue margin1, deValue margin2, deValue& min, deValue& middle, deValue& max)
{
    int i;

    deValue sum = 0.0;
    for (i = 0; i < size; i++)
    {
        deValue b = bars[i];
        sum += b;
    }       

    deValue left = -1;
    deValue right = -1;
    deValue leftMiddle = -1;
    deValue rightMiddle = -1;

    deValue s = 0.0;
    for (i = 0; i < size; i++)
    {
        deValue b = bars[i];
        s += b;

        deValue p = s / sum;
        if ((p >= 1.0 - margin1) && (right < 0))
        {
            right = (deValue) i / (size - 1);
        }
        if ((p >= 1.0 - margin2) && (rightMiddle < 0))
        {
            rightMiddle = (deValue) i / (size - 1);
        }
    }

    s = 0.0;
    for (i = size - 1; i >= 0; i--)
    {
        deValue b = bars[i];
        s += b;

        deValue p = s / sum;
        if ((p >= 1.0 - margin1) && (left < 0))
        {
            left = (deValue) i / (size - 1);
        }
        if ((p >= 1.0 - margin2) && (leftMiddle < 0))
        {
            leftMiddle = (deValue) i / (size - 1);
        }
    }

    min = left;
    max = right;
    middle = (leftMiddle + rightMiddle) / 2.0;

}


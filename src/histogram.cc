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
#include "channel.h"

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

void deHistogram::calc(const deChannel* channel, int n)
{
    if (!channel)
    {
        return;
    }

    int j;
    for (j = 0; j < n; j++)
    {
        deValue value = channel->getValue(j);
        int bar = (size-1) * value;
        if ((bar >=0) && (bar < size))
        {
            bars[bar] ++;    
        }            
    }
}

bool deHistogram::render(unsigned char* data, int sizeW, int sizeH, unsigned char g1, unsigned char g2)
{
    int mm = getMax();
    if (mm <= 0)
    {
        return false;
    }

    int maxH = sizeH - 10;

    int x;
    int y;
    for (x = 0; x < sizeW; x++)
    {
        int hh = get(x);
        int hp = hh;
        int hn = hh;
        if (x > 0)
        {
            hp = get(x - 1);
        }
        if (x < sizeW - 1)
        {
            hn = get(x + 1);
        }
        hh = (hh + hp + hn) / 3;
        int h = maxH * hh / mm;
        if (h > maxH)
        {
            h = maxH;
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

    return true;
}    

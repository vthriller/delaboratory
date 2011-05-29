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
#include "exception.h"
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
        throw deException("value in histogram out of range");
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

void deHistogram::calc(const deBaseChannel* channel, int n)
{
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


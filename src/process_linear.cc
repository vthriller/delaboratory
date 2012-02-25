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

#include "process_linear.h"

void processLinear(const deValue* src, deValue* dst,  int n, deValue min, deValue max, bool invert)
{
    if ((min == 0) && (max == 1) && (src==dst))
    {
        return;
    }

    if (max < min)
    {
        invert = !invert;
        deValue m = min;
        min = max;
        max = m;
    }

    deValue left;
    deValue right;
    deValue a = 0;

    if (min < max)
    {
        a = 1.0 / (max - min);
    }

    if (invert)
    {
        left = 1.0;
        right = 0.0;
        a = -a;
    }
    else
    {
        left = 0.0;
        right = 1.0;
    }

    int i;

    for (i = 0; i < n; i++)
    {
        deValue v = src[i];
        if (v <= min)
        {
            dst[i] = left;               
        } else if (v >= max)
        {
            dst[i] = right;              
        }
        else
        {
            dst[i] = left + a * (v - min);
        }
    }
}

void shiftChannel(const deValue* src, deValue* dst, deValue shift, int n)
{
    int i;

    for (i = 0; i < n; i++)
    {
        deValue v = src[i];
        v += shift;
        if (v < 0.0)
        {
            v += 1.0;
        }
        if (v > 1.0)
        {
            v -= 1.0;
        }
        dst[i] = v;
    }
}

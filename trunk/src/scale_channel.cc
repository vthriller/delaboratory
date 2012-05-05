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

#include "scale_channel.h"
#include "channel.h"
#include <iostream>
#include <cassert>

void scaleChannel(const deValue* src, deValue* dst, int x1, int y1, int x2, int y2, int w, int h, int ws)
{
    if (w <= 0)
    {
        return;
    }
    if (h <= 0)
    {
        return;
    }

    deValue ww = x2 - x1;
    deValue scaleW = ww / w;

    deValue hh = y2 - y1;
    deValue scaleH = hh / h;

    int x;
    for (x = 0; x < w; x++)
    {
        int y;

        int xx1 = x1 + scaleW * x;
        int xx2 = x1 + scaleW * (x + 1);
        if (xx2 >= x2)
        {
            xx2 = x2 - 1;
        }

        for (y = 0; y < h; y++)
        {
            int yy1 = y1 + scaleH * y;
            int yy2 = y1 + scaleH * (y + 1);
            if (yy2 >= y2)
            {
                yy2 = y2 - 1;
            }

            deValue value = 0.0;
            int n = 0;

            int x0;
            int y0;

            for (x0 = xx1; x0 <= xx2; x0++)
            {
                for (y0 = yy1; y0 <= yy2; y0++)
                {
                    value += src[x0 + y0 * ws];
                    n++;
                }
            }

            if (n > 0)
            {
               deValue v = value / n;
               dst[y*w+x] = v;
            }           
        }

    }        



}

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

void scaleChannel(deValue* src, deValue* dst, int maxx, int ox, deValue scaleW, int maxy, int oy, deValue scaleH, int ws, int hs, int wd)
{
    if (ws < 1)
    {
        return;
    }
    if (hs < 1)
    {
        return;
    }

    int x;
    for (x = 0; x < maxx; x++)
    {
        int x1 = ox + scaleW * x;
        int x2 = ox + scaleW * (x + 1);
        if (x2 == x1)
        {
            x2++;
        }
        if (x2 > ws)
        {
            x2 = ws;
        }

        int y;

        for (y = 0; y < maxy; y++)
        {
            int y1 = oy + scaleH * y;
            int y2 = oy + scaleH * (y + 1);
            if (y2 == y1)
            {
                y2++;
            }

            if (y2 > hs)
            {
                y2 = hs;
            }

            int x0;
            int y0;

            deValue value = 0.0;
            int n = 0;

            for (x0 = x1; x0 < x2; x0++)
            {
                for (y0 = y1; y0 < y2; y0++)
                {
                    value += src[x0 + y0 * ws];
                    n++;
                }
            }

            deValue v = value / n;

            dst[y*wd+x] = v;
        }
    }
}


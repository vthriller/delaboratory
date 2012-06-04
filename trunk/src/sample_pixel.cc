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

#include "sample_pixel.h"

deValue samplePixel(const deValue* src, int xx1, int xx2, int yy1, int yy2, bool mirrorX, bool mirrorY, int ws, int hs)
{
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
        return 0.0;
    }

    return value / n;
}


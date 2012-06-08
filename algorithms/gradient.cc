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

#include "gradient.h"
#include <cmath>
#include <cassert>
#include "logger.h"
#include "str.h"

void gradientChannel(deValue* destination, deSize size, deValue cx, deValue cy, deValue r, deValue a)
{
    int w = size.getW();
    int h = size.getH();

    if (w == 0)
    {
        return;
    }

    if (h == 0)
    {
        return;
    }

    deValue ww = w / 2.0;
    deValue hh = h / 2.0;

    int i;
    int j;

    int p = 0;

    deValue angle = 2 * M_PI * (a / 360.0);
    deValue vx = sin (angle);
    deValue vy = cos (angle);

    deValue div;

    if (r > 0)
    {
        div = 1.0 / (r + 0.1);
    }
    else
    {
        div = 1.0 / (r - 0.1);
    }

    for (i = 0; i < h; i++)
    {
        deValue y = (i - hh) / hh;

        deValue yy = y - cy;

        for (j = 0; j < w; j++)
        {
            deValue x = (j - ww) / ww;

            deValue xx = x - cx;

            deValue rr = (yy * vy + xx * vx) * div;

            rr = 1.0 - rr;

            if (rr < 0)
            {
                rr = 0;
            }
            if (rr > 1)
            {
                rr = 1;
            }
            
            destination[p] = rr;
            p++;
        }
    }
}

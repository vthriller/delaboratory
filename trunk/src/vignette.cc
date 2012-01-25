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

#include "vignette.h"
#include <cmath>
#include <cassert>
#include "logger.h"
#include "str.h"

void vignetteChannel(deValue* destination, deSize size, deValue centerX, deValue centerY, deValue radiusX, deValue radiusY, deValue light)
{
    logMessage("vignette channel cx: " + str(centerX) + " cy: " + str(centerY) + " rx: " + str(radiusX) + " ry: " + str(radiusY) + " l: " + str(light));

    if (radiusX <= 0.0)
    {
        return;
    }
    if (radiusY <= 0.0)
    {
        return;
    }

    //assert(source);
    assert(destination);

    int w = size.getW();
    int h = size.getH();

    deValue ww = w / 2.0;
    deValue hh = h / 2.0;

    if (w == 0)
    {
        return;
    }

    if (h == 0)
    {
        return;
    }

    assert(w > 0);
    assert(h > 0);
    //assert(radius > 0);

    deValue s = 3;

    radiusX *= s;
    radiusY *= s;

    int i;
    int j;

    int p = 0;

    logMessage("before vignette fill");

    for (i = 0; i < h; i++)
    {
        deValue y = (i - hh) / hh;

        y = y - centerY;

        y /= radiusY;

        for (j = 0; j < w; j++)
        {
            deValue x = (j - ww) / ww;

            x = x - centerX;
            x /= radiusX;

            deValue r = sqrt(x * x + y * y);
            deValue v = light;
            if (r > 0)
            {
                v = light - r;
            }
            if (v < 0)
            {
                v = 0;
            }
            if (v > 1)
            {
                v = 1;
            }
            destination[p] = v;
            p++;
        }
    }

    logMessage("after vignette fill");

}


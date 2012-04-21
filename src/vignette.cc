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

deEllipse::deEllipse(deValue _centerX, deValue _centerY, deValue _radiusX, deValue _radiusY)
:centerX(_centerX),
 centerY(_centerY),
 radiusX(_radiusX),
 radiusY(_radiusY)
{
}

deEllipse::~deEllipse()
{
}

deValue deEllipse::processX(deValue x) const
{
    x = x - centerX;

    x /= radiusX;

    return x;
}

deValue deEllipse::processY(deValue y) const
{
    y = y - centerY;

    y /= radiusY;

    return y;
}

bool deEllipse::isValid() const
{
    if (radiusX <= 0.0)
    {
        return false;
    }
    if (radiusY <= 0.0)
    {
        return false;
    }

    return true;
}

void vignetteChannel(deValue* destination, deSize size, deEllipse ellipse, deValue light, deValue darkness, deValue spot)
{
    if (!ellipse.isValid())
    {
        return;
    }

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

    for (i = 0; i < h; i++)
    {
        deValue y = (i - hh) / hh;

        y = ellipse.processY(y);

        for (j = 0; j < w; j++)
        {
            deValue x = (j - ww) / ww;

            x = ellipse.processX(x);

            deValue rr = x * x + y * y;

            deValue r = 0.5 * sqrt(rr);

            deValue v = 1.0 - r;

            if (v < 0)
            {
                v = 0;
            }

            if (v > 1 - spot)
            {
                deValue over = v - (1 - spot);
                v += 0.5 * over;
            }

            if (v > 1)
            {
                v = 1;
            }

            deValue vv = darkness + (light - darkness) * v;
            if (vv < 0)
            {
                vv = 0;
            }
            if (vv > 1)
            {
                vv = 1;
            }

            destination[p] = vv;
            p++;
        }
    }

}

void vignetteChannel(deValue* destination, deSize size, deEllipse ellipse1, deEllipse ellipse2, deValue light, deValue darkness, deValue spot)
{
    if (!ellipse1.isValid())
    {
        return;
    }

    if (!ellipse2.isValid())
    {
        return;
    }


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

    for (i = 0; i < h; i++)
    {
        deValue y = (i - hh) / hh;

        deValue y1 = ellipse1.processY(y);
        deValue y2 = ellipse2.processY(y);

        for (j = 0; j < w; j++)
        {
            deValue x = (j - ww) / ww;

            deValue x1 = ellipse1.processX(x);
            deValue x2 = ellipse2.processX(x);

            deValue r1 = sqrt(x1 * x1 + y1 * y1);
            deValue r2 = sqrt(x2 * x2 + y2 * y2);

            deValue v1 = 1.0 - r1 + spot;
            if (v1 < 0)
            {
                v1 = 0;
            }
            if (v1 > 1)
            {
                v1 = 1;
            }

            deValue v2 = 1.0 - r2 + spot;
            if (v2 < 0)
            {
                v2 = 0;
            }
            if (v2 > 1)
            {
                v2 = 1;
            }

            deValue v = v1;
            if (v2 > v)
            {
                v = v2;
            }

            deValue vv = darkness + (light - darkness) * v;
            if (vv < 0)
            {
                vv = 0;
            }
            if (vv > 1)
            {
                vv = 1;
            }

            destination[p] = vv;
            p++;
        }
    }

}


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

            deValue r = 1.0;

            deValue rr = x * x + y * y;

//            if (rr <= 1.0)
            {
                r = sqrt(rr);
            }                

            deValue v = 1.0 - r;

            if (v > 0)
            {
                v = sin(v * M_PI / 2);
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

deEllipse calcEllipse(deValue radX, deValue radY, deValue cenX, deValue cenY, deValue x1, deValue y1, deValue x2, deValue y2)
{
    deValue w = x2 - x1;
    deValue h = y2 - y1;

    deValue rx = radX / w;
    deValue ry = radY / h;

    // 0..1
    deValue ccx = (cenX + 1.0) / 2.0;
    deValue ccy = (cenY + 1.0) / 2.0;

    deValue cccx = (ccx - x1) / w;
    deValue cccy = (ccy - y1) / h;

    // -1..1
    deValue cx = cccx * 2.0 - 1.0;
    deValue cy = cccy * 2.0 - 1.0;

    return deEllipse(cx, cy, rx, ry);
}


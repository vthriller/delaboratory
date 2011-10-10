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

#include "cmyk.h"

void rgb2cmy(deValue r, deValue g, deValue b, deValue& c, deValue& m, deValue& y)
{
    c = 1 - r;
    m = 1 - g;
    y = 1 - b;
}

void cmy2rgb(deValue c, deValue m, deValue y, deValue& r, deValue& g, deValue& b)
{
    r = 1 - c;
    g = 1 - m;
    b = 1 - y;

    if (r < 0)
    {
        r = 0;
    }
    else if (r > 1)
    {
        r = 1;
    }
    if (g < 0)
    {
        g = 0;
    }
    else if (g > 1)
    {
        g = 1;
    }
    if (b < 0)
    {
        b = 0;
    }
    else if (b > 1)
    {
        b = 1;
    }
}

void cmy2cmyk(deValue c, deValue m, deValue y, deValue& _c, deValue& _m, deValue& _y, deValue& _k)
{
    deValue k;
    if (c < m)
    {
        if (c < y)
        {
            k = c;
        }
        else
        {
            k = y;
        }
    }
    else
    {
        if (m < y)
        {
            k = m;
        }
        else
        {
            k = y;
        }
    }

    k = k - 0.2;
    if (k < 0.0)
    {
        k = 0.0;
    }

    if (k > 0.8)
    {
        k = 0.8;
    }

    deValue kk = (1 - k);

    _k = k;
    _c = (c - k) / kk;
    _m = (m - k) / kk;
    _y = (y - k) / kk;
}

void cmyk2cmy(deValue c, deValue m, deValue y, deValue k, deValue& _c, deValue& _m, deValue& _y)
{
    deValue  kk = 1 - k;

    _c = c * kk + k;
    _m = m * kk + k;
    _y = y * kk + k;
}

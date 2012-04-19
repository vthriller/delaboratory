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

#include "apply_luminance_color.h"
#include "rgb2xyz2lab.h"
#include "cmyk.h"
#include "conversion_functions.h"

void applyLuminanceRGB(deValue r1, deValue g1, deValue b1, deValue r2, deValue g2, deValue b2, deValue &r, deValue &g, deValue &b)
{
    deValue x1;
    deValue y1;
    deValue z1;
    rgb2xyz(r1, g1, b1, x1, y1, z1);

    deValue lab_l1;
    deValue lab_a1;
    deValue lab_b1;
    xyz2lab(x1, y1, z1, lab_l1, lab_a1, lab_b1);

    deValue x2;
    deValue y2;
    deValue z2;
    rgb2xyz(r2, g2, b2, x2, y2, z2);

    deValue lab_l2;
    deValue lab_a2;
    deValue lab_b2;
    xyz2lab(x2, y2, z2, lab_l2, lab_a2, lab_b2);

    deValue x;
    deValue y;
    deValue z;
    lab2xyz(lab_l2, lab_a1, lab_b1, x, y, z);

    xyz2rgb(x, y, z, r, g, b);

    if (r < 0.0)
    {
        r = 0.0;
    }
    if (r > 1.0)
    {
        r = 1.0;
    }
    if (g < 0.0)
    {
        g = 0.0;
    }
    if (g > 1.0)
    {
        g = 1.0;
    }
    if (b < 0.0)
    {
        b = 0.0;
    }
    if (b > 1.0)
    {
        b = 1.0;
    }
}

void applyLuminanceProPhoto(deValue r1, deValue g1, deValue b1, deValue r2, deValue g2, deValue b2, deValue &r, deValue &g, deValue &b)
{
    deValue x1;
    deValue y1;
    deValue z1;
    prophoto2xyz(r1, g1, b1, x1, y1, z1);

    deValue lab_l1;
    deValue lab_a1;
    deValue lab_b1;
    xyz2lab(x1, y1, z1, lab_l1, lab_a1, lab_b1);

    deValue x2;
    deValue y2;
    deValue z2;
    prophoto2xyz(r2, g2, b2, x2, y2, z2);

    deValue lab_l2;
    deValue lab_a2;
    deValue lab_b2;
    xyz2lab(x2, y2, z2, lab_l2, lab_a2, lab_b2);

    deValue x;
    deValue y;
    deValue z;
    lab2xyz(lab_l2, lab_a1, lab_b1, x, y, z);

    xyz2prophoto(x, y, z, r, g, b);

    if (r < 0.0)
    {
        r = 0.0;
    }
    if (r > 1.0)
    {
        r = 1.0;
    }
    if (g < 0.0)
    {
        g = 0.0;
    }
    if (g > 1.0)
    {
        g = 1.0;
    }
    if (b < 0.0)
    {
        b = 0.0;
    }
    if (b > 1.0)
    {
        b = 1.0;
    }
}

void applyLuminanceCMYK(deValue c1, deValue m1, deValue y1, deValue k1, deValue c2, deValue m2, deValue y2, deValue k2, deValue &c, deValue &m, deValue &y, deValue& k)
{
    deValue lab_l1;
    deValue lab_a1;
    deValue lab_b1;
    cmyk2lab(c1, m1, y1, k1, lab_l1, lab_a1, lab_b1);

    deValue lab_l2;
    deValue lab_a2;
    deValue lab_b2;
    cmyk2lab(c2, m2, y2, k2, lab_l2, lab_a2, lab_b2);

    lab2cmyk(lab_l2, lab_a1, lab_b1, c, m, y, k);
}

void applyColor(deValue r1, deValue g1, deValue b1, deValue r2, deValue g2, deValue b2, deValue &r, deValue &g, deValue &b)
{
    deValue x1;
    deValue y1;
    deValue z1;
    rgb2xyz(r1, g1, b1, x1, y1, z1);

    deValue lab_l1;
    deValue lab_a1;
    deValue lab_b1;
    xyz2lab(x1, y1, z1, lab_l1, lab_a1, lab_b1);

    deValue x2;
    deValue y2;
    deValue z2;
    rgb2xyz(r2, g2, b2, x2, y2, z2);

    deValue lab_l2;
    deValue lab_a2;
    deValue lab_b2;
    xyz2lab(x2, y2, z2, lab_l2, lab_a2, lab_b2);

    deValue x;
    deValue y;
    deValue z;
    lab2xyz(lab_l1, lab_a2, lab_b2, x, y, z);

    xyz2rgb(x, y, z, r, g, b);

    if (r < 0.0)
    {
        r = 0.0;
    }
    if (r > 1.0)
    {
        r = 1.0;
    }
    if (g < 0.0)
    {
        g = 0.0;
    }
    if (g > 1.0)
    {
        g = 1.0;
    }
    if (b < 0.0)
    {
        b = 0.0;
    }
    if (b > 1.0)
    {
        b = 1.0;
    }
}


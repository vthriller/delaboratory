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

#include "conversion_functions.h"
#include "rgb2xyz2lab.h"
#include "hsl_hsv.h"
#include "cmyk.h"
#include "bw.h"

void copy(deValue s1, deValue s2, deValue s3, deValue& d1, deValue& d2, deValue& d3)
{
    d1 = s1;
    d2 = s2;
    d3 = s3;
}

void rgb2lab(deValue s1, deValue s2, deValue s3, deValue& d1, deValue& d2, deValue& d3)
{
    deValue x;
    deValue y;
    deValue z;

    rgb2xyz(s1, s2, s3, x, y, z);

    xyz2lab(x, y, z, d1, d2, d3);
}

void lab2rgb(deValue s1, deValue s2, deValue s3, deValue& d1, deValue& d2, deValue& d3)
{
    deValue x;
    deValue y;
    deValue z;

    lab2xyz(s1, s2, s3, x, y, z);

    xyz2rgb(x, y, z, d1, d2, d3);


}

void cmyk2rgb(deValue s1, deValue s2, deValue s3, deValue s4, deValue& d1, deValue& d2, deValue& d3)
{
    deValue c;
    deValue m;
    deValue y;

    cmyk2cmy(s1, s2, s3, s4, c, m, y);

    cmy2rgb(c, m, y, d1, d2, d3);

}

void cmyk2hsv(deValue s1, deValue s2, deValue s3, deValue s4, deValue& d1, deValue& d2, deValue& d3)
{
    deValue c;
    deValue m;
    deValue y;

    cmyk2cmy(s1, s2, s3, s4, c, m, y);

    deValue r;
    deValue g;
    deValue b;

    cmy2rgb(c, m, y, r, g, b);

    rgb2hsv(r, g, b, d1, d2, d3);
}   

void cmyk2lab(deValue s1, deValue s2, deValue s3, deValue s4, deValue& d1, deValue& d2, deValue& d3)
{
    deValue c;
    deValue m;
    deValue y;

    cmyk2cmy(s1, s2, s3, s4, c, m, y);

    deValue r;
    deValue g;
    deValue b;

    cmy2rgb(c, m, y, r, g, b);

    rgb2lab(r, g, b, d1, d2, d3);
}   

void rgb2cmyk(deValue s1, deValue s2, deValue s3, deValue& d1, deValue& d2, deValue& d3, deValue& d4)
{
    deValue c;
    deValue m;
    deValue y;

    rgb2cmy(s1, s2, s3, c, m, y);

    cmy2cmyk(c, m, y, d1, d2, d3, d4);
}    

void lab2cmyk(deValue s1, deValue s2, deValue s3, deValue& d1, deValue& d2, deValue& d3, deValue& d4)
{
    deValue x;
    deValue y;
    deValue z;

    lab2xyz(s1, s2, s3, x, y, z);

    deValue r;
    deValue g;
    deValue b;

    xyz2rgb(x, y, z, r, g, b);

    deValue _c;
    deValue _m;
    deValue _y;

    rgb2cmy(r, g, b, _c, _m, _y);

    cmy2cmyk(_c, _m, _y, d1, d2, d3, d4);
}    



void rgb2lch(deValue s1, deValue s2, deValue s3, deValue& d1, deValue& d2, deValue& d3)
{
    deValue l;
    deValue a;
    deValue b;

    rgb2lab(s1, s2, s3, l, a, b);

    lab2lch(l, a, b, d1, d2, d3);
}

void lch2rgb(deValue s1, deValue s2, deValue s3, deValue& d1, deValue& d2, deValue& d3)
{
    deValue l;
    deValue a;
    deValue b;

    lch2lab(s1, s2, s3, l, a, b);

    lab2rgb(l, a, b, d1, d2, d3);
}

deConversion4x3 getConversion4x3(deColorSpace s, deColorSpace d)
{
    if ((s == deColorSpaceCMYK) && (d == deColorSpaceRGB))
    {
        return cmyk2rgb;
    }

    if ((s == deColorSpaceCMYK) && (d == deColorSpaceCMY))
    {
        return cmyk2cmy;
    }

    if ((s == deColorSpaceCMYK) && (d == deColorSpaceHSV))
    {
        return cmyk2hsv;
    }

    if ((s == deColorSpaceCMYK) && (d == deColorSpaceLAB))
    {
        return cmyk2lab;
    }

    return NULL;
}

deConversion3x4 getConversion3x4(deColorSpace s, deColorSpace d)
{
    if ((s == deColorSpaceRGB) && (d == deColorSpaceCMYK))
    {
        return rgb2cmyk;
    }

    if ((s == deColorSpaceCMY) && (d == deColorSpaceCMYK))
    {
        return cmy2cmyk;
    }

    if ((s == deColorSpaceLAB) && (d == deColorSpaceCMYK))
    {
        return lab2cmyk;
    }

    return NULL;
}

deConversion3x1 getConversion3x1(deColorSpace s, deColorSpace d)
{
    if ((s == deColorSpaceRGB) && (d == deColorSpaceBW))
    {
        return rgb2bw;
    }

    return NULL;
}

deConversion1x3 getConversion1x3(deColorSpace s, deColorSpace d)
{
    if ((s == deColorSpaceBW) && (d == deColorSpaceRGB))
    {
        return bw2rgb;
    }

    return NULL;
}

deConversion3x3 getConversion3x3(deColorSpace s, deColorSpace d)
{
    if (s == d)
    {
        return copy;
    }

    if ((s == deColorSpaceRGB) && (d == deColorSpaceLAB))
    {
        return rgb2lab;
    }

    if ((s == deColorSpaceRGB) && (d == deColorSpaceXYZ))
    {
        return rgb2xyz;
    }

    if ((s == deColorSpaceXYZ) && (d == deColorSpaceLAB))
    {
        return xyz2lab;
    }

    if ((s == deColorSpaceRGB) && (d == deColorSpaceLCH))
    {
        return rgb2lch;
    }

    if ((s == deColorSpaceLAB) && (d == deColorSpaceLCH))
    {
        return lab2lch;
    }

    if ((s == deColorSpaceLCH) && (d == deColorSpaceLAB))
    {
        return lch2lab;
    }

    if ((s == deColorSpaceLAB) && (d == deColorSpaceRGB))
    {
        return lab2rgb;
    }

    if ((s == deColorSpaceLAB) && (d == deColorSpaceXYZ))
    {
        return lab2xyz;
    }

    if ((s == deColorSpaceXYZ) && (d == deColorSpaceRGB))
    {
        return xyz2rgb;
    }

    if ((s == deColorSpaceLCH) && (d == deColorSpaceRGB))
    {
        return lch2rgb;
    }

    if ((s == deColorSpaceRGB) && (d == deColorSpaceHSL))
    {
        return rgb2hsl;
    }

    if ((s == deColorSpaceHSL) && (d == deColorSpaceRGB))
    {
        return hsl2rgb;
    }

    if ((s == deColorSpaceRGB) && (d == deColorSpaceHSV))
    {
        return rgb2hsv;
    }

    if ((s == deColorSpaceHSV) && (d == deColorSpaceRGB))
    {
        return hsv2rgb;
    }

    if ((s == deColorSpaceRGB) && (d == deColorSpaceCMY))
    {
        return rgb2cmy;
    }

    if ((s == deColorSpaceCMY) && (d == deColorSpaceRGB))
    {
        return cmy2rgb;
    }

    return NULL;
}

bool checkConversion(deColorSpace currentColorSpace, deColorSpace colorSpace)
{
    if (getConversion3x3(currentColorSpace, colorSpace))
    {
        return true;
    }

    if (getConversion4x3(currentColorSpace, colorSpace))
    {
        return true;
    }

    if (getConversion3x4(currentColorSpace, colorSpace))
    {
        return true;
    }

    if (getConversion1x3(currentColorSpace, colorSpace))
    {
        return true;
    }

    if (getConversion3x1(currentColorSpace, colorSpace))
    {
        return true;
    }

    return false;
}


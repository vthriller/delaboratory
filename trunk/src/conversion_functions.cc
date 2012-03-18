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

void prophoto2lab(deValue s1, deValue s2, deValue s3, deValue& d1, deValue& d2, deValue& d3)
{
    deValue x;
    deValue y;
    deValue z;

    prophoto2xyz(s1, s2, s3, x, y, z);

    xyz2lab(x, y, z, d1, d2, d3);
}

void prophoto2lch(deValue s1, deValue s2, deValue s3, deValue& d1, deValue& d2, deValue& d3)
{
    deValue x;
    deValue y;
    deValue z;

    prophoto2xyz(s1, s2, s3, x, y, z);

    deValue l;
    deValue a;
    deValue b;

    xyz2lab(x, y, z, l, a, b);

    lab2lch(l, a, b, d1, d2, d3);
}


void lab2rgb(deValue s1, deValue s2, deValue s3, deValue& d1, deValue& d2, deValue& d3)
{
    deValue x;
    deValue y;
    deValue z;

    lab2xyz(s1, s2, s3, x, y, z);

    xyz2rgb(x, y, z, d1, d2, d3);
}

void lab2prophoto(deValue s1, deValue s2, deValue s3, deValue& d1, deValue& d2, deValue& d3)
{
    deValue x;
    deValue y;
    deValue z;

    lab2xyz(s1, s2, s3, x, y, z);

    xyz2prophoto(x, y, z, d1, d2, d3);
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

void lab2hsl(deValue s1, deValue s2, deValue s3, deValue& d1, deValue& d2, deValue& d3)
{
    deValue r;
    deValue g;
    deValue b;

    lab2rgb(s1, s2, s3, r, g, b);

    rgb2hsl(r, g, b, d1, d2, d3);
}   

void lab2hsv(deValue s1, deValue s2, deValue s3, deValue& d1, deValue& d2, deValue& d3)
{
    deValue r;
    deValue g;
    deValue b;

    lab2rgb(s1, s2, s3, r, g, b);

    rgb2hsv(r, g, b, d1, d2, d3);
}   

void lch2hsv(deValue s1, deValue s2, deValue s3, deValue& d1, deValue& d2, deValue& d3)
{
    deValue l;
    deValue a;
    deValue b;

    lch2lab(s1, s2, s3, l, a, b);

    lab2hsv(l, a, b, d1, d2, d3);
}   

void lch2hsl(deValue s1, deValue s2, deValue s3, deValue& d1, deValue& d2, deValue& d3)
{
    deValue l;
    deValue a;
    deValue b;

    lch2lab(s1, s2, s3, l, a, b);

    lab2hsl(l, a, b, d1, d2, d3);
}   


void cmyk2hsl(deValue s1, deValue s2, deValue s3, deValue s4, deValue& d1, deValue& d2, deValue& d3)
{
    deValue c;
    deValue m;
    deValue y;

    cmyk2cmy(s1, s2, s3, s4, c, m, y);

    deValue r;
    deValue g;
    deValue b;

    cmy2rgb(c, m, y, r, g, b);

    rgb2hsl(r, g, b, d1, d2, d3);
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

void cmyk2lch(deValue s1, deValue s2, deValue s3, deValue s4, deValue& d1, deValue& d2, deValue& d3)
{
    deValue l;
    deValue a;
    deValue b;

    cmyk2lab(s1, s2, s3, s4, l, a, b);

    lab2lch(l, a, b, d1, d2, d3);
}   

void rgb2lch(deValue s1, deValue s2, deValue s3, deValue& d1, deValue& d2, deValue& d3)
{
    deValue l;
    deValue a;
    deValue b;

    rgb2lab(s1, s2, s3, l, a, b);

    lab2lch(l, a, b, d1, d2, d3);
}

void rgb2cmyk(deValue s1, deValue s2, deValue s3, deValue& d1, deValue& d2, deValue& d3, deValue& d4)
{
    deValue c;
    deValue m;
    deValue y;

    rgb2cmy(s1, s2, s3, c, m, y);

    cmy2cmyk(c, m, y, d1, d2, d3, d4);
}    

void hsv2cmyk(deValue s1, deValue s2, deValue s3, deValue& d1, deValue& d2, deValue& d3, deValue& d4)
{
    deValue r;
    deValue g;
    deValue b;

    hsv2rgb(s1, s2, s3, r, g, b);

    rgb2cmyk(r, g, b, d1, d2, d3, d4);
}    

void hsl2cmyk(deValue s1, deValue s2, deValue s3, deValue& d1, deValue& d2, deValue& d3, deValue& d4)
{
    deValue r;
    deValue g;
    deValue b;

    hsl2rgb(s1, s2, s3, r, g, b);

    rgb2cmyk(r, g, b, d1, d2, d3, d4);
}    

void prophoto2cmyk(deValue s1, deValue s2, deValue s3, deValue& d1, deValue& d2, deValue& d3, deValue& d4)
{
    deValue r;
    deValue g;
    deValue b;

    prophoto2rgb(s1, s2, s3, r, g, b);

    deValue c;
    deValue m;
    deValue y;

    rgb2cmy(r, g, b, c, m, y);

    cmy2cmyk(c, m, y, d1, d2, d3, d4);
}    

void prophoto2hsv(deValue s1, deValue s2, deValue s3, deValue& d1, deValue& d2, deValue& d3)
{
    deValue r;
    deValue g;
    deValue b;

    prophoto2rgb(s1, s2, s3, r, g, b);

    rgb2hsv(r, g, b, d1, d2, d3);
}    

void prophoto2hsl(deValue s1, deValue s2, deValue s3, deValue& d1, deValue& d2, deValue& d3)
{
    deValue r;
    deValue g;
    deValue b;

    prophoto2rgb(s1, s2, s3, r, g, b);

    rgb2hsl(r, g, b, d1, d2, d3);
}    


void bw2cmyk(deValue s1, deValue& d1, deValue& d2, deValue& d3, deValue& d4)
{
    deValue r;
    deValue g;
    deValue b;

    bw2rgb(s1, r, g, b);

    rgb2cmyk(r, g, b, d1, d2, d3, d4);
}    

void bw2lab(deValue s1, deValue& d1, deValue& d2, deValue& d3)
{
    deValue r;
    deValue g;
    deValue b;

    bw2rgb(s1, r, g, b);

    rgb2lab(r, g, b, d1, d2, d3);
}    

void lab2bw(deValue s1, deValue s2, deValue s3, deValue& d)
{
    deValue r;
    deValue g;
    deValue b;

    lab2rgb(s1, s2, s3, r, g, b);

    rgb2bw(r, g, b, d);
}    

void cmyk2bw(deValue s1, deValue s2, deValue s3, deValue s4, deValue& d)
{
    deValue r;
    deValue g;
    deValue b;

    cmyk2rgb(s1, s2, s3, s4, r, g, b);

    rgb2bw(r, g, b, d);
}    

void hsv2bw(deValue s1, deValue s2, deValue s3, deValue& d)
{
    deValue r;
    deValue g;
    deValue b;

    hsv2rgb(s1, s2, s3, r, g, b);

    rgb2bw(r, g, b, d);
}    

void hsl2bw(deValue s1, deValue s2, deValue s3, deValue& d)
{
    deValue r;
    deValue g;
    deValue b;

    hsl2rgb(s1, s2, s3, r, g, b);

    rgb2bw(r, g, b, d);
}    

void hsl2lab(deValue s1, deValue s2, deValue s3, deValue& d1, deValue& d2, deValue& d3)
{
    deValue r;
    deValue g;
    deValue b;

    hsl2rgb(s1, s2, s3, r, g, b);

    rgb2lab(r, g, b, d1, d2, d3);
}    

void hsv2lab(deValue s1, deValue s2, deValue s3, deValue& d1, deValue& d2, deValue& d3)
{
    deValue r;
    deValue g;
    deValue b;

    hsv2rgb(s1, s2, s3, r, g, b);

    rgb2lab(r, g, b, d1, d2, d3);
}    

void hsl2hsv(deValue s1, deValue s2, deValue s3, deValue& d1, deValue& d2, deValue& d3)
{
    deValue r;
    deValue g;
    deValue b;

    hsl2rgb(s1, s2, s3, r, g, b);

    rgb2hsv(r, g, b, d1, d2, d3);
}    

void hsv2hsl(deValue s1, deValue s2, deValue s3, deValue& d1, deValue& d2, deValue& d3)
{
    deValue r;
    deValue g;
    deValue b;

    hsv2rgb(s1, s2, s3, r, g, b);

    rgb2hsl(r, g, b, d1, d2, d3);
}    

void hsl2lch(deValue s1, deValue s2, deValue s3, deValue& d1, deValue& d2, deValue& d3)
{
    deValue r;
    deValue g;
    deValue b;

    hsl2rgb(s1, s2, s3, r, g, b);

    rgb2lch(r, g, b, d1, d2, d3);
}    

void hsv2lch(deValue s1, deValue s2, deValue s3, deValue& d1, deValue& d2, deValue& d3)
{
    deValue r;
    deValue g;
    deValue b;

    hsv2rgb(s1, s2, s3, r, g, b);

    rgb2lch(r, g, b, d1, d2, d3);
}    

void lch2rgb(deValue s1, deValue s2, deValue s3, deValue& d1, deValue& d2, deValue& d3)
{
    deValue l;
    deValue a;
    deValue b;

    lch2lab(s1, s2, s3, l, a, b);

    lab2rgb(l, a, b, d1, d2, d3);
}

void lch2bw(deValue s1, deValue s2, deValue s3, deValue& d)
{
    deValue r;
    deValue g;
    deValue b;

    lch2rgb(s1, s2, s3, r, g, b);

    rgb2bw(r, g, b, d);
}    


void prophoto2bw(deValue s1, deValue s2, deValue s3, deValue& d)
{
    deValue r;
    deValue g;
    deValue b;

    prophoto2rgb(s1, s2, s3, r, g, b);

    rgb2bw(r, g, b, d);
}    

void bw2hsl(deValue s1, deValue& d1, deValue& d2, deValue& d3)
{
    deValue r;
    deValue g;
    deValue b;

    bw2rgb(s1, r, g, b);

    rgb2hsl(r, g, b, d1, d2, d3);
}    

void bw2hsv(deValue s1, deValue& d1, deValue& d2, deValue& d3)
{
    deValue r;
    deValue g;
    deValue b;

    bw2rgb(s1, r, g, b);

    rgb2hsv(r, g, b, d1, d2, d3);
}    

void bw2lch(deValue s1, deValue& d1, deValue& d2, deValue& d3)
{
    deValue r;
    deValue g;
    deValue b;

    bw2rgb(s1, r, g, b);

    rgb2lch(r, g, b, d1, d2, d3);
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

void lch2cmyk(deValue s1, deValue s2, deValue s3, deValue& d1, deValue& d2, deValue& d3, deValue& d4)
{
    deValue l;
    deValue a;
    deValue b;

    lch2lab(s1, s2, s3, l, a, b);

    lab2cmyk(l, a, b, d1, d2, d3, d4);
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

    if ((s == deColorSpaceCMYK) && (d == deColorSpaceHSL))
    {
        return cmyk2hsl;
    }

    if ((s == deColorSpaceCMYK) && (d == deColorSpaceLAB))
    {
        return cmyk2lab;
    }

    if ((s == deColorSpaceCMYK) && (d == deColorSpaceLCH))
    {
        return cmyk2lch;
    }

    return NULL;
}

deConversion3x4 getConversion3x4(deColorSpace s, deColorSpace d)
{
    if ((s == deColorSpaceRGB) && (d == deColorSpaceCMYK))
    {
        return rgb2cmyk;
    }

    if ((s == deColorSpaceHSV) && (d == deColorSpaceCMYK))
    {
        return hsv2cmyk;
    }

    if ((s == deColorSpaceHSL) && (d == deColorSpaceCMYK))
    {
        return hsl2cmyk;
    }

    if ((s == deColorSpaceProPhoto) && (d == deColorSpaceCMYK))
    {
        return prophoto2cmyk;
    }

    if ((s == deColorSpaceCMY) && (d == deColorSpaceCMYK))
    {
        return cmy2cmyk;
    }

    if ((s == deColorSpaceLAB) && (d == deColorSpaceCMYK))
    {
        return lab2cmyk;
    }

    if ((s == deColorSpaceLCH) && (d == deColorSpaceCMYK))
    {
        return lch2cmyk;
    }

    return NULL;
}

deConversion4x1 getConversion4x1(deColorSpace s, deColorSpace d)
{
    if ((s == deColorSpaceCMYK) && (d == deColorSpaceBW))
    {
        return cmyk2bw;
    }

    return false;
}    

deConversion3x1 getConversion3x1(deColorSpace s, deColorSpace d)
{
    if ((s == deColorSpaceRGB) && (d == deColorSpaceBW))
    {
        return rgb2bw;
    }

    if ((s == deColorSpaceHSV) && (d == deColorSpaceBW))
    {
        return hsv2bw;
    }

    if ((s == deColorSpaceHSL) && (d == deColorSpaceBW))
    {
        return hsl2bw;
    }

    if ((s == deColorSpaceLAB) && (d == deColorSpaceBW))
    {
        return lab2bw;
    }

    if ((s == deColorSpaceLCH) && (d == deColorSpaceBW))
    {
        return lch2bw;
    }

    if ((s == deColorSpaceProPhoto) && (d == deColorSpaceBW))
    {
        return prophoto2bw;
    }

    return NULL;
}

deConversion1x3 getConversion1x3(deColorSpace s, deColorSpace d)
{
    if ((s == deColorSpaceBW) && (d == deColorSpaceRGB))
    {
        return bw2rgb;
    }

    if ((s == deColorSpaceBW) && (d == deColorSpaceLAB))
    {
        return bw2lab;
    }

    if ((s == deColorSpaceBW) && (d == deColorSpaceHSL))
    {
        return bw2hsl;
    }

    if ((s == deColorSpaceBW) && (d == deColorSpaceHSV))
    {
        return bw2hsv;
    }

    if ((s == deColorSpaceBW) && (d == deColorSpaceLCH))
    {
        return bw2lch;
    }

    return NULL;
}

deConversion1x4 getConversion1x4(deColorSpace s, deColorSpace d)
{
    if ((s == deColorSpaceBW) && (d == deColorSpaceCMYK))
    {
        return bw2cmyk;
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

    if ((s == deColorSpaceProPhoto) && (d == deColorSpaceXYZ))
    {
        return prophoto2xyz;
    }

    if ((s == deColorSpaceProPhoto) && (d == deColorSpaceLAB))
    {
        return prophoto2lab;
    }

    if ((s == deColorSpaceProPhoto) && (d == deColorSpaceLCH))
    {
        return prophoto2lch;
    }

    if ((d == deColorSpaceProPhoto) && (s == deColorSpaceXYZ))
    {
        return xyz2prophoto;
    }

    if ((d == deColorSpaceProPhoto) && (s == deColorSpaceLAB))
    {
        return lab2prophoto;
    }

    if ((s == deColorSpaceRGB) && (d == deColorSpaceProPhoto))
    {
        return rgb2prophoto;
    }

    if ((d == deColorSpaceRGB) && (s == deColorSpaceProPhoto))
    {
        return prophoto2rgb;
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

    if ((s == deColorSpaceLAB) && (d == deColorSpaceHSL))
    {
        return lab2hsl;
    }

    if ((s == deColorSpaceLAB) && (d == deColorSpaceHSV))
    {
        return lab2hsv;
    }

    if ((s == deColorSpaceLCH) && (d == deColorSpaceHSL))
    {
        return lch2hsl;
    }

    if ((s == deColorSpaceLCH) && (d == deColorSpaceHSV))
    {
        return lch2hsv;
    }

    if ((s == deColorSpaceHSL) && (d == deColorSpaceRGB))
    {
        return hsl2rgb;
    }

    if ((s == deColorSpaceRGB) && (d == deColorSpaceHSV))
    {
        return rgb2hsv;
    }

    if ((s == deColorSpaceProPhoto) && (d == deColorSpaceHSV))
    {
        return prophoto2hsv;
    }

    if ((s == deColorSpaceProPhoto) && (d == deColorSpaceHSL))
    {
        return prophoto2hsl;
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

    if ((s == deColorSpaceHSV) && (d == deColorSpaceLAB))
    {
        return hsv2lab;
    }

    if ((s == deColorSpaceHSL) && (d == deColorSpaceLAB))
    {
        return hsl2lab;
    }

    if ((s == deColorSpaceHSV) && (d == deColorSpaceLCH))
    {
        return hsv2lch;
    }

    if ((s == deColorSpaceHSL) && (d == deColorSpaceLCH))
    {
        return hsl2lch;
    }

    if ((s == deColorSpaceHSV) && (d == deColorSpaceHSL))
    {
        return hsv2hsl;
    }

    if ((d == deColorSpaceHSV) && (s == deColorSpaceHSL))
    {
        return hsl2hsv;
    }

    return NULL;
}

bool checkConversion(deColorSpace currentColorSpace, deColorSpace colorSpace)
{
    if (getConversion3x3(currentColorSpace, colorSpace))
    {
        return true;
    }

    if (getConversion1x4(currentColorSpace, colorSpace))
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

    if (getConversion4x1(currentColorSpace, colorSpace))
    {
        return true;
    }

    return false;
}


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

#include "power.h"
#include "rgb2xyz2lab.h"
#include <cmath>
#include <cassert>
#include <iostream>

const deValue lch_hue_shift = 1.0/8.0;
//const deValue lch_hue_shift = 0;

/* almost same for conversion */
static const deValue Xn  = 0.951;
static const deValue Yn  = 1.0;
static const deValue Zn  = 1.089;

/* consts for conversion */
static const deValue c6_29 = 6.0 / 29.0;
static const deValue c29_6 = 29.0 / 6.0;
static const deValue c6_29_2 = c6_29 * c6_29;
static const deValue c6_29_3 = c6_29 * c6_29 * c6_29;
static const deValue c29_6_2 = c29_6 * c29_6;
static const deValue c4_29 = 4.0 / 29.0;

void rgb2xyz(deValue r, deValue g, deValue b, deValue &x, deValue &y, deValue& z)
{
    assert(r >= 0);
    assert(g >= 0);
    assert(b >= 0);
    assert(r <= 1);
    assert(g <= 1);
    assert(b <= 1);
    x = 0.4124 * r + 0.3576 * g + 0.1805 * b;
    y = 0.2126 * r + 0.7152 * g + 0.0722 * b;
    z = 0.0193 * r + 0.1192 * g + 0.9505 * b;

    assert(x >= 0);
    assert(y >= 0);
    assert(z >= 0);
    assert(x <= 1);
    assert(y <= 1);
    assert(z <= 1);
}

void xyz2rgb(deValue x, deValue y, deValue z, deValue &r, deValue &g, deValue& b)
{
    r =  3.2410 * x - 1.5374 * y - 0.4986 * z;
    g = -0.9692 * x + 1.8760 * y + 0.0416 * z;
    b =  0.0556 * x - 0.2040 * y + 1.0570 * z;

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

void rgb2prophoto(deValue r, deValue g, deValue b, deValue &pr, deValue &pg, deValue& pb)
{
    assert(r >= 0);
    assert(g >= 0);
    assert(b >= 0);
    assert(r <= 1);
    assert(g <= 1);
    assert(b <= 1);
    pr = 0.529 * r + 0.330 * g + 0.141 * b;
    pg = 0.098 * r + 0.873 * g + 0.028 * b;
    pb = 0.017 * r + 0.118 * g + 0.865 * b;

}

void prophoto2rgb(deValue pr, deValue pg, deValue pb, deValue &r, deValue &g, deValue& b)
{
    r =  2.034 * pr - 0.727 * pg - 0.307 * pb;
    g = -0.229 * pr + 1.232 * pg - 0.003 * pb;
    b = -0.009 * pr - 0.153 * pg + 1.162 * pb;

/*
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
    */
}

void lab2lch(deValue l, deValue a, deValue b, deValue &_l, deValue &_c, deValue& _h)
{
    _l = l;

    a = ( a - 0.5) * 200.0;
    b = ( b - 0.5) * 200.0;

    _c = sqrt(a * a + b * b);
    _h = atan2(b, a);

    _c = _c / 100.0;
    _h = (_h / ( 2 * M_PI )) - lch_hue_shift;

    if (_h < 0)
    {
        _h += 1;
    }

    if ( _c > 1)
    {
        _c = 1;
    }

    assert(_c >= 0.0);
    assert(_c <= 1.0);
    assert(_h >= 0.0);
    assert(_h <= 1.0);

}

void lch2lab(deValue l, deValue c, deValue h, deValue &_l, deValue &_a, deValue& _b)
{
    _l = l;

    c = c * 100.0;
    h = (h + lch_hue_shift) * (2 * M_PI);

    _a = c * cos(h);
    _b = c * sin(h);

    _a = _a / 200.0 + 0.5;
    _b = _b / 200.0 + 0.5;

    assert(_a >= 0.0);
    assert(_a <= 1.0);
    assert(_b >= 0.0);
    assert(_b <= 1.0);
}


void xyz2lab(deValue x, deValue y, deValue z, deValue &l, deValue &a, deValue& b)
{
    static dePower power(1.0 / 3.0, 2);

    deValue xx = x / Xn;    
    deValue yy = y / Yn;    
    deValue zz = z / Zn;    

    deValue fx;
    deValue fy;
    deValue fz;

    if (xx > c6_29_3)
    {
        fx = power.get(xx);
    }
    else
    {
        fx = 1.0 / 3.0 * c29_6_2 * xx + c4_29;
    }

    if (yy > c6_29_3)
    {
        fy = power.get(yy);
    }
    else
    {
        fy = 1.0 / 3.0 * c29_6_2 * yy + c4_29;
    }

    if (zz > c6_29_3)
    {
        fz = power.get(zz);
    }
    else
    {
        fz = 1.0 / 3.0 * c29_6_2 * zz + c4_29;
    }

    l = 116.0 * fy - 16.0;
    a = 500.0 * (fx - fy);
    b = 200.0 * (fy - fz);

    l /= 100.0;
    a += 100.0;
    b += 100.0;
    a /= 200.0;
    b /= 200.0;

    if (l < 0)
    {
        l = 0;
    }

    if (a < 0)
    {
        a = 0;
    }
    if (b < 0)
    {
        b = 0;
    }

    assert(l >= 0);
    assert(a >= 0);
    assert(b >= 0);
    assert(l <= 1);
    assert(a <= 1);
    assert(b <= 1);
}

void lab2xyz(deValue l, deValue a, deValue b, deValue &x, deValue &y, deValue& z)
{
/*
    assert( l >= 0);
    assert( l <= 1);
    assert( a >= 0);
    assert( a <= 1);
    assert( b >= 0);
    assert( b <= 1);
    */

    l *= 100.0;
    a *= 200.0;
    b *= 200.0;
    a -= 100.0;
    b -= 100.0;

    deValue ll = (l + 16.0) / 116.0;
    deValue ll_aa = ll + a / 500.0;
    deValue ll_bb = ll - b / 200.0;

    deValue ffx;
    deValue ffy;
    deValue ffz;

    if (ll > c6_29)
    {
        ffy = ll * ll * ll;     
    }
    else
    {
        ffy = 3.0 * c6_29_2 * (ll - c4_29);
    }

    if (ll_aa > c6_29)
    {
        ffx = ll_aa * ll_aa * ll_aa;     
    }
    else
    {
        ffx = 3.0 * c6_29_2 * (ll_aa - c4_29);
    }

    if (ll_bb > c6_29)
    {
        ffz = ll_bb * ll_bb * ll_bb;     
    }
    else
    {
        ffz = 3.0 * c6_29_2 * (ll_bb - c4_29);
    }

    x = Xn * ffx;
    y = Yn * ffy;
    z = Zn * ffz;

    assert(x >= 0);
    assert(y >= 0);
    assert(z >= 0);
    assert(x <= 1);
    assert(y <= 1);
    assert(z <= 1);

}

void initLAB()
{
    /* this code just calls LAB conversion to create dePower function
       calling this is not important for functionality but will affect benchmarks */

    deValue x = 0.4;
    deValue y = 0.4;
    deValue z = 0.4;
    deValue l;
    deValue a;
    deValue b;
    xyz2lab(x, y, z, l, a, b);
    
}

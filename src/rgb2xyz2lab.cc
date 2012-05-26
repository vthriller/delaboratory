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

//const deValue lch_hue_shift = 1.0/8.0;
const deValue lch_hue_shift = 0;

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

void prophoto2xyz(deValue pr, deValue pg, deValue pb, deValue &x, deValue &y, deValue& z)
{

    /*

    octave:7> z
    z =

       2.0341926  -0.7274198  -0.3067655
      -0.2288108   1.2317292  -0.0029216
      -0.0085649  -0.1532726   1.1618390

    octave:8> y
    y =

       0.412400   0.357600   0.180500
       0.212600   0.715200   0.072200
       0.019300   0.119200   0.950500

    octave:10> y * z
    ans =

       0.7555323   0.1128127   0.0821571
       0.2682055   0.7152170   0.0165769
       0.0038447  -0.0129027   1.0980591

    */

    x = 0.7555323 * pr + 0.1128127* pg + 0.0821571* pb;
    y = 0.2682055 * pr + 0.7152170* pg + 0.0165769* pb;
    z = 0.0038447 * pr -0.0129027 * pg + 1.0980591* pb;

}

void xyz2prophoto(deValue x, deValue y, deValue z, deValue &pr, deValue &pg, deValue& pb)
{
    /*

    x = [0.7555323, 0.1128127, 0.0821571; 0.2682055, 0.7152170, 0.0165769; 0.0038447, -0.0129027, 1.0980591]

    octave:2> inv(x)
    ans =

       1.403314  -0.223181  -0.101627
      -0.525984   1.481448   0.016990
      -0.011094   0.018189   0.911253

    */


    pr = 1.403314* x -0.223181* y -0.101627* z;
    pg = -0.525984* x + 1.481448* y + 0.016990* z;
    pb = -0.011094* x + 0.018189* y + 0.911253* z;

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

}

void lab2xyz(deValue l, deValue a, deValue b, deValue &x, deValue &y, deValue& z)
{

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


}


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

#include "convert_color.h"
#include <cmath>
#include <iostream>
#include <cstdlib>

deValue min(deValue a, deValue b, deValue c)
{
    if (a < b)
    {
        if (a < c)
        {
            return a;   
        }
        else
        {
            return c;
        }
    }
    else
    {
        if (b < c)
        {
            return b;   
        }
        else
        {
            return c;
        }
    }
}

deValue max(deValue a, deValue b, deValue c)
{
    if (a > b)
    {
        if (a > c)
        {
            return a;   
        }
        else
        {
            return c;
        }
    }
    else
    {
        if (b > c)
        {
            return b;   
        }
        else
        {
            return c;
        }
    }
}

void simpleCopy(deValue* values)
{
    values[4] = values[0];
    values[5] = values[1];
    values[6] = values[2];
    values[7] = values[3];
}

void inverseCopy(deValue* values)
{
    values[0] = values[4];
    values[1] = values[5];
    values[2] = values[6];
    values[3] = values[7];
}

void rgb2cmy(deValue* values)
{
    values[4] = 1.0 - values[0];
    values[5] = 1.0 - values[1];
    values[6] = 1.0 - values[2];
}

void cmy2rgb(deValue* values)
{
    values[4] = 1.0 - values[0];
    values[5] = 1.0 - values[1];
    values[6] = 1.0 - values[2];
}

void cmy2cmyk(deValue* values)
{
    // I was thinking about making k configurable, but this is float precision,
    // so it doesn't really matter if I scale this by some parameter, right?

    deValue k = min(values[0], values[1], values[2]);
    k*=0.3;

    deValue kk = (1 - k);

    values[4] = ( values[0] - k ) / kk;
    values[5] = ( values[1] - k ) / kk;
    values[6] = ( values[2] - k ) / kk;
    values[7] = k;
}

void cmyk2cmy(deValue* values)
{
    deValue k = values[3];

    deValue  kk = 1 - k;

    values[4] = values[0] * kk + k;
    values[5] = values[1] * kk + k;
    values[6] = values[2] * kk + k;
}

void rgb2cmyk(deValue* values)
{
    rgb2cmy(values);
    inverseCopy(values);
    cmy2cmyk(values);
}

void cmyk2rgb(deValue* values)
{
    cmyk2cmy(values);
    inverseCopy(values);
    cmy2rgb(values);
}

void lab2cmyk(deValue* values)
{
    lab2rgb(values);
    inverseCopy(values);
    rgb2cmy(values);
    inverseCopy(values);
    cmy2cmyk(values);
}

void cmyk2lab(deValue* values)
{
    cmyk2cmy(values);
    inverseCopy(values);
    cmy2rgb(values);
    inverseCopy(values);
    rgb2lab(values);
}

void rgb2xyz(deValue* values)
{
    deValue r = values[0];
    deValue g = values[1];
    deValue b = values[2];

    if ( r > 0.04045 ) 
        r = pow( ((r + 0.055) / 1.055), 2.4);
    else 
        r = r / 12.92;
        
    if ( g > 0.04045 ) 
        g = pow( ((g + 0.055) / 1.055), 2.4);
    else 
        g = g / 12.92;

    if ( b > 0.04045 ) 
        b = pow( ((b + 0.055) / 1.055), 2.4);
    else 
        b = b / 12.92;

    values[4] = 0.412453 * r + 0.357580 * g + 0.180423 * b;
    values[5] = 0.212671 * r + 0.715160 * g + 0.072169 * b;
    values[6] = 0.019334 * r + 0.119193 * g + 0.950227 * b; 
}

void xyz2rgb(deValue* values)
{
    deValue x = values[0];
    deValue y = values[1];
    deValue z = values[2];

    deValue r = x *  3.2406 + y * -1.5372 + z * -0.4986;
    deValue g = x * -0.9689 + y *  1.8758 + z *  0.0415;
    deValue b = x *  0.0557 + y * -0.2040 + z *  1.0570;

    if ( r > 0.0031308 )
        r = 1.055 * ( pow(r , ( 1 / 2.4 ))) - 0.055;
    else 
        r = 12.92 * r;

    if ( g > 0.0031308 ) 
        g = 1.055 * ( pow(g, ( 1 / 2.4 ))) - 0.055;
    else 
        g = 12.92 * g;

    if ( b > 0.0031308 )
        b = 1.055 * ( pow(b, ( 1 / 2.4 ))) - 0.055;
    else
        b = 12.92 * b;

    values[4] = r;        
    values[5] = g;        
    values[6] = b;        
}

void xyz2lab(deValue* values)
{
    deValue x = values[0] / 95.047 * 100.0;
    deValue y = values[1] / 100.0 * 100.0;
    deValue z = values[2] / 108.883 * 100.0;

    if ( x > 0.008856 ) 
        x = pow(x, ( 1.0/3 ));
    else 
        x = ( 7.787 * x ) + ( 16.0 / 116.0 );

    if ( y > 0.008856 ) 
        y = pow(y, ( 1.0/3 ));
    else 
        y = ( 7.787 * y ) + ( 16.0 / 116.0 );

    if ( z > 0.008856 )
        z = pow(z, ( 1.0/3 ));
    else
        z = ( 7.787 * z ) + ( 16.0 / 116.0 );

    deValue l = ( 116.0 * y ) - 16.0;
    deValue a = 500 * ( x - y );
    deValue b = 200 * ( y - z );

    values[4] = l / 100.0;
    values[5] = a / 200.0 + 0.5;
    values[6] = b / 200.0 + 0.5;
}

void lab2xyz(deValue* values)
{
    deValue l = values[0] * 100.0;
    deValue a = ( values[1] - 0.5) * 200.0;
    deValue b = ( values[2] - 0.5) * 200.0;

    deValue y = ( l + 16.0 ) / 116.0;
    deValue x = a / 500 + y;
    deValue z = y - b / 200;

    deValue yyy = y*y*y;
    if ( yyy > 0.008856 ) 
        y = yyy;
    else 
        y = ( y - 16.0 / 116.0 ) / 7.787;

    deValue xxx = x*x*x;
    if ( xxx > 0.008856 ) 
        x = xxx;
    else
        x = ( x - 16.0 / 116.0 ) / 7.787;

    deValue zzz = z*z*z;
    if ( zzz > 0.008856 ) 
        z = zzz;
    else            
        z = ( z - 16.0 / 116.0 ) / 7.787;

    x *= 95.047;
    y *= 100.0;
    z *= 108.883;

    x /= 100.0;
    y /= 100.0;
    z /= 100.0;

    values[4] = x;
    values[5] = y;
    values[6] = z;
}

void lab2rgb(deValue* values)
{
    lab2xyz(values);
    inverseCopy(values);
    xyz2rgb(values);
}

void rgb2lab(deValue* values)
{
    rgb2xyz(values);
    inverseCopy(values);
    xyz2lab(values);
}

void rgb2hsl(deValue* values)
{
    deValue r = values[0];
    deValue g = values[1];
    deValue b = values[2];

    deValue minVal = min( r, g, b );
    deValue maxVal = max( r, g, b );
    deValue delta = maxVal - minVal;
    deValue sum = minVal + maxVal;

    deValue L = sum / 2.0;
    deValue H;
    deValue S;

    if (delta == 0)
    {
       H = 0;
       S = 0;
    }
    else                                   
    {
        if ( L < 0.5 )
        {
            S = delta / sum;
        }
        else
        {
            S = delta / ( 2.0 - sum );
        }            

        deValue delR = ( ( ( maxVal - r ) / 6 ) + ( delta / 2 ) ) / delta;
        deValue delG = ( ( ( maxVal - g ) / 6 ) + ( delta / 2 ) ) / delta;
        deValue delB = ( ( ( maxVal - b ) / 6 ) + ( delta / 2 ) ) / delta;

        if ( r == maxVal )
        {
            H = delB - delG;
        }            
        else if ( g == maxVal )
        {
            H = ( 1.0 / 3.0 ) + delR - delB;
        }            
        else if ( b == maxVal )
        {
            H = ( 2.0 / 3.0 ) + delG - delR;
        }            

        if ( H < 0 )
            H += 1;
        if ( H > 1 )
            H -= 1;
    }

    values[4] = H;        
    values[5] = S;        
    values[6] = L;        
}

deValue Hue_2_RGB( deValue v1, deValue v2, deValue vH ) 
{
   if ( vH < 0 )
        vH += 1;
   if ( vH > 1 ) 
        vH -= 1;
   if ( ( 6.0 * vH ) < 1.0 ) 
        return ( v1 + ( v2 - v1 ) * 6.0 * vH );
   if ( ( 2.0 * vH ) < 1.0 ) 
        return ( v2 );
   if ( ( 3.0 * vH ) < 2.0 ) 
        return ( v1 + ( v2 - v1 ) * ( ( 2.0 / 3.0 ) - vH ) * 6.0 );
   return ( v1 );
}

void hsl2rgb(deValue* values)
{
    deValue H = values[0];
    deValue S = values[1];
    deValue L = values[2];

    deValue r;
    deValue g;
    deValue b;

    if ( S == 0 )                       
    {
        r = L;
        g = L;
        b = L;
    }
    else
    {
        deValue v1;
        deValue v2;

        if ( L < 0.5 )
        {
            v2 = L * ( 1 + S );
        }            
        else
        {
            v2 = ( L + S ) - ( S * L );
        }                

        v1 = 2.0 * L - v2;

        r = Hue_2_RGB( v1, v2, H + ( 1.0 / 3.0 ) );
        g = Hue_2_RGB( v1, v2, H );
        b = Hue_2_RGB( v1, v2, H - ( 1.0 / 3.0 ) );
    }
    values[4] = r;        
    values[5] = g;        
    values[6] = b;        
}

void rgb2hsv(deValue* values)
{
    deValue r = values[0];
    deValue g = values[1];
    deValue b = values[2];

    deValue minVal = min( r, g, b );
    deValue maxVal = max( r, g, b );
    deValue delta = maxVal - minVal;

    deValue V = maxVal;
    deValue H;
    deValue S;

    if (delta == 0)
    {
       H = 0;
       S = 0;
    }
    else                                   
    {
        S = delta / maxVal;

        deValue delR = ( ( ( maxVal - r ) / 6 ) + ( delta / 2 ) ) / delta;
        deValue delG = ( ( ( maxVal - g ) / 6 ) + ( delta / 2 ) ) / delta;
        deValue delB = ( ( ( maxVal - b ) / 6 ) + ( delta / 2 ) ) / delta;

        if ( r == maxVal )
        {
            H = delB - delG;
        }            
        else if ( g == maxVal )
        {
            H = ( 1.0 / 3.0 ) + delR - delB;
        }            
        else if ( b == maxVal )
        {
            H = ( 2.0 / 3.0 ) + delG - delR;
        }            

        if ( H < 0 )
            H += 1;
        if ( H > 1 )
            H -= 1;
    }

    values[4] = H;        
    values[5] = S;        
    values[6] = V;        
}

void hsv2rgb(deValue* values)
{
    deValue H = values[0];
    deValue S = values[1];
    deValue V = values[2];

    deValue r;
    deValue g;
    deValue b;

    if ( S == 0 )
    {
       r = V;
       g = V;
       b = V;
    }
    else
    {
        deValue var_h = H * 6.0;
    
        if ( var_h == 6.0 )
        {
             var_h = 0;
        }

        deValue var_i = int( var_h);
        deValue var_1 = V * ( 1.0 - S );
        deValue var_2 = V * ( 1.0 - S * ( var_h - var_i ) );
        deValue var_3 = V * ( 1 - S * ( 1 - ( var_h - var_i ) ) );

        if ( var_i == 0 ) 
        { 
            r = V;
            g = var_3;
            b = var_1;
        }
        else if ( var_i == 1 ) 
        { 
            r = var_2;
            g = V;
            b = var_1;
        }
        else if ( var_i == 2 )
        {
            r = var_1;
            g = V;
            b = var_3;
        }
        else if ( var_i == 3 )
        {
            r = var_1;
            g = var_2;
            b = V;
        }
        else if ( var_i == 4 )
        {
            r = var_3;
            g = var_1;
            b = V;
        }
        else
        {
            r = V;
            g = var_1;
            b = var_2;
        }
    }

    values[4] = r;        
    values[5] = g;        
    values[6] = b;        
}

void rgb2bw(deValue* values)
{
    // it could be configurable but this is the same as mixer from RGB to BW

    values[4] = 0.3 * values[0] + 0.6 * values[1] + 0.1 * values[2];
}

void bw2rgb(deValue* values)
{
    values[4] = values[0];
    values[5] = values[0];
    values[6] = values[0];
}

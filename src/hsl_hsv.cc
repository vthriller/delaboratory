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

#include "hsl_hsv.h"

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

void rgb2hsl(deValue r, deValue g, deValue b, deValue &h, deValue &s, deValue& l)
{
    deValue minVal = min( r, g, b );
    deValue maxVal = max( r, g, b );
    deValue delta = maxVal - minVal;
    deValue sum = minVal + maxVal;

    l = sum / 2.0;

    if (delta == 0)
    {
       h = 0;
       s = 0;
    }
    else                                   
    {
        if ( l < 0.5 )
        {
            s = delta / sum;
        }
        else
        {
            s = delta / ( 2.0 - sum );
        }            

        deValue deltaR = ( ( ( maxVal - r ) / 6 ) + ( delta / 2 ) ) / delta;
        deValue deltaG = ( ( ( maxVal - g ) / 6 ) + ( delta / 2 ) ) / delta;
        deValue deltaB = ( ( ( maxVal - b ) / 6 ) + ( delta / 2 ) ) / delta;

        if ( r == maxVal )
        {
            h = deltaB - deltaG;
        }            
        else if ( g == maxVal )
        {
            h = ( 1.0 / 3.0 ) + deltaR - deltaB;
        }            
        else if ( b == maxVal )
        {
            h = ( 2.0 / 3.0 ) + deltaG - deltaR;
        }            

        if ( h < 0 )
            h += 1;
        if ( h > 1 )
            h -= 1;
    }
}

deValue hue2rgb( deValue v1, deValue v2, deValue vH ) 
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

void hsl2rgb(deValue h, deValue s, deValue l, deValue& r, deValue& g, deValue& b)
{
    if ( s == 0 )                       
    {
        r = l;
        g = l;
        b = l;
    }
    else
    {
        deValue v1;
        deValue v2;

        if ( l < 0.5 )
        {
            v2 = l * ( 1 + s );
        }            
        else
        {
            v2 = ( l + s ) - ( s * l );
        }                

        v1 = 2.0 * l - v2;

        r = hue2rgb( v1, v2, h + ( 1.0 / 3.0 ) );
        g = hue2rgb( v1, v2, h );
        b = hue2rgb( v1, v2, h - ( 1.0 / 3.0 ) );
    }
}

void rgb2hsv(deValue r, deValue g, deValue b, deValue &h, deValue &s, deValue& v)
{
    v = max( r, g, b );

    deValue minVal = min( r, g, b );
    deValue delta = v - minVal;

    if (delta == 0)
    {
       h = 0;
       s = 0;
    }
    else                                   
    {
        s = delta / v;

        deValue deltaR = ( ( ( v - r ) / 6.0 ) + ( delta / 2.0 ) ) / delta;
        deValue deltaG = ( ( ( v - g ) / 6.0 ) + ( delta / 2.0 ) ) / delta;
        deValue deltaB = ( ( ( v - b ) / 6.0 ) + ( delta / 2.0 ) ) / delta;

        if ( r == v )
        {
            h = deltaB - deltaG;
        }            
        else if ( g == v )
        {
            h = ( 1.0 / 3.0 ) + deltaR - deltaB;
        }            
        else if ( b == v )
        {
            h = ( 2.0 / 3.0 ) + deltaG - deltaR;
        }            

        if ( h < 0 )
            h += 1.0;
        if ( h > 1 )
            h -= 1.0;
    }
}

void hsv2rgb(deValue h, deValue s, deValue v, deValue& r, deValue& g, deValue& b)
{
    if ( s == 0 )
    {
       r = v;
       g = v;
       b = v;
    }
    else
    {
        deValue var_h = h * 6.0;
    
        if ( var_h == 6.0 )
        {
             var_h = 0;
        }

        deValue var_i = int( var_h);
        deValue var_1 = v * ( 1.0 - s );
        deValue var_2 = v * ( 1.0 - s * ( var_h - var_i ) );
        deValue var_3 = v * ( 1.0 - s * ( 1.0 - ( var_h - var_i ) ) );

        if ( var_i == 0 ) 
        { 
            r = v;
            g = var_3;
            b = var_1;
        }
        else if ( var_i == 1 ) 
        { 
            r = var_2;
            g = v;
            b = var_1;
        }
        else if ( var_i == 2 )
        {
            r = var_1;
            g = v;
            b = var_3;
        }
        else if ( var_i == 3 )
        {
            r = var_1;
            g = var_2;
            b = v;
        }
        else if ( var_i == 4 )
        {
            r = var_3;
            g = var_1;
            b = v;
        }
        else
        {
            r = v;
            g = var_1;
            b = var_2;
        }
    }
}

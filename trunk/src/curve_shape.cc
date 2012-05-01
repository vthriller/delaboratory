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

#include "curve_shape.h"
#include <iostream>
#include "curve_function_bezier.h"
#include <cassert>

deCurveShape::deCurveShape(int _size)
:size(_size), functions(_size)
{
}

deCurveShape::~deCurveShape()
{
}

deValue deCurveShape::calc(deValue value)
{
    if ( value <= 0 )
    {
        return functions.at(0).calc(0);
    }
    else if (value >= 1)
    {
        return functions.at(size-1).calc(1);
    }
    else
    {
        deValue s = size - 1;
        int p = s * value;

        return functions.at(p).calc(value);
    }        
}

void deCurveShape::build(const deCurvePoints& points)
{
    typedef std::map<deValue, deValue> deNodes;
    deNodes nodes;
    deCurvePoints::const_iterator j;

    for (j = points.begin(); j != points.end(); j++)
    {
        const deCurvePoint& point = *j;
        deValue x = point.getX();
        deValue y = point.getY();
        nodes.insert(std::pair<deValue, deValue>(x, y));
    }        

    /* math formulas from GIMP 
       gimp_curve_plot from gimpcurve.c */

    deNodes::const_iterator i;
    
    for (i = nodes.begin(); i != nodes.end(); i++)
    {
        bool left = false;
        bool right = false;

        deValue x0 = (*i).first;
        deValue y0 = (*i).second;
        deNodes::const_iterator j = i;
        deNodes::const_iterator k = i;
        j++;
        k++;
        deValue x3 = x0;
        deValue y3 = y0;
        if (j != nodes.end())
        {
            x3 = (*j).first;
            y3 = (*j).second;
            k++;
            if (k != nodes.end())
            {
                right = true;
            }
        }

        deNodes::const_iterator h = i;
        if (h != nodes.begin())
        {
            h--;
            left = true;
        }            

        deValue dx = x3 - x0;
        deValue dy = y3 - y0;

        deValue y1;
        deValue y2;

        if ((!left) && (!right))
        {
            y1 = y0 + dy / 3.0;
            y2 = y0 + dy * 2.0 / 3.0;
        }

        if ((!left) && (right))
        {
            deValue s = ((*k).second - y0) / ((*k).first - x0);
            y2 = y3 - s * dx / 3.0;
            y1 = y0 + (y2 - y0) / 2.0;
        }

        if ((left) && (!right))
        {
            deValue s = (y3 - (*h).second) / (x3 - (*h).first);

            y1 = y0 + s * dx / 3.0;
            y2 = y3 + (y1 - y3) / 2.0;
        }

        if ((left) && (right))
        {
            deValue s1 = (y3 - (*h).second) / (x3 - (*h).first);
            deValue s2 = ((*k).second - y0) / ((*k).first - x0);

            y1 = y0 + s1 * dx / 3.0;
            y2 = y3 - s2 * dx / 3.0;
        }

        int p1 = x0 * (size - 1);
        int p2 = x3 * (size - 1);
        int iii;
        for (iii = p1; iii <= p2; iii++)
        {
            functions.at(iii).set(x0, x3, y0, y1, y2, y3);
        }
    }

}

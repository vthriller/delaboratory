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
#include "curve_function_spline.h"
#include "curve_function_bezier.h"

#define BEZIER 1

deCurveShape::deCurveShape(int _size)
:size(_size)
{
    functions.reserve(size);
    int i;
    for (i = 0; i < size; i++)
    {
        functions[i] = NULL;
    }
}

deCurveShape::~deCurveShape()
{
    clearFunctions();
}

void deCurveShape::clearFunctions()
{
    int i;
    for (i = 0; i < size; i++)
    {
        if (functions[i] != NULL)
        {
            delete functions[i];
            functions[i] = NULL;
        }            
    }
}

/*
void deCurveShape::storeValues(deValue x1, deValue y1, deValue x2, deValue y2)
{
    int p1 = x1 * (size - 1);
    int p2 = x2 * (size - 1);

    int i;
    for (i = p1; i <= p2; i++)
    {
        functions[i] = new deCurveFunctionLinear(x1, y1, x2, y2);
    }

}
*/

deValue deCurveShape::calc(deValue value)
{
    deValue s = size - 1;
    int p = s * value;

    return functions[p]->calc(value);
}

void deCurveShape::build(const deCurvePoints& points)
{
    nodes.clear();
    clearFunctions();
    deCurvePoints::const_iterator j;

    for (j = points.begin(); j != points.end(); j++)
    {
        const deCurvePoint& point = *j;
        deValue x = point.getX();
        deValue y = point.getY();
        nodes.insert(std::pair<deValue, deValue>(x, y));
    }        

    /*

    deNodes::const_iterator i;
    
    for (i = nodes.begin(); i != nodes.end(); i++)
    {
        deValue x1 = (*i).first;
        deValue y1 = (*i).second;
        deNodes::const_iterator k;
        k = i;
        k++;
        deValue x2 = x1;
        deValue y2 = y1;
        if (k != nodes.end())
        {
            x2 = (*k).first;
            y2 = (*k).second;
        }
        storeValues(x1, y1, x2, y2);
    }
    */
#if BEZIER
    generateBezier();
#else    
    generateSpline();
#endif
}

void deCurveShape::getControlPoints(deCurvePoints& points) const
{
    points.clear();

    deNodes::const_iterator i;
    for (i = nodes.begin(); i != nodes.end(); i++)
    {
        deValue x = i->first;        
        deValue y = i->second;
        points.push_back(deCurvePoint(x,y));
    }

}

void deCurveShape::getCurvePoints(deCurvePoints& points) const
{
    points.clear();
    int i;

    deValue s = 1.0 / (size - 1);
    for (i = 0; i < size; i++)
    {
        deValue x = i * s;
        deValue y = functions[i]->calc(x);
        points.push_back(deCurvePoint(x,y));
    }

}

void deCurveShape::generateSpline()
{
    int n = nodes.size();

    deValue *x = new deValue[n+1];
    deValue *y = new deValue[n+1];

    deNodes::const_iterator j;
    int pos = 1;
    for (j = nodes.begin(); j != nodes.end(); j++)
    {
        x[pos] = j->first;
        y[pos] = j->second;
        pos++;
    }

    deValue *u = new deValue[n];
    deValue *y2 = new deValue[n + 1];

    y2[1] = 0.0;
    u[1] = 0.0;

    int i;
    for (i = 2 ; i <= n-1 ; i++)
    {
        deValue sig = (x[i] - x[i-1]) / (x[i+1] - x[i-1]);
        deValue p = sig * y2[i-1] + 2.0;
        y2[i] = ( sig - 1.0 ) / p;
        u[i] = ( y[i+1] - y[i] ) / (x[i+1] - x[i]) - (y[i] - y[i-1]) / (x[i] - x[i-1]);
        u[i] = ( 6.0 * u[i] / (x[i+1] - x[i-1]) - sig * u[i-1]) / p;
    }

    y2 [n] = 0.0;

    int k;
    for (k = n-1 ; k >=1 ; k--)
    {
        y2[k] = y2[k] * y2[k+1] + u[k];
    }        

    {
        deNodes::const_iterator i;
        int ii = 1;
        
        for (i = nodes.begin(); i != nodes.end(); i++)
        {
            deValue x1 = (*i).first;
            deNodes::const_iterator k = i;
            k++;
            deValue x2 = x1;
            if (k != nodes.end())
            {
                x2 = (*k).first;
            }
            int p1 = x1 * (size - 1);
            int p2 = x2 * (size - 1);
            int iii;
            for (iii = p1; iii <= p2; iii++)
            {
                int mm = ii;
                if (mm < 1)
                {
                    mm = 1;
                }
                int nn = ii + 1;
                if (nn > n)
                {
                    nn = n;
                }
                if (functions[iii] != NULL)
                {
                    delete functions[iii];
                }
                functions[iii] = new deCurveFunctionSpline(x[mm], y[mm], y2[mm], x[nn], y[nn], y2[nn]);
            }
            ii++;
        }
    }        

    delete [] x;
    delete [] y;
    delete [] u;
    delete [] y2;
}

void deCurveShape::generateBezier()
{
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
            functions[iii] = new deCurveFunctionBezier(x0, x3, y0, y1, y2, y3);
        }
    }

}

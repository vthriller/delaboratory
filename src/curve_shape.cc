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
#include "curve_function_linear.h"

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
            functions[i] = NULL;
        }            
        delete functions[i];
    }
}

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
}

void deCurveShape::getShapePoints(deCurvePoints& points) const
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

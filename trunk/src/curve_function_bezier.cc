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

#include "curve_function_bezier.h"

deCurveFunctionBezier::deCurveFunctionBezier(deValue _x0, deValue _x3, deValue _y0, deValue _y1, deValue _y2, deValue _y3)
:x0(_x0),
 x3(_x3),
 y0(_y0),
 y1(_y1),
 y2(_y2),
 y3(_y3)
{
}

deCurveFunctionBezier::~deCurveFunctionBezier()
{
}


deValue deCurveFunctionBezier::calc(deValue value) const
{
    deValue t = (value - x0) / (x3 - x0);
    deValue t1 = 1.0 - t;

    return (y0 * t1 * t1 * t1) + 3 * (y1 * t1 * t1 * t) + 3 * (y2 * t1 * t * t) + (y3 * t * t * t);
}    


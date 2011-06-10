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

#include "curve_function_spline.h"
#include <iostream>
#include <cassert>

deCurveFunctionSpline::deCurveFunctionSpline(deValue _xa_lo, deValue _ya_lo, deValue _y2a_lo, deValue _xa_hi, deValue _ya_hi, deValue _y2a_hi)
:xa_lo(_xa_lo),
 ya_lo(_ya_lo),
 y2a_lo(_y2a_lo),
 xa_hi(_xa_hi),
 ya_hi(_ya_hi),
 y2a_hi(_y2a_hi)
{
}

deCurveFunctionSpline::~deCurveFunctionSpline()
{
}


deValue deCurveFunctionSpline::calc(deValue value) const
{
    if (xa_hi == xa_lo)
    {
        return ya_lo;
    }

    assert(xa_hi > xa_lo);

    deValue h = xa_hi - xa_lo;
    deValue a = ( xa_hi - value ) / h;
    deValue b = ( value - xa_lo ) / h;

    return a * ya_lo + b * ya_hi + ( (a*a*a-a) * y2a_lo + (b*b*b-b)*y2a_hi ) * (h*h) / 6.0;
}    


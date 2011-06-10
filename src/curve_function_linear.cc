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

#include "curve_function_linear.h"
#include <cassert>

deCurveFunctionLinear::deCurveFunctionLinear(deValue x1, deValue y1, deValue x2, deValue y2)
{
    assert(x2 >= x1);
    a = 0;
    if ( x2 > x1) 
    {
        a = (y2 - y1) / (x2 - x1);
    }       
    b = y1 - x1 * a;
}

deCurveFunctionLinear::~deCurveFunctionLinear()
{
}

deValue deCurveFunctionLinear::calc(deValue value) const
{
    return a * value + b;
}

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

#ifndef _DE_CURVE_FUNCTION_SPLINE_H
#define _DE_CURVE_FUNCTION_SPLINE_H

#include "curve_function.h"

class deCurveFunctionSpline:public deCurveFunction
{
    private:
        deValue xa_lo;
        deValue ya_lo;
        deValue y2a_lo;
        deValue xa_hi;
        deValue ya_hi;
        deValue y2a_hi;

    public:
        deCurveFunctionSpline(deValue _xa_lo, deValue _ya_lo, deValue _y2a_lo, deValue _xa_hi, deValue _ya_hi, deValue _y2a_hi);
        virtual ~deCurveFunctionSpline();

        virtual deValue calc(deValue value) const;
};

#endif

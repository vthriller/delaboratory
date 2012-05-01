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

#ifndef _DE_CURVE_POINT_H
#define _DE_CURVE_POINT_H

#include "value.h"
#include <vector>

class deCurvePoint
{
    private:
        deValue x;
        deValue y;

        deCurvePoint();
    public:
        deCurvePoint(deValue _x, deValue _y);
        virtual ~deCurvePoint();

        deValue getX() const {return x;};
        deValue getY() const {return y;};

        void move(deValue _x, deValue _y);
        
};

typedef std::vector<deCurvePoint> deCurvePoints;

#endif

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

#ifndef _DE_CURVE_H
#define _DE_CURVE_H

#include <libxml/parser.h>
#include "curve_point.h"
#include "curve_shape.h"
#include <list>

class deBaseChannel;


class deCurve
{
    private:
        deCurvePoints points;
        deCurveShape shape;

        void loadPoint(xmlNodePtr node);
    public:    
        deCurve(int size);
        virtual ~deCurve();

        int findPoint(deValue x, deValue y) const;
        int addPoint(deValue x, deValue y);
        void deletePoint(int p);
        void movePoint(int p, deValue x, deValue y);

        void process(const deBaseChannel& source, deBaseChannel& destination);

        const deCurvePoints& getPoints() const;
        void getControlPoints(deCurvePoints& points) const;
        void getCurvePoints(deCurvePoints& points) const;
        const deCurvePoint& getPoint(int n) const;

        void save(xmlNodePtr node) const;
        void load(xmlNodePtr node);

        void reset();

};

#endif

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
#include "mutex.h"
#include <vector>

class deChannel;

class deBaseCurve
{
    private:
        deCurvePoints controlPoints;
        deCurveShape shape;

        mutable deMutex mutex;

    public:
        deBaseCurve();
        virtual ~deBaseCurve();

        void clearPoints();
        int addPoint(deValue x, deValue y);

        void build();
        void process(const deValue* source, deValue* destination, int n);
};

class deCurveOld
{
    private:
        deCurvePoints points;
        deCurveShape shape;

        mutable deMutex mutex;

        void loadPoint(xmlNodePtr node);

        deCurveOld(const deCurveOld& c);
        deCurveOld& operator=(const deCurveOld& c);

        void lock() const; 
        void unlock() const;

    public:    
        deCurveOld();
        virtual ~deCurveOld();

        int findPoint(deValue x, deValue y) const;
        int addPoint(deValue x, deValue y);
        void deletePoint(int p);
        void movePoint(int p, deValue x, deValue y);

        void process(const deChannel& source, deChannel& destination, int n);
        void process(const deValue* source, deValue* destination, int n);

        const deCurvePoints& getPoints() const;
        const deCurvePoints& getControlPoints() const;
        const deCurvePoint& getPoint(int n) const;

        void save(xmlNodePtr node) const;
        void load(xmlNodePtr node);

        void reset();
        void setConst(deValue v);
        void setAngle(int a);
        void setS(int a);
        void invert();
        void fill(int n, deValue a, deValue r);

        bool isNeutral() const;

        deValue calcValue(deValue value);

        void movePointVertically(int p, deValue delta);

        void addRandom();



};

#endif

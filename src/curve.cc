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

#include "curve.h"
#include "channel.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <ctime>
#include "str.h"
#include "logger.h"

#define CURVE_POINT_PICK_DISTANCE 0.03
#define VERTICAL_STEP 0.01

#define CURVE_SHAPE_SIZE 256

deBaseCurve::deBaseCurve()
:shape(CURVE_SHAPE_SIZE)
{
}

deBaseCurve::~deBaseCurve()
{
}

void deBaseCurve::clearPoints()
{
    mutex.lock();
    controlPoints.clear();
    mutex.unlock();
}

int deBaseCurve::addPoint(deValue x, deValue y)
{
    if ((x < 0) || (x > 1))
    {
        return -1;
    }
    if (y < 0)
    {
        y = 0;
    }
    if (y > 1)
    {
        y = 1;
    }
    /*
    if ((y < 0) || (y > 1))
    {
        return -1;
    }*/

    mutex.lock();

    controlPoints.push_back(deCurvePoint(x,y));

    int p = controlPoints.size() - 1;

    mutex.unlock();

    return p;
}

void deBaseCurve::build()
{
    mutex.lock();

    shape.build(controlPoints);

    mutex.unlock();
}

void deBaseCurve::process(const deValue* pixels, deValue* p, int n) const
{
    mutex.lock();

    int i;
    for (i = 0; i < n; i++)
    {
        deValue value = pixels[i];
        deValue result = shape.calc(value);

        if (result < 0)
        {
            p[i] = 0;
        }
        else if (result > 1)
        {
            p[i] = 1;
        }
        else
        {
            p[i] = result;
        }
    }

    mutex.unlock();
}

deValue deBaseCurve::calcValue(deValue value) const
{
    return shape.calc(value);
}

int deBaseCurve::findPoint(deValue x, deValue y) const
{
    mutex.lock();

    deCurvePoints::const_iterator j;
    int i = 0;

    for (j = controlPoints.begin(); j != controlPoints.end(); j++)
    {
        const deCurvePoint& point = *j;
        deValue xx = x - point.getX();
        float yy = y - point.getY();
        if (sqrt (xx * xx + yy * yy) < CURVE_POINT_PICK_DISTANCE)
        {
            mutex.unlock();
            return i;
        }
        i++;
    }

    mutex.unlock();
    return -1;
}

void deBaseCurve::deletePoint(int p)
{
    mutex.lock();

    deCurvePoints::iterator i = controlPoints.begin();
    while (p > 0)
    {
        i++;
        p--;
    }

    controlPoints.erase(i);

    mutex.unlock();
}

void deBaseCurve::movePoint(int p, deValue x, deValue y)
{
    if (p < 0)
    {
        return;
    }

    if ((unsigned int)p >= controlPoints.size())
    {
        return;
    }

    if ((x < 0) || (x > 1))
    {
        return;
    }
    if ((y < 0) || (y > 1))
    {
        return;
    }

    mutex.lock();

    deCurvePoints::iterator i = controlPoints.begin();
    while (p > 0)
    {
        i++;
        p--;
    }

    deValue xx = (*i).getX();

    if (xx == 0)
    {
        x = 0;
    }
    else
    {
        if (x == 0)
        {
            mutex.unlock();
            return;
        }
    }

    if (xx == 1)
    {
        x = 1;
    }
    else
    {
        if (x == 1)
        {
            mutex.unlock();
            return;
        }
    }

    (*i).move(x, y);

    mutex.unlock();
}

const deCurvePoint& deBaseCurve::getPoint(int n) const
{
    mutex.lock();

    deCurvePoints::const_iterator i = controlPoints.begin();
    while (n > 0)
    {
        i++;
        n--;
    }

    mutex.unlock();
    return *i;
}

const deCurvePoints& deBaseCurve::getControlPoints() const
{
    return controlPoints;
}

bool deBaseCurve::isNeutral() const
{
    if (controlPoints.size() != 2)
    {
        return false;
    }
    deCurvePoints::const_iterator i = controlPoints.begin();
    deCurvePoint a = *i;
    if (a.getY() != 0)
    {
        return false;
    }
    i++;
    deCurvePoint b = *i;
    if (b.getY() != 1)
    {
        return false;
    }
    return true;
}


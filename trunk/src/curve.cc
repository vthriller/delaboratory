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
#include "xml.h"
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
    if ((y < 0) || (y > 1))
    {
        return -1;
    }

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

void deBaseCurve::process(const deValue* pixels, deValue* p, int n)
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

deCurveOld::deCurveOld()
:shape(CURVE_SHAPE_SIZE)
{
    reset();
}

deCurveOld::~deCurveOld()
{
}

void deCurveOld::reset()
{
    lock();

    points.clear();
    fill(1, 1, 0);
    shape.build(points);

    unlock();
}

void deCurveOld::invert()
{
    lock();

    points.clear();
    fill(1, -1, 0);

    shape.build(points);

    unlock();
}

void deCurveOld::setConst(deValue v)
{
    lock();

    points.clear();
    points.push_back(deCurvePoint(0,v));
    points.push_back(deCurvePoint(1,v));
    shape.build(points);

    unlock();
}

void deCurveOld::setAngle(int a)
{
    lock();

    if (a < 4)
    {
        deValue p1 = a / 8.0;
        deValue p2 = 1 - p1;
        points.clear();
        points.push_back(deCurvePoint(0,0));
        points.push_back(deCurvePoint(p1,0));
        points.push_back(deCurvePoint(p2,1));
        points.push_back(deCurvePoint(1,1));
    }
    else
    {
        deValue p1 = (a - 3) / 8.0;
        deValue p2 = 1 - p1;
        points.clear();
        points.push_back(deCurvePoint(0,p1));
        points.push_back(deCurvePoint(1,p2));
    }
    shape.build(points);

    unlock();
}

void deCurveOld::setS(int a)
{
    lock();

    deValue h = a / 16.0;
    deValue p1 = 1.0 / 4.0;
    deValue p2 = 1.0 - 1.0 / 4.0;

    points.clear();
    points.push_back(deCurvePoint(0, 0));

    points.push_back(deCurvePoint(p1, p1 - h));
    points.push_back(deCurvePoint(p2, p2 + h));

    points.push_back(deCurvePoint(1, 1));
    shape.build(points);

    unlock();
}


void deCurveOld::fill(int n, deValue a, deValue r)
{
    lock();

    points.clear();
    double step = 1.0 / n;
    double p = 0.0;

    srand(time(0));

    while (p <= 1.0)
    {
        double y = p;
        if (a < 0)
        {
            y = 1.0 - y;
        }
        deValue rr = r * ( (double) rand() / RAND_MAX );
        rr = 2 * rr - r;
        y+= rr;
        if (y < 0)
        {
            y = 0;
        }
        if (y > 1)
        {
            y = 1;
        }

        points.push_back(deCurvePoint(p,y));
        p+=step;
    }
    shape.build(points);

    unlock();
}

deValue deCurveOld::calcValue(deValue value)
{
    lock();
    unlock();

    return shape.calc(value);
}

void deCurveOld::process(const deChannel& source, deChannel& destination, int n)
{
    lock();

    const deValue* pixels = source.getPixels();
    deValue* p = destination.getPixels();

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

    unlock();
}

void deCurveOld::process(const deValue* pixels, deValue* p, int n)
{
    lock();

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

    unlock();
}

int deCurveOld::findPoint(deValue x, deValue y) const
{
    lock();

    deCurvePoints::const_iterator j;
    int i = 0;

    for (j = points.begin(); j != points.end(); j++)
    {
        const deCurvePoint& point = *j;
        deValue xx = x - point.getX();
        float yy = y - point.getY();
        if (sqrt (xx * xx + yy * yy) < CURVE_POINT_PICK_DISTANCE)
        {
            unlock();
            return i;
        }
        i++;
    }

    unlock();
    return -1;
}

int deCurveOld::addPoint(deValue x, deValue y)
{
    if ((x <= 0) || (x >= 1))
    {
        return -1;
    }
    if ((y < 0) || (y > 1))
    {
        return -1;
    }

    lock();

    points.push_back(deCurvePoint(x,y));


    shape.build(points);

    unlock();

    return points.size() - 1;
}

void deCurveOld::deletePoint(int p)
{
    lock();

    deCurvePoints::iterator i = points.begin();
    while (p > 0)
    {
        i++;
        p--;
    }

    deValue xx = (*i).getX();

    points.erase(i);

    if (xx == 0)
    {
        points.push_back(deCurvePoint(0,0));
    }
    if (xx == 1)
    {
        points.push_back(deCurvePoint(1,1));
    }


    shape.build(points);

    unlock();
}

void deCurveOld::movePoint(int p, deValue x, deValue y)
{
    if (p < 0)
    {
        return;
    }

    if ((unsigned int)p >= points.size())
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

    lock();

    deCurvePoints::iterator i = points.begin();
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
            unlock();
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
            unlock();
            return;
        }
    }

    (*i).move(x, y);
    shape.build(points);

    unlock();
}

void deCurveOld::movePointVertically(int p, deValue delta)
{
    if (p < 0)
    {
        return;
    }

    if ((unsigned int)p >= points.size())
    {
        return;
    }

    lock();

    deCurvePoints::iterator i = points.begin();
    while (p > 0)
    {
        i++;
        p--;
    }

    deValue xx = (*i).getX();
    deValue yy = (*i).getY();

    yy += delta;

    if (yy < 0.0)
    {
        yy = 0.0;
    }
    if (yy > 1.0)
    {
        yy = 1.0;
    }

    (*i).move(xx, yy);
    shape.build(points);

    unlock();
}

const deCurvePoints& deCurveOld::getPoints() const
{
    lock();
    unlock();

    return points;
}

const deCurvePoint& deCurveOld::getPoint(int n) const
{
    lock();

    deCurvePoints::const_iterator i = points.begin();
    while (n > 0)
    {
        i++;
        n--;
    }

    unlock();
    return *i;
}

const deCurvePoints& deCurveOld::getControlPoints() const
{
    lock();
    unlock();

    return points;
}

void deCurveOld::save(xmlNodePtr node) const
{
    deCurvePoints::const_iterator i;
    
    for (i = points.begin(); i != points.end(); i++)
    {
        xmlNodePtr child = xmlNewChild(node, NULL, BAD_CAST("point"), NULL);
        const deCurvePoint& point = *i;

        saveChild(child, "x", str(point.getX()));
        saveChild(child, "y", str(point.getY()));

    }        
}

void deCurveOld::loadPoint(xmlNodePtr node) 
{
    xmlNodePtr child = node->xmlChildrenNode;

    std::string xStr = "";
    std::string yStr = "";

    while (child)
    {
        if ((!xmlStrcmp(child->name, BAD_CAST("x")))) 
        {
            xStr = getContent(child);
        }            

        if ((!xmlStrcmp(child->name, BAD_CAST("y")))) 
        {
            yStr = getContent(child);
        }            

        child = child->next;
    }

    deValue x = 0;
    deValue y = 0;

    {
        x = getValue(xStr);
    }

    {
        y = getValue(yStr);
    }

    points.push_back(deCurvePoint(x,y));
}

void deCurveOld::load(xmlNodePtr node) 
{
    points.clear();

    xmlNodePtr child = node->xmlChildrenNode;

    while (child)
    {
        if ((!xmlStrcmp(child->name, BAD_CAST("point")))) 
        {
            loadPoint(child);
        }            

        child = child->next;
    }

    shape.build(points);
}

bool deCurveOld::isNeutral() const
{
    lock();
    unlock();

    if (points.size() != 2)
    {
        return false;
    }
    deCurvePoints::const_iterator i = points.begin();
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

void deCurveOld::lock() const
{
    mutex.lock();
}

void deCurveOld::unlock() const
{
    mutex.unlock();
}

void deCurveOld::addRandom()
{
    deValue x = (deValue) rand() / RAND_MAX;

    deValue v = calcValue(x);

    deValue y = (deValue) rand() / RAND_MAX;

    y *= 0.4;
    y -= 0.2;

    y += v;

    addPoint(x, y);
}        

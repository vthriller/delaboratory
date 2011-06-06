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

#include "nd.h"
#include <cmath>

deND::deND()
{
    //type = deNDHorizontal;
    type = deNDVignette;
    xCenter = 0.0;
    yCenter = 0.0;
    power = 1.0;
    inverted = false;
}

deND::~deND()
{
}

deValue deND::getValue(deValue x, deValue y) const
{
    deValue result = 0.0;
    x+= xCenter;
    y+= yCenter;
    switch (type)
    {
        case deNDHorizontal:
            result = 1.0 - power * x;
            break;
        case deNDVertical:
            result = 1.0 - power * y;
            break;
        case deNDVignette:
            result = 1.0 - power * sqrt(x*x+y*y);
            break;
        case deNDFlat:
            result = 1.0 - 0.4 * power;
            break;
        default:
            result = 0.0;
            break;
    }
    if (result > 1.0)
    {
        result = 1.0;
    }
    if (result < 0.0)
    {
        result = 0.0;
    }
    if (inverted)
    {
        result = 1.0 - result;
    }
    return result;
}

deValue deND::getXCenter() const
{
    return xCenter;
}

deValue deND::getYCenter() const
{
    return yCenter;
}

deValue deND::getPower() const
{
    return power;
}

void deND::setXCenter(deValue& v)
{
    xCenter = v;
}

void deND::setYCenter(deValue& v)
{
    yCenter = v;
}

void deND::setPower(deValue& v)
{
    power = v;
}

deNDType deND::getType() const
{
    return type;
}

void deND::setType(deNDType _type)
{
    type = _type;
}

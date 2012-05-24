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

#include "property_levels.h"
#include "str.h"

deLevels::deLevels()
{
    min = 0;
    middle = 0.5;
    max = 1.0;
}

deLevels::~deLevels()
{
}

void deLevels::setMin(deValue _value)
{
    min = _value;

    sort();
}

deValue deLevels::getMin() const
{
    return min;
}

void deLevels::setMiddle(deValue _value)
{
    middle = _value;

    sort();
}

deValue deLevels::getMiddle() const
{
    return middle;
}

void deLevels::setMax(deValue _value)
{
    max = _value;

    sort();
}

deValue deLevels::getMax() const
{
    return max;
}

void deLevels::sort()
{
    if ((min <= middle) && (middle <= max))
    {
        return;
    }

    deValue m = 0.01;

    if (middle > 1.0 - m)
    {
        middle = 1.0 + m;
    }

    if (middle < m)
    {
        middle = m;
    }

    if (min > middle - m)
    {
        min = middle - m;
    }

    if (max < middle + m)
    {
        max = middle + m;
    }
}

bool deLevels::isNeutral() const
{
    if (min != 0.0)
    {
        return false;
    }
    if (middle != 0.5)
    {
        return false;
    }
    if (max != 1.0)
    {
        return false;
    }
    return true;
}


dePropertyLevels::dePropertyLevels(const std::string& _name, int _size)
:deProperty(_name), size(_size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        levels.push_back( deLevels());
    }
}

dePropertyLevels::~dePropertyLevels()
{
}



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

#include "property_numeric.h"
#include "str.h"

dePropertyNumeric::dePropertyNumeric(const std::string& _name, deValue _min, deValue _max)
:deProperty(_name), min(_min), max(_max)
{
    value = (min + max) / 2.0;
}

dePropertyNumeric::~dePropertyNumeric()
{
}

void dePropertyNumeric::set(deValue _value)
{
    value = _value;
    if (value > max)
    {
        value = max;
    }
    if (value < min)
    {
        value = min;
    }
}

deValue dePropertyNumeric::get() const
{
    return value;
}

deValue dePropertyNumeric::getMin() const
{
    return min;
}

deValue dePropertyNumeric::getMax() const
{
    return max;
}


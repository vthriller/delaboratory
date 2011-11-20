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

#include "property_boolean.h"
#include "str.h"
#include "xml.h"

dePropertyBoolean::dePropertyBoolean(const std::string& _name)
:deProperty(_name)
{
    value = false;
}

dePropertyBoolean::~dePropertyBoolean()
{
}

void dePropertyBoolean::set(bool _value)
{
    value = _value;
}


bool dePropertyBoolean::get() const
{
    return value;
}

void dePropertyBoolean::save(xmlNodePtr root) const
{
    saveChild(root, name, str(value));
}    

void dePropertyBoolean::load(xmlNodePtr child)
{
    if ((!xmlStrcmp(child->name, BAD_CAST(name.c_str())))) 
    {
        value = getBool(getContent(child));
    }
}    


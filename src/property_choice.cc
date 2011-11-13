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

#include "property_choice.h"
#include "xml.h"
#include "str.h"

dePropertyChoice::dePropertyChoice(const std::string& _name)
:deProperty(_name)
{
}

dePropertyChoice::~dePropertyChoice()
{
}

void dePropertyChoice::set(std::string _value)
{
    value = _value;
}

std::string dePropertyChoice::get() const
{
    return value;
}

void dePropertyChoice::save(xmlNodePtr root) const
{
    saveChild(root, name, value);
}

void dePropertyChoice::load(xmlNodePtr root)
{
    if ((!xmlStrcmp(root->name, BAD_CAST(name.c_str())))) 
    {
        value = getContent(root);
    }
}

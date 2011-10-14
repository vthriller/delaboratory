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

#include "xml.h"

void saveChild(xmlNodePtr node, std::string name, std::string value)
{
    xmlChar* _name = BAD_CAST(name.c_str());
    xmlChar* _value = BAD_CAST(value.c_str());

    xmlNodePtr child = xmlNewChild(node, NULL, _name, NULL);
    xmlNodeSetContent(child, _value);
}

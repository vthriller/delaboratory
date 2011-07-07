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

#include "property_integer.h"
#include "property_integer_slider.h"
#include <sstream>

dePropertyInteger::dePropertyInteger(deLayer& _parent, const std::string& _label, int _min, int _max)
:deProperty(_parent), label(_label), min(_min), max(_max)
{
    value = 0;
    if ((value < min) || (value > max))
    {
        value = (min + max) / 2.0;
    }
}

dePropertyInteger::~dePropertyInteger()
{
}

void dePropertyInteger::setValue(int _value)
{
    value = _value;
}

void dePropertyInteger::addPanelContent(wxPanel* panel, wxSizer* sizer)
{
    dePropertyIntegerSlider* slider = new dePropertyIntegerSlider(panel, *this, 100);
    sizer->Add(slider);
}    

void dePropertyInteger::saveContent(xmlNodePtr node)
{
    std::ostringstream oss;
    oss << value;
    xmlNodeSetContent(node, xmlCharStrdup(oss.str().c_str()));
}

void dePropertyInteger::load(xmlNodePtr node)
{
    xmlChar* s = xmlNodeGetContent(node);            
    std::string valueStr = (char*)(s);
    xmlFree(s);

    std::istringstream iss(valueStr);
    iss >> value;
}


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

#include "property_value.h"
#include "property_value_slider.h"

dePropertyValue::dePropertyValue(deLayer& _parent, const std::string& _label, deValue _min, deValue _max)
:deProperty(_parent), label(_label), min(_min), max(_max)
{
}

dePropertyValue::~dePropertyValue()
{
}

void dePropertyValue::setValue(deValue _value)
{
    value = _value;
}

void dePropertyValue::addPanelContent(wxPanel* panel, wxSizer* sizer, dePreviewStack& _stack)
{
    stack = &_stack;
    dePropertyValueSlider* slider = new dePropertyValueSlider(panel, *this, 100);
    sizer->Add(slider);
}    

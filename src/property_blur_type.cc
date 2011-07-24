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

#include "property_blur_type.h"
#include "blur_type_selector.h"

dePropertyBlurType::dePropertyBlurType(deLayer& _parent)
:deProperty(_parent)
{
    type = deBoxBlur;
}

dePropertyBlurType::~dePropertyBlurType()
{
}

void dePropertyBlurType::setBlurType(deBlurType _type)
{
    type = _type;
}

void dePropertyBlurType::addPanelContent(wxPanel* panel, wxSizer* sizer)
{
    wxPanel* selector = new deBlurTypeSelector(panel, *this);
    sizer->Add(selector);
}    

void dePropertyBlurType::saveContent(xmlNodePtr node)
{
    xmlNodeSetContent(node, xmlCharStrdup(getBlurTypeName(type).c_str()));
}

void dePropertyBlurType::load(xmlNodePtr node)
{
    xmlChar* s = xmlNodeGetContent(node);            
    std::string valueStr = (char*)(s);
    xmlFree(s);

    type = blurTypeFromString(valueStr);
}

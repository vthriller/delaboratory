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

#include "property_blend_mode.h"
#include "blend_mode_selector.h"

dePropertyBlendMode::dePropertyBlendMode(deLayer& _parent)
:deProperty(_parent)
{
    mode = deBlendInvalid;
}

dePropertyBlendMode::~dePropertyBlendMode()
{
}

void dePropertyBlendMode::setBlendMode(deBlendMode _mode)
{
    mode = _mode;
}

void dePropertyBlendMode::addPanelContent(wxPanel* panel, wxSizer* sizer)
{
    wxPanel* selector = new deBlendModeSelector(panel, *this);
    sizer->Add(selector);
}    

void dePropertyBlendMode::saveContent(xmlNodePtr node)
{
    xmlNodeSetContent(node, xmlCharStrdup(getBlendModeName(mode).c_str()));
}

void dePropertyBlendMode::load(xmlNodePtr node)
{
    xmlChar* s = xmlNodeGetContent(node);            
    std::string valueStr = (char*)(s);
    xmlFree(s);

    mode = blendModeFromString(valueStr);
}


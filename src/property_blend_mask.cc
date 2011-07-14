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

#include "property_blend_mask.h"
#include "mask_selector.h"
#include <sstream>

dePropertyBlendMask::dePropertyBlendMask(deLayer& _parent, const std::string& _label)
:deProperty(_parent), label(_label)
{
    layerIndex = 0;
    channel = 0;
}

dePropertyBlendMask::~dePropertyBlendMask()
{
}

void dePropertyBlendMask::setLayerIndex(int _index)
{
    layerIndex = _index;
}

void dePropertyBlendMask::setChannel(int _channel)
{
    channel = _channel;
}


void dePropertyBlendMask::addPanelContent(wxPanel* panel, wxSizer* sizer)
{
    wxPanel* selector = new deMaskSelector(panel, *this, label);
    sizer->Add(selector);
}    

void dePropertyBlendMask::saveContent(xmlNodePtr node)
{
/*
    std::ostringstream oss;
    oss << index;
    xmlNodeSetContent(node, xmlCharStrdup(oss.str().c_str()));
    */
}

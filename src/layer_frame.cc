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

#include "layer_frame.h"

deLayerFrame::deLayerFrame(wxWindow *parent, deLayer& _layer, dePreviewStack& _stack)
:deActionFrame(parent, _layer, _T("generic")), stack(_stack)
{
    sizer = new wxBoxSizer(wxVERTICAL);

    SetSizer(sizer);
}

deLayerFrame::~deLayerFrame()
{
}


void deLayerFrame::addProperty(deProperty* property)
{
    wxPanel* panel = property->getPanel(this, stack);
    sizer->Add(panel, 0);
}

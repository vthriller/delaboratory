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

#include "blend_mode_selector.h"
#include "property_blend_mode.h"
#include "blend_mode_choice.h"

deBlendModeSelector::deBlendModeSelector(wxPanel* parent, dePropertyBlendMode& _property, const std::string& s)
:wxPanel(parent), property(_property)
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* label = new wxStaticText(this, wxID_ANY, wxString::FromAscii(s.c_str()) );
    sizer->Add(label);

    blendModeChoice = makeBlendModeChoice(this, blendModes, property.getBlendMode());
    sizer->Add(blendModeChoice);

    SetSizer(sizer);

    Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(deBlendModeSelector::choose));
}

deBlendModeSelector::~deBlendModeSelector()
{
}

void deBlendModeSelector::choose(wxCommandEvent &event)
{
    int c = blendModeChoice->GetCurrentSelection();
    property.setBlendMode(blendModes[c]);
    property.onUpdate();
}

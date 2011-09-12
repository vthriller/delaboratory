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

#include "layer_selector.h"
#include "property_layer_index.h"
#include "layer_choice.h"
#include "layer.h"

deLayerSelector::deLayerSelector(wxPanel* parent, dePropertyLayerIndex& _property, const std::string& s)
:wxPanel(parent), property(_property)
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* label = new wxStaticText(this, wxID_ANY, wxString::FromAscii(s.c_str()) );
    sizer->Add(label);

    layerChoice = makeLayerChoice(this, property.getParent().getIndex(), property.getLayerIndex());
    sizer->Add(layerChoice);

    SetSizer(sizer);

    Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(deLayerSelector::choose));
}

deLayerSelector::~deLayerSelector()
{
}

void deLayerSelector::choose(wxCommandEvent &event)
{
    property.setLayerIndex( layerChoice->GetCurrentSelection() );
    property.onUpdate();
}


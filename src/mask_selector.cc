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

#include "mask_selector.h"
#include "property_blend_mask.h"
#include "layer_choice.h"
#include "channel_choice.h"
#include "layer.h"
#include "layer_stack.h"

deMaskSelector::deMaskSelector(wxPanel* parent, dePropertyBlendMask& _property, const std::string& s)
:wxPanel(parent), property(_property)
{
    sizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* label = new wxStaticText(this, wxID_ANY, wxString::FromAscii(s.c_str()) );
    sizer->Add(label);

    enabled = new wxCheckBox(this, wxID_ANY, _T("enabled"));
    sizer->Add(enabled);

    layerChoice = NULL;

    channelChoice = NULL;
    regenerateChannelChoice();

    SetSizer(sizer);

    Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(deMaskSelector::choose));
    Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(deMaskSelector::check));
}

deMaskSelector::~deMaskSelector()
{
}

void deMaskSelector::regenerateChannelChoice()
{
    if (layerChoice)
    {
        sizer->Detach(layerChoice);
        delete layerChoice;
        layerChoice = NULL;
    }

    if (channelChoice)
    {
        sizer->Detach(channelChoice);
        delete channelChoice;
        channelChoice = NULL;
    }

    layerChoice = makeLayerChoice(this, property.getParent().getIndex(), property.getLayerIndex());
    sizer->Add(layerChoice);

    deLayerStack& stack = property.getParent().getStack();
    deLayer* maskLayer = stack.getLayer(property.getLayerIndex());
    if (maskLayer)
    {
        deColorSpace colorSpace = maskLayer->getColorSpace();
        channelChoice = makeChannelChoice(this, colorSpace);
        channelChoice->SetSelection(property.getChannel());
        sizer->Add(channelChoice);
    }

    updateView();

    sizer->Layout();
}

void deMaskSelector::updateView()
{
    if (enabled->IsChecked())
    {
        layerChoice->Enable();
        channelChoice->Enable();
        property.enable();
    }
    else
    {
        layerChoice->Disable();
        channelChoice->Disable();
        property.disable();
    }
}

void deMaskSelector::choose(wxCommandEvent &event)
{
    property.setLayerIndex( layerChoice->GetCurrentSelection() );
    property.setChannel( channelChoice->GetCurrentSelection() );
    property.onUpdate();

    regenerateChannelChoice();
}

void deMaskSelector::check(wxCommandEvent &event)
{
    updateView();
}

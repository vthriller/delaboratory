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

#include "channels_selector.h"
#include "channel_check_boxes.h"
#include "property_channels.h"
#include "layer.h"
#include <iostream>

deChannelsSelector::deChannelsSelector(wxPanel* parent, dePropertyChannels& _property)
:wxPanel(parent), property(_property)
{
    sizer = new wxBoxSizer(wxVERTICAL); 
    SetSizer(sizer);
    rebuild();
    Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(deChannelsSelector::check));
}

deChannelsSelector::~deChannelsSelector()
{
}

void deChannelsSelector::rebuild()
{
    while (channels.size() > 0)
    {
        std::vector<wxCheckBox*>::iterator i = channels.begin();
        sizer->Detach(*i);
        delete *i;
        channels.erase(i);
    }
    createChannelCheckBoxes(this, property.getParent().getColorSpace(), sizer, channels, property.getChannels());
    Fit();
    GetParent()->Layout();
}

void deChannelsSelector::check(wxCommandEvent &event)
{
    property.clear();
    deColorSpace colorSpace = property.getParent().getColorSpace();
    int n = getColorSpaceSize(colorSpace);
    int i;
    for (i = 0; i < n; i++)
    {
        if (channels[i]->IsChecked())
        {
            property.insert(i);
        }
    }
    property.onUpdate();

}

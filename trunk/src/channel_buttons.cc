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

#include "channel_buttons.h"
#include "color_space.h"
#include "layer.h"

void addButton(wxPanel* parent, const std::string& label, int c, std::vector<wxRadioButton*>& buttons, std::map<int, int>& channels, wxSizer* sizer)
{
    wxRadioButton* button = new wxRadioButton(parent, wxID_ANY, wxString::FromAscii(label.c_str()), wxDefaultPosition, wxDefaultSize);
    sizer->Add(button, 0);
    buttons.push_back(button);

    channels[button->GetId()] = c;
}

void createChannelButtons(deColorSpace colorSpace, std::vector<wxRadioButton*>& buttons, std::map<int, int>& channels, wxPanel* parent, wxSizer* sizer, bool firstDisabled)
{
    std::vector<wxRadioButton*>::iterator i;
    for (i = buttons.begin(); i!= buttons.end(); i++)
    {
        sizer->Detach(*i);
        delete *i;
    }
    buttons.clear();
    channels.clear();


    int s = getColorSpaceSize(colorSpace);
    int j;

    std::string name;
    if (firstDisabled)
    {
        name = "disabled";
    }
    else
    {
        name = getColorSpaceName(colorSpace);
    }
    addButton(parent, name, -1, buttons, channels, sizer);

    if (s > 1)
    {
            for (j = 0; j < s; j++)
            {
                std::string channel = getChannelName(colorSpace, j);
                addButton(parent, channel, j, buttons, channels, sizer);
            }
    }

    sizer->Layout();
}    


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

#include "channel_check_boxes.h"

void createChannelCheckBoxes(wxWindow* parent, deColorSpace colorSpace, wxSizer* sizer, std::vector<wxCheckBox*>& channels, const std::set<int>& enabledChannels)
{
    int n = getColorSpaceSize(colorSpace);
    int i;
    channels.reserve(n);
    for (i = 0; i < n; i++)
    {
        wxCheckBox* cb = new wxCheckBox(parent, wxID_ANY, wxString::FromAscii(getChannelName(colorSpace, i).c_str()));
        cb->SetValue(enabledChannels.count(i) == 1);
        sizer->Add(cb, 0);
        channels[i] = cb;
    }
}



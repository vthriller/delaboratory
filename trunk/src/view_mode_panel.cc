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

#include "view_mode_panel.h"
#include "gui.h"
#include "layer.h"
#include "channel_buttons.h"
#include <iostream>

void deViewModePanel::select(wxCommandEvent &event)
{
    int i = event.GetId();
    int c = channels[i];

    if (c < 0)
    {
        gui.setViewMode(deViewNormal, -1);
    }
    else
    {
        gui.setViewMode(deViewSingleChannel, c);
    }
    gui.refreshView();

}

deViewModePanel::deViewModePanel(wxWindow* parent, deGUI& _gui)
:wxPanel(parent), gui(_gui)
{
    gui.setViewModePanel(this);

    sizer = new wxBoxSizer(wxHORIZONTAL);
    SetSizer(sizer);

    updateButtons(deColorSpaceRGB);

    Connect(wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(deViewModePanel::select));

}

deViewModePanel::~deViewModePanel()
{

}


void deViewModePanel::updateButtons(deColorSpace colorSpace)
{
    createChannelButtons(colorSpace, buttons, channels, this, sizer, false);
    gui.setViewMode(deViewNormal, -1);
    gui.refreshView();
}    


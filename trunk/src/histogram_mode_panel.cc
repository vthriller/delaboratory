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

#include "histogram_mode_panel.h"
#include "channel_buttons.h"
#include "gui.h"
#include "layer.h"

void deHistogramModePanel::select(wxCommandEvent &event)
{
    int i = event.GetId();
    int c = channels[i];

    if (c < 0)
    {
        gui.setHistogramMode(-1);
    }
    else
    {
        gui.setHistogramMode(c);
    }

}


deHistogramModePanel::deHistogramModePanel(wxWindow* parent, deGUI& _gui)
:wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(200,10)), gui(_gui)
{
    sizer = new wxBoxSizer(wxHORIZONTAL);
    SetSizer(sizer);

    updateButtons(deColorSpaceRGB);

    gui.setHistogramModePanel(this);

    Connect(wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(deHistogramModePanel::select));
}

deHistogramModePanel::~deHistogramModePanel()
{
}

void deHistogramModePanel::updateButtons(deColorSpace colorSpace)
{
    createChannelButtons(colorSpace, buttons, channels, this, sizer, true);
    gui.setHistogramMode(-1);
}

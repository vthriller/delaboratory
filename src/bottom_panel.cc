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

#include "bottom_panel.h"
#include "save_buttons_panel.h"
#include "info_bar_panel.h"

deBottomPanel::deBottomPanel(wxWindow* parent, deProject* project)
:wxPanel(parent)
{
//    SetBackgroundColour(*wxBLUE);

    wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

    wxPanel* saveButtonsPanel = new deSaveButtonsPanel(this, project);
    sizer->Add(saveButtonsPanel, 3, wxEXPAND);

    wxPanel* infoBarPanel = new deInfoBarPanel(this, project);
    sizer->Add(infoBarPanel, 3, wxEXPAND);

    SetSizer(sizer);

}

deBottomPanel::~deBottomPanel()
{
}

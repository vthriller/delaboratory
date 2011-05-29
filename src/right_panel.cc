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

#include "right_panel.h"
#include "layer_list_panel.h"
#include "histogram_panel.h"

deRightPanel::deRightPanel(wxWindow* parent, deProject* _project)
:wxPanel(parent), project(_project)
{
//    SetBackgroundColour(*wxLIGHT_GREY);

    wxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);

    wxPanel* histogramPanel = new deHistogramPanel(this, project);
    rightSizer->Add(histogramPanel, 0, wxEXPAND);


    deLayerListPanel* listbox = new deLayerListPanel(this, project);

    rightSizer->Add(listbox, 1, wxEXPAND);

    SetSizer(rightSizer);
}

deRightPanel::~deRightPanel()
{
}

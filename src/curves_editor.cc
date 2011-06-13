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

#include "curves_editor.h"
#include "curves_layer.h"
#include "curves_panel.h"
#include "channel_choice.h"

deCurvesEditor::deCurvesEditor(wxWindow *parent, deCurvesLayer& curvesLayer, dePreviewStack& _stack, int layerNumber)
:wxPanel(parent)
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    deColorSpace colorSpace = curvesLayer.getColorSpace();

    channelChoice = makeChannelChoice(this, colorSpace);

    sizer->Add(channelChoice, 0);

    wxPanel* bigPanel = new wxPanel(this);
    bigPanel->SetBackgroundColour(*wxBLACK);

    sizer->Add(bigPanel, 0);

    wxSizer* bigSizer = new wxBoxSizer(wxHORIZONTAL);
    bigPanel->SetSizer(bigSizer);

    curvesPanel = new deCurvesPanel(bigPanel, curvesLayer, _stack, layerNumber);

    bigSizer->Add(curvesPanel, 0, wxEXPAND | wxALL, 20);

    SetSizer(sizer);

    Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(deCurvesEditor::choose));
}

deCurvesEditor::~deCurvesEditor()
{
}

void deCurvesEditor::choose(wxCommandEvent &event)
{
    int b = channelChoice->GetCurrentSelection();
    curvesPanel->changeChannel(b);
}


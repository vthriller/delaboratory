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

#include "curves_frame.h"
#include "channel_choice.h"
#include "curves_layer.h"
#include "curves_panel.h"

deCurvesFrame::deCurvesFrame(wxWindow *parent, deCurvesLayer& _curvesLayer, dePreviewStack& _stack, int layerNumber, dePropertyCurves& _property)
:deActionFrame(parent, _curvesLayer, _T("curves")),
 curvesLayer(_curvesLayer), property(_property)
{

    property.setStack(_stack);

    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    deColorSpace colorSpace = curvesLayer.getColorSpace();

    channelChoice = makeChannelChoice(this, colorSpace);

    sizer->Add(channelChoice, 0);

    wxPanel* bigPanel = new wxPanel(this);
    bigPanel->SetBackgroundColour(*wxBLACK);

    sizer->Add(bigPanel, 0);

    wxSizer* bigSizer = new wxBoxSizer(wxHORIZONTAL);
    bigPanel->SetSizer(bigSizer);

    curvesPanel = new deCurvesPanel(bigPanel, curvesLayer, _stack, layerNumber, property);

    bigSizer->Add(curvesPanel, 0, wxEXPAND | wxALL, 20);

    wxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    sizer->Add(buttonSizer, 0);

    SetSizer(sizer);
    Fit();

    Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(deCurvesFrame::choose));
}

deCurvesFrame::~deCurvesFrame()
{
}

void deCurvesFrame::choose(wxCommandEvent &event)
{
    int b = channelChoice->GetCurrentSelection();
    curvesPanel->changeChannel(b);
}

void deCurvesFrame::traceSampler(deSampler* sampler)
{
    curvesPanel->traceSampler(sampler);
}

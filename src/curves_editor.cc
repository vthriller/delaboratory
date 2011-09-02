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
#include "preview_stack.h"
#include "project.h"

#define FILL_COUNT 10

deCurvesEditor::deCurvesEditor(wxWindow *parent, dePreviewStack& _stack, dePropertyCurves& _property)
:wxPanel(parent), property(_property), stack(_stack)
{
    stack.getProject()->logMessage("deCurvesEditor::deCurvesEditor");
    sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    channelChoice = NULL;
    bigPanel = NULL;
    curvesPanel = NULL;

    rebuild();

    Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(deCurvesEditor::choose));
}

void deCurvesEditor::rebuild()
{
    stack.getProject()->logMessage("deCurvesEditor::rebuild");

    deColorSpace colorSpace = property.getParent().getColorSpace();

    if (channelChoice)
    {
        sizer->Detach(channelChoice);
        delete channelChoice;
        channelChoice = NULL;
    }

    channelChoice = makeChannelChoice(this, colorSpace);
    sizer->Add(channelChoice, 0);


    if (bigPanel)
    {
        wxSizer* bigSizer = bigPanel->GetSizer();
        if (curvesPanel)
        {
            bigSizer->Detach(curvesPanel);
            delete curvesPanel;
            curvesPanel = NULL;
        }

        sizer->Detach(bigPanel);
        delete bigPanel;
        bigPanel = NULL;
    }

    bigPanel = new wxPanel(this);
    bigPanel->SetBackgroundColour(*wxBLACK);
    sizer->Add(bigPanel, 0);

    wxSizer* bigSizer = new wxBoxSizer(wxHORIZONTAL);
    bigPanel->SetSizer(bigSizer);

    int size;
    if (property.isHalf())
    {
        size = INITIAL_CURVES_PANEL_SIZE / 2;
    }
    else
    {
        size = INITIAL_CURVES_PANEL_SIZE;
    }

    curvesPanel = new deCurvesPanel(bigPanel,  stack, property, size);
    bigSizer->Add(curvesPanel, 0, wxEXPAND | wxALL, 20);

    wxButton* buttonReset = new wxButton(this, wxID_ANY, _T("reset"));
    Connect(buttonReset->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deCurvesEditor::reset));
    sizer->Add(buttonReset, 1, wxEXPAND);

    wxButton* buttonFill = new wxButton(this, wxID_ANY, _T("fill"));
    Connect(buttonFill->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deCurvesEditor::fill));
    sizer->Add(buttonFill, 1, wxEXPAND);

    wxButton* buttonSize = new wxButton(this, wxID_ANY, _T("change size"));
    Connect(buttonSize->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deCurvesEditor::size));
    sizer->Add(buttonSize, 1, wxEXPAND);

    sizer->Layout();

    Fit();

}

deCurvesEditor::~deCurvesEditor()
{
}

void deCurvesEditor::choose(wxCommandEvent &event)
{
    int b = channelChoice->GetCurrentSelection();
    curvesPanel->changeChannel(b);
}

void deCurvesEditor::traceSampler(deSampler* sampler)
{
    curvesPanel->traceSampler(sampler);
}

void deCurvesEditor::reset(wxCommandEvent &event)
{
    curvesPanel->reset();
}

void deCurvesEditor::fill(wxCommandEvent &event)
{
    curvesPanel->fill(FILL_COUNT);
}

void deCurvesEditor::size(wxCommandEvent &event)
{
    curvesPanel->changeSize();
}

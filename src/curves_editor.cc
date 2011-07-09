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
    curvesPanel = new deCurvesPanel(bigPanel,  stack, property);
    bigSizer->Add(curvesPanel, 0, wxEXPAND | wxALL, 20);

    sizer->Layout();

    Fit();

    Update();
    Refresh();

}

deCurvesEditor::~deCurvesEditor()
{
}

void deCurvesEditor::choose(wxCommandEvent &event)
{
    int b = channelChoice->GetCurrentSelection();
    curvesPanel->changeChannel(b);
}


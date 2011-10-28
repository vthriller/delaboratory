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
#include <iostream>
#include "color_space.h"
#include "project.h"
#include <iostream>

void deViewModePanel::select(wxCommandEvent &event)
{
    int i = event.GetId();
    deViewManager& viewManager = project.getViewManager();

    if (buttons[0]->GetId() == i)
    {
        viewManager.setNormal();
        return;
    }

    int j;
    for (j = 0; j < 4; j++)
    {
        if (buttons[j+1]->GetId() == i)
        {
            viewManager.setSingleChannel(j);
        }
    }

}

deViewModePanel::deViewModePanel(wxWindow* parent, deProject& _project)
:wxPanel(parent), project(_project)
{
    project.setViewModePanel(this);

    wxSizer* sizerS = new wxStaticBoxSizer(wxVERTICAL, this, _T("view"));
    SetSizer(sizerS);

    wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    sizerS->Add(sizer);

    int i;
    for (i = 0; i < 5; i++)
    {
        wxRadioButton* b = new wxRadioButton(this, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize);
        sizer->Add(b);
        buttons.push_back(b);

    }        

    updateNames();
    updateMode();

    Connect(wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(deViewModePanel::select));

}

deViewModePanel::~deViewModePanel()
{

}

void deViewModePanel::updateNames()
{
    deViewManager& viewManager = project.getViewManager();

    deColorSpace colorSpace = viewManager.getColorSpace();

    int i;
    int n = getColorSpaceSize(colorSpace);
    for (i = 0; i < 5; i++)
    {
        wxRadioButton* b = buttons[i];
        if (i == 0)
        {
            b->SetLabel(wxString::FromAscii(getColorSpaceName(colorSpace).c_str()));
            b->Show();
        }
        else 
        {
            int c = i - 1;
            if (c < n)
            {
                std::string name = getChannelName(colorSpace, c);
                b->SetLabel(wxString::FromAscii(name.c_str()));
                b->Show();
            }
            else
            {
                b->Hide();
            }
        }

    }        

    Layout();
    Fit();
    SetFocus();
}

void deViewModePanel::updateMode()
{
    deViewManager& viewManager = project.getViewManager();

    if (viewManager.isSingleChannel())
    {
        int c = viewManager.getChannel();
        buttons[c+1]->SetValue(1);
    }
    else
    {
        buttons[0]->SetValue(1);
    }
}


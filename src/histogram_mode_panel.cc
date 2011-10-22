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
#include <iostream>
#include "color_space.h"
#include "project.h"
#include <iostream>
#include "histogram_panel.h"

void deHistogramModePanel::select(wxCommandEvent &event)
{
    int i = event.GetId();
    deHistogramPanel* histogramPanel = project.getHistogramPanel();

    int j;
    for (j = 0; j < 4; j++)
    {
        if (buttons[j]->GetId() == i)
        {
            histogramPanel->setChannel(j);
            histogramPanel->generate();
            histogramPanel->paint();
        }
    }

}

deHistogramModePanel::deHistogramModePanel(wxWindow* parent, deProject& _project)
:wxPanel(parent), project(_project)
{
    project.setHistogramModePanel(this);

    wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    SetSizer(sizer);

    int i;
    for (i = 0; i < 4; i++)
    {
        wxRadioButton* b = new wxRadioButton(this, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize);
        sizer->Add(b);
        buttons.push_back(b);

    }        

    updateNames();
    updateMode();

    Connect(wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(deHistogramModePanel::select));
}

deHistogramModePanel::~deHistogramModePanel()
{

}

void deHistogramModePanel::updateNames()
{
    deViewManager& viewManager = project.getViewManager();
    deColorSpace colorSpace = viewManager.getColorSpace();

    int i;
    int n = getColorSpaceSize(colorSpace);
    for (i = 0; i < 4; i++)
    {
        wxRadioButton* b = buttons[i];
        if (i < n)
        {
            std::string name = getChannelName(colorSpace, i);
            b->SetLabel(wxString::FromAscii(name.c_str()));
            b->Show();
        }
        else
        {
            b->Hide();
        }
    }        

    Layout();
    Fit();
    SetFocus();
}

void deHistogramModePanel::updateMode()
{
    deHistogramPanel* histogramPanel = project.getHistogramPanel();

    int c = histogramPanel->getChannel();

    buttons[c]->SetValue(1);
}


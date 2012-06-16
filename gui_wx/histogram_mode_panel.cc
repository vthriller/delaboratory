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
#include "layer_processor.h"
#include "color_space_utils.h"
#include "gui.h"

void deHistogramModePanel::select(wxCommandEvent &event)
{
    int i = event.GetId();

    int j;
    for (j = 0; j < MAX_COLOR_SPACE_SIZE; j++)
    {
        if (buttons[j]->GetId() == i)
        {
            histogramPanel->setChannel(j);
            project.getLayerProcessor().generateHistogram();
        }
    }

}

deHistogramModePanel::deHistogramModePanel(wxWindow* parent, deProject& _project, deHistogramPanel* _histogramPanel, deGUI& gui)
:wxPanel(parent), project(_project), histogramPanel(_histogramPanel)
{
    gui.setHistogramModePanel(this);

    wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    SetSizer(sizer);

    int i;
    for (i = 0; i < MAX_COLOR_SPACE_SIZE; i++)
    {
        int style = 0;
        if (i == 0)
        {
            style = wxRB_GROUP;
        }
        wxRadioButton* b = new wxRadioButton(this, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, style);
        sizer->Add(b);
        buttons.push_back(b);

    }        

    updateNames();
    updateMode(0);

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
    for (i = 0; i < MAX_COLOR_SPACE_SIZE; i++)
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

void deHistogramModePanel::updateMode(int c)
{
    if ((c < 0) || (c >= MAX_COLOR_SPACE_SIZE))
    {
        return;
    }

    histogramPanel->setChannel(c);
    buttons[c]->SetValue(1);
}


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

#include "sampler_panel.h"
#include <sstream>
#include "sampler.h"
#include <iostream>
#include "gui.h"
#include <wx/sizer.h>
#include "project.h"

void deSamplerPanel::click(wxCommandEvent &event)
{
    int c = colorSpaceChoice->GetCurrentSelection();
    colorSpace = colorSpaces[c];

    gui.updateSamplers();
}

deSamplerPanel::deSamplerPanel(wxWindow* parent, deSampler* _sampler, deGUI& _gui)
:wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize), sampler(_sampler), gui(_gui)
{

    colorSpace = deColorSpaceRGB;

    getSupportedColorSpaces(colorSpaces);

    wxString* colorSpaceStrings = new wxString [colorSpaces.size()];
    unsigned int i;
    int selectedColorSpace = -1;
    for (i = 0; i < colorSpaces.size(); i++)
    {
        colorSpaceStrings[i] = wxString::FromAscii(getColorSpaceName(colorSpaces[i]).c_str());
        if (colorSpace == colorSpaces[i])
        {
            selectedColorSpace = i;
        }
    }        

    wxSizer* bigSizer = new wxBoxSizer(wxVERTICAL);
    colorSpaceChoice =  new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, colorSpaces.size(), colorSpaceStrings);
    bigSizer->Add(colorSpaceChoice, 0);
    wxPanel* insidePanel = new wxPanel(this);
    bigSizer->Add(insidePanel, 1, wxEXPAND | wxALL, 10);
    SetSizer(bigSizer);

    delete [] colorSpaceStrings;

    colorSpaceChoice->SetSelection(selectedColorSpace);

    cells = new wxStaticText * [2*4];

    wxSizer* sizer = new wxGridSizer(4, 2, 0, 0);
    insidePanel->SetSizer(sizer);

    cells[0 + 2 * 0] = new wxStaticText(insidePanel, wxID_ANY, _T("c:") );
    cells[1 + 2 * 0] = new wxStaticText(insidePanel, wxID_ANY, _T("c") );
    cells[0 + 2 * 1] = new wxStaticText(insidePanel, wxID_ANY, _T("m:") );
    cells[1 + 2 * 1] = new wxStaticText(insidePanel, wxID_ANY, _T("m") );
    cells[0 + 2 * 2] = new wxStaticText(insidePanel, wxID_ANY, _T("y:") );
    cells[1 + 2 * 2] = new wxStaticText(insidePanel, wxID_ANY, _T("y") );
    cells[0 + 2 * 3] = new wxStaticText(insidePanel, wxID_ANY, _T("k:") );
    cells[1 + 2 * 3] = new wxStaticText(insidePanel, wxID_ANY, _T("k") );

    for (i = 0; i < 2*4; i++)
    {
        sizer->Add(cells[i], 1, wxEXPAND);
    }

    setLabels(NULL);

    Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(deSamplerPanel::click));
}

deSamplerPanel::~deSamplerPanel()
{
}

void deSamplerPanel::setLabels(deProject* project)
{
    std::ostringstream oss;

    oss.setf(std::ios_base::fixed);

    int n = getColorSpaceSize(colorSpace);

    deValue values[4];

    bool conversion = true;

    if ((sampler->isEnabled()) && (project))
    {
        int source = project->getVisibleLayerID();
        dePreview* preview = project->getPreviewStack().getPreview(source);
        if (preview)
        {
            conversion = sampler->getPixel(values[0], values[1], values[2], values[3], colorSpace, *preview);
            if (!conversion)
            {
                n = 0;
            }
        }            
        else
        {
            n = 0;
        }
        Enable();
    }        
    else
    {
        n = 0;
        Disable();
    }

    oss.precision(3);

    int i;
    for (i = 0; i < n; i++)
    {
        cells[0 + 2 * i]->SetLabel(wxString::FromAscii(getChannelName(colorSpace, i).c_str()));

        oss.str("");
        deValue v = values[i];

        deValue vv = getPresentationValue(colorSpace, i, v);

        oss << vv;
        cells[1 + 2 * i]->SetLabel(wxString::FromAscii(oss.str().c_str()));
        cells[0 + 2 * i]->Show();
        cells[1 + 2 * i]->Show();
    }        
    for (i = n; i < 4; i++)
    {
        cells[0 + 2 * i]->Hide();
        cells[1 + 2 * i]->Hide();
    }

    if (!conversion)
    {
        cells[0]->SetLabel(_T("conversion"));
        cells[0]->Show();
        cells[2]->SetLabel(_T("not"));
        cells[2]->Show();
        cells[4]->SetLabel(_T("supported"));
        cells[4]->Show();
    }        
}

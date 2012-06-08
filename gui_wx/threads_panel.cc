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

#include "threads_panel.h"
#include "layer_processor.h"

deThreadsPanel::deThreadsPanel(wxWindow* parent)
:wxPanel(parent)
{
    //wxSizer* sizerP = new wxStaticBoxSizer(wxHORIZONTAL, this,  _T("thread activity"));
    wxSizer* sizerP = new wxStaticBoxSizer(wxHORIZONTAL, this,  _T("threads"));
    SetSizer(sizerP);

    //processingInfo = new wxStaticText(this, wxID_ANY, _T("[processing]"), wxDefaultPosition);
    processingInfo = new wxStaticText(this, wxID_ANY, _T("[p]"), wxDefaultPosition);
    sizerP->Add(processingInfo, 0, wxEXPAND);

    //renderingInfo = new wxStaticText(this, wxID_ANY, _T("[rendering]"), wxDefaultPosition);
    renderingInfo = new wxStaticText(this, wxID_ANY, _T("[r]"), wxDefaultPosition);
    sizerP->Add(renderingInfo, 0, wxEXPAND);

    //histogramInfo = new wxStaticText(this, wxID_ANY, _T("[histogram]"), wxDefaultPosition);
    histogramInfo = new wxStaticText(this, wxID_ANY, _T("[h]"), wxDefaultPosition);
    sizerP->Add(histogramInfo, 0, wxEXPAND);

    //dcrawInfo = new wxStaticText(this, wxID_ANY, _T("[dcraw]"), wxDefaultPosition);
    dcrawInfo = new wxStaticText(this, wxID_ANY, _T("[d]"), wxDefaultPosition);
    sizerP->Add(dcrawInfo, 0, wxEXPAND);
}

deThreadsPanel::~deThreadsPanel()
{

}

void deThreadsPanel::setInfoColor(int i)
{
    int e = 100;
    int e2 = 50;
    int d = 150;
    switch (i)
    {
        case DE_PROCESSING_START:
        {
            processingInfo->SetForegroundColour(wxColour(e, e, e));
            break;
        }
        case DE_PROCESSING_END:
        {
            processingInfo->SetForegroundColour(wxColour(d, d, d));
            break;
        }
        case DE_RENDERING_START:
        {
            renderingInfo->SetForegroundColour(wxColour(e, e, e));
            break;
        }
        case DE_RENDERING_END:
        {
            renderingInfo->SetForegroundColour(wxColour(d, d, d));
            break;
        }
        case DE_HISTOGRAM_START:
        {
            histogramInfo->SetForegroundColour(wxColour(e, e, e));
            break;
        }
        case DE_HISTOGRAM_END:
        {
            histogramInfo->SetForegroundColour(wxColour(d, d, d));
            break;
        }
        case DE_DCRAW_START:
        {
            dcrawInfo->SetForegroundColour(wxColour(e2, e2, e2));
            break;
        }
        case DE_DCRAW_END:
        {
            dcrawInfo->SetForegroundColour(wxColour(d, d, d));
            break;
        }
    }
}


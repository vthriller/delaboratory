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

#include "zoom_panel.h"
#include "project.h"

deZoomPanel::deZoomPanel(wxWindow* parent, deProject& _project)
:wxPanel(parent), project(_project)
{
    wxSizer* sizerS = new wxStaticBoxSizer(wxVERTICAL, this, _T("zoom"));
    SetSizer(sizerS);

    wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    sizerS->Add(sizer);

    realtime = new wxCheckBox(this, wxID_ANY, _T("realtime (slow)"), wxDefaultPosition, wxSize(-1, 25));
    sizer->Add(realtime, 0);
    realtime->Disable();

    full = new wxButton(this, wxID_ANY, _T("full zoom out"), wxDefaultPosition, wxSize(-1, 25));
    sizer->Add(full, 0);

    max = new wxButton(this, wxID_ANY, _T("big zoom"), wxDefaultPosition, wxSize(-1, 25));
    sizer->Add(max, 0);

    Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deZoomPanel::click));
}

deZoomPanel::~deZoomPanel()
{

}

void deZoomPanel::click(wxCommandEvent &event)
{
/*
    int id = event.GetId();

    if (full->GetId() == id)
    {
       project.fullZoomOut();
    }      

    if (max->GetId() == id)
    {
        project.maxZoom();
    }      
    */
}    


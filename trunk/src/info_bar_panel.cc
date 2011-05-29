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

#include "info_bar_panel.h"
#include "project.h"

deInfoBarPanel::deInfoBarPanel(wxWindow* parent, deProject* _project)
:wxPanel(parent), project(_project)
{
    wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

    int i;
    for (i = 0; i < 6; i++)
    {
        labels[i] = new wxStaticText(this, wxID_ANY, _T(""));
        sizer->Add(labels[i], 1, wxEXPAND);
    }        

    SetSizer(sizer);

    project->getGUI().setInfoBarPanel(this);


}

deInfoBarPanel::~deInfoBarPanel()
{
}

void deInfoBarPanel::setInfo(int index, const std::string s)
{
    if ((index < 0) || (index > 5))
    {
        return;
    }
    labels[index]->SetLabel(wxString::FromAscii(s.c_str()));
}


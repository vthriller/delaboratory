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

#include "warning_panel.h"

deWarningPanel::deWarningPanel(wxWindow* parent)
:wxPanel(parent)
{
    SetMinSize(wxSize(220,10));

    wxSizer* sizerP = new wxStaticBoxSizer(wxHORIZONTAL, this,  _T("status"));
    SetSizer(sizerP);

    warning = new wxStaticText(this, wxID_ANY, _T(""), wxDefaultPosition);
    sizerP->Add(warning, 0, wxEXPAND);

}

deWarningPanel::~deWarningPanel()
{

}

void deWarningPanel::setWarning(const std::string& w)
{
    warning->SetLabel(wxString::FromAscii(w.c_str()));

    if (w == "OK")
    {
        unsigned char g = 128;
        warning->SetForegroundColour(wxColour(g, g, g));
    }
    else
    {
        warning->SetForegroundColour(wxColour(255, 0, 0));
    }
}


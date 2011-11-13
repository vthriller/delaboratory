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

#include "choice.h"

deChoice::deChoice(wxWindow *parent, const std::string& labelString, const std::vector<std::string>& choices)
:wxPanel(parent)
{
    sizer = new wxBoxSizer(wxHORIZONTAL);
    SetSizer(sizer);
    
    label = new wxStaticText(this, wxID_ANY, wxString::FromAscii(labelString.c_str()), wxDefaultPosition, wxSize(80, 30));
    sizer->Add(label, 0, wxCENTER);

    wxString* ws = new wxString [choices.size()];
    unsigned int i;
    for (i = 0; i < choices.size(); i++)
    {
        ws[i] = wxString::FromAscii(choices[i].c_str());
    }        

    choice =  new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(200, -1), choices.size(), ws);
    sizer->Add(choice);

    Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(deChoice::choose));
}

deChoice::~deChoice()
{
}

void deChoice::choose(wxCommandEvent &event)
{
    int i = event.GetInt();
    onChoose(i);
}

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

#include "check_box.h"

deCheckBox::deCheckBox(wxWindow *parent, const std::string& labelString)
:wxPanel(parent)
{
    sizer = new wxBoxSizer(wxHORIZONTAL);
    SetSizer(sizer);
    
    checkBox =  new wxCheckBox(this, wxID_ANY, wxString::FromAscii(labelString.c_str()));
    sizer->Add(checkBox);

    Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(deCheckBox::check));
}

deCheckBox::~deCheckBox()
{
}

void deCheckBox::check(wxCommandEvent &event)
{
    if (checkBox->IsChecked())
    {
        onCheck(true);
    }
    else
    {
        onCheck(false);
    }
}

void deCheckBox::set(bool b)
{
    if (b)
    {
        checkBox->SetValue(1);
    }
    else
    {
        checkBox->SetValue(0);
    }
}

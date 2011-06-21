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

#include "name_editor.h"
#include "property_name.h"
#include <iostream>

deNameEditor::deNameEditor(wxPanel* parent, dePropertyName& _property)
:wxPanel(parent), property(_property)
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* labelName = new wxStaticText(this, wxID_ANY, _T("layer name:") );
    sizer->Add(labelName);
    edit =  new wxTextCtrl(this, wxID_ANY, wxString::FromAscii(property.getName().c_str()), wxDefaultPosition, wxSize(200, -1), wxTE_PROCESS_ENTER);
    sizer->Add(edit);

    SetSizer(sizer);

    Connect(wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(deNameEditor::textEnter));
}    

deNameEditor::~deNameEditor()
{
}


void deNameEditor::textEnter(wxCommandEvent &event)
{
    wxString valueStr = edit->GetValue();
    std::string name(valueStr.mb_str());
    property.setName(name);
    property.onListUpdate();
}

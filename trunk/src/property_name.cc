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

#include "property_name.h"
#include <wx/wx.h>

dePropertyName::dePropertyName(deLayer& _parent)
:deProperty(_parent)
{
    name = "";
}

dePropertyName::~dePropertyName()
{
}

void dePropertyName::setName(const std::string& _name)
{
    name = _name;
}

void dePropertyName::addPanelContent(wxPanel* panel, wxSizer* sizer, dePreviewStack& stack)
{
    // FIXME name editor
    /*
    wxStaticText* labelName = new wxStaticText(panel, wxID_ANY, _T("layer name:") );
    sizer->Add(labelName);
    editName =  new wxTextCtrl(panel, wxID_ANY, wxString::FromAscii(name.c_str()), wxDefaultPosition, wxSize(200, -1));
    sizer->Add(editName);

    panel->Connect(wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(dePropertyName::textEnter));
    */
}    

void dePropertyName::textEnter(wxCommandEvent &event)
{
/*
    wxString valueStr = editName->GetValue();
    name = valueStr.mb_str();
*/
}

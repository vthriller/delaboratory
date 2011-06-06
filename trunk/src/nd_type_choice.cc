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

#include "nd_type_choice.h"

wxChoice* makeNDTypeChoice(wxWindow* parent, std::vector<deNDType>& ndTypes, deNDType currentNDType)
{
    getSupportedNDTypes(ndTypes);

    wxString* ndTypeStrings = new wxString [ndTypes.size()];
    unsigned int i;
    int selectedNDType;
    for (i = 0; i < ndTypes.size(); i++)
    {
        ndTypeStrings[i] = wxString::FromAscii(getNDTypeName(ndTypes[i]).c_str());
        if (currentNDType == ndTypes[i])
        {
            selectedNDType = i;
        }
    }        

    wxChoice* choice =  new wxChoice(parent, wxID_ANY, wxDefaultPosition, wxSize(200, -1), ndTypes.size(),ndTypeStrings);
    choice->SetSelection(selectedNDType);

    delete [] ndTypeStrings;

    return choice;
}


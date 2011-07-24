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

#include "blur_type_choice.h"

wxChoice* makeBlurTypeChoice(wxWindow* parent, std::vector<deBlurType>& ndTypes, deBlurType currentBlurType)
{
    getSupportedBlurTypes(ndTypes);

    wxString* ndTypeStrings = new wxString [ndTypes.size()];
    unsigned int i;
    int selectedBlurType;
    for (i = 0; i < ndTypes.size(); i++)
    {
        ndTypeStrings[i] = wxString::FromAscii(getBlurTypeName(ndTypes[i]).c_str());
        if (currentBlurType == ndTypes[i])
        {
            selectedBlurType = i;
        }
    }        

    wxChoice* choice =  new wxChoice(parent, wxID_ANY, wxDefaultPosition, wxSize(200, -1), ndTypes.size(),ndTypeStrings);
    choice->SetSelection(selectedBlurType);

    delete [] ndTypeStrings;

    return choice;
}


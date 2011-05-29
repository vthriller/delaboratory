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

#include "color_space_choice.h"

wxChoice* makeColorSpaceChoice(wxWindow* parent, std::vector<deColorSpace>& colorSpaces, deColorSpace currentColorSpace)
{
    int selectedColorSpace = -1;

    getSupportedColorSpaces(colorSpaces);

    wxString* colorSpaceStrings = new wxString [colorSpaces.size()];
    unsigned int i;
    for (i = 0; i < colorSpaces.size(); i++)
    {
        colorSpaceStrings[i] = wxString::FromAscii(getColorSpaceName(colorSpaces[i]).c_str());
        if (currentColorSpace == colorSpaces[i])
        {
            selectedColorSpace = i;
        }
    }        

    wxChoice *choice =  new wxChoice(parent, wxID_ANY, wxDefaultPosition, wxSize(200, -1), colorSpaces.size(), colorSpaceStrings);
    choice->SetSelection(selectedColorSpace);

    delete [] colorSpaceStrings;

    return choice;

}            


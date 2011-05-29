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

#include "blend_mode_choice.h"

wxChoice* makeBlendModeChoice(wxWindow* parent, std::vector<deBlendMode>& blendModes, deBlendMode currentBlendMode)
{
    getSupportedBlendModes(blendModes);

    wxString* blendModeStrings = new wxString [blendModes.size()];
    unsigned int i;
    int selectedBlendMode;
    for (i = 0; i < blendModes.size(); i++)
    {
        blendModeStrings[i] = wxString::FromAscii(getBlendModeName(blendModes[i]).c_str());
        if (currentBlendMode == blendModes[i])
        {
            selectedBlendMode = i;
        }
    }        

    wxChoice* choice =  new wxChoice(parent, wxID_ANY, wxDefaultPosition, wxSize(200, -1), blendModes.size(), blendModeStrings);
    choice->SetSelection(selectedBlendMode);

    delete [] blendModeStrings;

    return choice;
}


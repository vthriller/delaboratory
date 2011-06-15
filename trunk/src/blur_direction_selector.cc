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

#include "blur_direction_selector.h"
#include "property_blur_direction.h"

deBlurDirectionSelector::deBlurDirectionSelector(wxPanel* parent, dePropertyBlurDirection& _property)
:wxPanel(parent), property(_property)
{
    wxString blurDirectionStrings[2];
    blurDirectionStrings[0] = _T("horizontal");
    blurDirectionStrings[1] = _T("vertical");

    blurDirectionChoice =  new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(200, -1), 2, blurDirectionStrings);
    if (property.getDirection() == deBlurHorizontal)
    {
        blurDirectionChoice->SetSelection(0);
    }
    else
    {
        blurDirectionChoice->SetSelection(1);
    }

    Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(deBlurDirectionSelector::choose));
}

deBlurDirectionSelector::~deBlurDirectionSelector()
{
}

void deBlurDirectionSelector::choose(wxCommandEvent &event)
{
    int c = blurDirectionChoice->GetCurrentSelection();
    if (c == 0)
    {
        property.setDirection(deBlurHorizontal);
    }
    else
    {
        property.setDirection(deBlurVertical);
    }
}

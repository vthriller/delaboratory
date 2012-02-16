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

#include "dcraw_support.h"
#include <wx/wx.h>

#define DCRAW_EXECUTABLE "dcraw"

std::string checkDcraw()
{
    const wxString command = wxString::FromAscii(DCRAW_EXECUTABLE);
    wxArrayString output;
    wxExecute(command, output);

    int c = output.GetCount();

    if (c < 2)
    {
        return "";
    }

    wxString line1 = output.Item(1);
    std::string l1(line1.ToAscii());

    int p = l1.rfind("v");

    std::string vs = l1.substr(p + 1);

    return vs;

}


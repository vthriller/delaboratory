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

#include "help_color_spaces_frame6.h"
#include "color_space.h"
#include "gradient_panel.h"
#include <sstream>
#include "conversion_functions.h"
#include <map>
#include <iostream>

const int LAB_COLORS_COUNT=10;
const int LAB_COLORS_ELEMENTS = (2 * LAB_COLORS_COUNT + 1);

void generateAB(wxWindow* window, wxSizer* sizer, int scale)
{
    int w = 40;
    int h = 40;

    int b;
    int a;

    sizer->Add(-1, 0, wxCENTER);

    for (a = -LAB_COLORS_COUNT; a <= LAB_COLORS_COUNT; a++)
    {
        int sa = scale * a;
        std::ostringstream oss;
        oss << sa;
        wxStaticText* la = new wxStaticText(window, wxID_ANY, wxString::FromAscii(oss.str().c_str()));
        sizer->Add(la, 1, wxCENTER);
    }            

    for (b = LAB_COLORS_COUNT; b >= -LAB_COLORS_COUNT; b--)
    {
        int sb = scale * b;

        std::ostringstream oss;
        oss << sb;
        wxStaticText* lb = new wxStaticText(window, wxID_ANY, wxString::FromAscii(oss.str().c_str()));
        sizer->Add(lb, 0, wxCENTER);

        deValue bb = (sb + 100.0) / 200.0;
        for (a = -LAB_COLORS_COUNT; a <= LAB_COLORS_COUNT; a++)
        {
            int sa = scale * a;
            deValue aa = (sa + 100.0) / 200.0;
            
            deGradientPanel2* gradient = new deGradientPanel2(window, wxSize(w, h), deColorSpaceLAB, aa, bb, -1, -1);
            sizer->Add(gradient, 0, wxCENTER);
        }
    }

    sizer->Layout();
}

deHelpColorSpacesFrame6::deHelpColorSpacesFrame6(wxWindow *parent, int scale)
:deHelpFrame(parent)
{
    wxSizer* sizer = new wxFlexGridSizer(1 + LAB_COLORS_ELEMENTS, 0, 0);
    SetSizer(sizer);

    generateAB(this, sizer, scale);

    Fit();

}

deHelpColorSpacesFrame6::~deHelpColorSpacesFrame6()
{
}


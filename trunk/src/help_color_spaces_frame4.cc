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

#include "help_color_spaces_frame4.h"
#include "color_space.h"
#include "gradient_panel.h"
#include <sstream>

void add2(deColorSpace colorSpace, wxWindow* parent, wxSizer* sizer)
{
    wxSizer* sizerB = new wxStaticBoxSizer(wxHORIZONTAL, parent,  wxString::FromAscii(getColorSpaceName(colorSpace).c_str()));
    sizer->Add(sizerB);

    wxSizer* sizerG = new wxGridSizer(6);
    sizerB->Add(sizerG);

    int w = 120;
    int h = 50;

    int n = 18;
    deValue step = 1.0 / n;
    deValue l = 0;

    int i;
    for (i = 0; i < n; i++)
    {
        std::ostringstream oss;

        int hue;

        hue = 360 * l;

        oss << hue;
        wxSizer* sizerBC= new wxStaticBoxSizer(wxHORIZONTAL, parent,  wxString::FromAscii(oss.str().c_str()));
        sizerG->Add(sizerBC);

        int c1;
        int c2;
        if (colorSpace == deColorSpaceLCH)
        {
            c1 = 0;
            c2 = 1;
        }
        else
        {
            c1 = 1;
            c2 = 2;
        }

        deGradientPanel1* gradient = new deGradientPanel1(parent, wxSize(w, h), colorSpace, c1, c2, -1, l, -1);
        sizerBC->Add(gradient, 0, wxCENTER);

        l += step;

    }


}

deHelpColorSpacesFrame4::deHelpColorSpacesFrame4(wxWindow *parent)
:deHelpFrame(parent, "hue")
{
    wxSizer* sizer = new wxGridSizer(1);
    SetSizer(sizer);

    add2(deColorSpaceLCH, this, sizer);
    add2(deColorSpaceHSL, this, sizer);
    add2(deColorSpaceHSV, this, sizer);

    Fit();

}

deHelpColorSpacesFrame4::~deHelpColorSpacesFrame4()
{
}


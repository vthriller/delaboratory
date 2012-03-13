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

#include "help_color_spaces_frame3.h"
#include "color_space.h"
#include "gradient_panel.h"
#include <sstream>

void add(deColorSpace colorSpace, wxWindow* parent, wxSizer* sizer)
{
    wxSizer* sizerB = new wxStaticBoxSizer(wxHORIZONTAL, parent,  wxString::FromAscii(getColorSpaceName(colorSpace).c_str()));
    sizer->Add(sizerB);

    int w = 160;
    int h = 160;

    deValue max = 1.0;
    int n = 6;
    deValue min = max - n * (0.25/2.0);
    deValue step = (max-min) / (n );

    deValue light;

    for (light = min; light <= max; light+= step)
    {
        deValue l = light * 100;

        std::ostringstream oss;
        oss << l;
        wxSizer* sizerBC= new wxStaticBoxSizer(wxHORIZONTAL, parent,  wxString::FromAscii(oss.str().c_str()));
        sizerB->Add(sizerBC);

        int c1;
        int c2;
        if ((colorSpace == deColorSpaceLAB) || (colorSpace == deColorSpaceLCH))
        {
            c1 = 1;
            c2 = 2;
        }
        else
        {
            c1 = 1;
            c2 = 0;
        }

        deGradientPanel1* gradient = new deGradientPanel1(parent, wxSize(w, h), colorSpace, c1, c2, light, -1, -1);
        sizerBC->Add(gradient, 0, wxCENTER);

    }

}

deHelpColorSpacesFrame3::deHelpColorSpacesFrame3(wxWindow *parent)
:deHelpFrame(parent, "lightness / value")
{
    wxSizer* sizer = new wxGridSizer(1);
    SetSizer(sizer);

    add(deColorSpaceLAB, this, sizer);
    add(deColorSpaceLCH, this, sizer);
    add(deColorSpaceHSL, this, sizer);
    add(deColorSpaceHSV, this, sizer);

    Fit();

}

deHelpColorSpacesFrame3::~deHelpColorSpacesFrame3()
{
}


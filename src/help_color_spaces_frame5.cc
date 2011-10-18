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

#include "help_color_spaces_frame5.h"
#include "color_space.h"
#include "gradient_panel.h"
#include <sstream>
#include "conversion_functions.h"
#include <map>
#include <iostream>

#include "skin_color_chart.h"

void generateFelixVonLuschan(wxWindow* window, wxSizer* sizer)
{
    int w = 240;
    int h = 30;

    std::vector<deSkinRGB> skins;
    getFelixVonLuschan(skins);

    std::vector<deSkinRange> ranges;
    
    getSkinRanges(ranges);

    deConversion3x4 f = getConversion3x4(deColorSpaceRGB, deColorSpaceCMYK);

    std::vector<deSkinRange>::iterator r;
    for (r = ranges.begin(); r != ranges.end(); r++)
    {
        deSkinRange range = *r;

        std::ostringstream oss;
        oss << range.description;
        wxSizer* sizer_B = new wxStaticBoxSizer(wxVERTICAL, window,  wxString::FromAscii(oss.str().c_str()));
        sizer->Add(sizer_B);

        int i;

        for (i = range.first - 1; i <= range.last - 1; i++)
        {
            deSkinRGB& skin = skins[i];
            deValue cc;
            deValue mm;
            deValue yy;
            deValue kk;

            f(skin.r / 255.0, skin.g / 255.0, skin.b / 255.0, cc, mm, yy, kk);

            int c = 100 * cc;
            int m = 100 * mm;
            int y = 100 * yy;
            int k = 100 * kk;

            std::ostringstream oss;
            oss << "C: " << c << " M: " << m << " Y: " << y << " K: " << k;
            wxSizer* sizer_S = new wxStaticBoxSizer(wxHORIZONTAL, window,  wxString::FromAscii(oss.str().c_str()));
            sizer_B->Add(sizer_S);

            deGradientPanel2* gradient = new deGradientPanel2(window, wxSize(w, h), deColorSpaceCMYK, cc, mm, yy, kk);
            sizer_S->Add(gradient, 0, wxCENTER);
        }

    }

}

deHelpColorSpacesFrame5::deHelpColorSpacesFrame5(wxWindow *parent)
:deHelpFrame(parent)
{
    wxSizer* sizer = new wxFlexGridSizer(3);
    SetSizer(sizer);

    generateFelixVonLuschan(this, sizer);

    Fit();

}

deHelpColorSpacesFrame5::~deHelpColorSpacesFrame5()
{
}


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

void generateAllSkins(wxWindow* window, wxSizer* sizer)
{
    std::map<deValue, deSkinCMYK> skins;

    //deConversion4x3 f = getConversion4x3(deColorSpaceCMYK, deColorSpaceLAB);
    deConversion4x3 f = getConversion4x3(deColorSpaceCMYK, deColorSpaceHSV);

    int w = 100;
    int h = 40;

    int c;
    int m;
    int y;

    int min_m = 15;
    int limit_y = 70;
    int limit_m = 65;

    int step_m = 5;
    int step_y = 5;
    int step_c = 6;

    for (m = min_m; m <= limit_m; m += step_m)
    {
        int max_y = m * 2.2;
        if (max_y > limit_y)
        {
            max_y = limit_y;
        }

        for (y = m; y <= max_y; y += step_y)
        {

            int min_c = m / 6;
            int max_c = m / 2.2;

            for (c = min_c; c <= max_c; c += step_c)
            {

                deValue h;
                deValue s;
                deValue v;
                f(c / 100.0, m / 100.0, y / 100.0, 0.9, h, s, v);

//              std::cout << "a: " << a << " b: " << b << std::endl;

                deSkinCMYK skin;
                skin.c = c;
                skin.m = m;
                skin.y = y;

                deValue hash = 1000 * h + v + s / 1000.0;

                skins[hash] = skin;
            }                
        }

    }        

    std::map<deValue, deSkinCMYK>::iterator i;

    for (i = skins.begin(); i != skins.end(); i++)
    {

        deSkinCMYK skin = i->second;

        int c = skin.c;
        int m = skin.m;
        int y = skin.y;

        deValue mm = m / 100.0;
        deValue yy = y / 100.0;
        deValue cc = c / 100.0;

        std::ostringstream oss;
        oss << "C" << c << " M" << m << " Y" << y;
        wxSizer* sizer_B = new wxStaticBoxSizer(wxHORIZONTAL, window,  wxString::FromAscii(oss.str().c_str()));
        sizer->Add(sizer_B);

        deGradientPanel* gradient = new deGradientPanel(window, wxSize(w, h), deColorSpaceCMYK, 3, -1, cc, mm, yy);
        sizer_B->Add(gradient, 0, wxCENTER);
    }
}

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

//    generateAllSkins(this, sizer);
    generateFelixVonLuschan(this, sizer);

    Fit();

}

deHelpColorSpacesFrame5::~deHelpColorSpacesFrame5()
{
}


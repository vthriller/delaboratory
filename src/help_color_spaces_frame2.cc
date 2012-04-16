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

#include "help_color_spaces_frame2.h"
#include "color_space.h"
#include "gradient_panel.h"
#include "color_space_utils.h"

deHelpColorSpacesFrame2::deHelpColorSpacesFrame2(wxWindow *parent)
:deHelpFrame(parent, "mix of channels")
{
    wxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(mainSizer);

    wxSizer* sizer1 = new wxGridSizer(2);
    mainSizer->Add(sizer1);

    std::vector<deColorSpace> colorSpaces;
    getSupportedColorSpaces(colorSpaces);

    int width = 140;
    int barSize = 140;

    std::vector<deColorSpace>::iterator i;

    for (i = colorSpaces.begin(); i != colorSpaces.end(); i++)
    {
        deColorSpace c = *i;

        int n = getColorSpaceSize(c);

        if (n == 3)
        {

            wxSizer* sizerB = new wxStaticBoxSizer(wxHORIZONTAL, this,  wxString::FromAscii(getColorSpaceName(c).c_str()));
            sizer1->Add(sizerB);

            int j;
            for (j = 0; j < n; j++)
            {
                int k = j + 1;
                if (k >= n)
                {
                    k = 0;
                }
        
                std::string s = getChannelName(c, j) + " / " + getChannelName(c, k);
                wxSizer* sizerBC= new wxStaticBoxSizer(wxHORIZONTAL, this,  wxString::FromAscii(s.c_str()));
                sizerB->Add(sizerBC);

                deGradientPanel1* gradient = new deGradientPanel1(this, wxSize(width, barSize), c, j, k, -1, -1, -1);
                sizerBC->Add(gradient, 0, wxCENTER);
            }

        }
    }

    wxSizer* sizer2 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(sizer2);

    for (i = colorSpaces.begin(); i != colorSpaces.end(); i++)
    {
        deColorSpace c = *i;

        int n = getColorSpaceSize(c);

        if (n == 4)
        {

            wxSizer* sizerB = new wxStaticBoxSizer(wxHORIZONTAL, this,  wxString::FromAscii(getColorSpaceName(c).c_str()));
            sizer2->Add(sizerB);

            int j;
            for (j = 0; j < n; j++)
            {
                int k;
                for (k = j + 1; k < n; k++)
                {
                    std::string s = getChannelName(c, j) + " / " + getChannelName(c, k);
                    wxSizer* sizerBC= new wxStaticBoxSizer(wxHORIZONTAL, this,  wxString::FromAscii(s.c_str()));
                    sizerB->Add(sizerBC);

                    deGradientPanel1* gradient = new deGradientPanel1(this, wxSize(width, barSize), c, j, k, -1, -1, -1);
                    sizerBC->Add(gradient, 0, wxCENTER);
                }                    
            }

        }
    }

    Fit();

}

deHelpColorSpacesFrame2::~deHelpColorSpacesFrame2()
{
}


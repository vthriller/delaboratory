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

#include "help_color_spaces_frame.h"
#include "color_space.h"
#include "gradient_panel.h"
#include "color_space_utils.h"

deHelpColorSpacesFrame::deHelpColorSpacesFrame(wxWindow *parent)
:deHelpFrame(parent, "channels")
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    std::vector<deColorSpace> colorSpaces;
    getSupportedColorSpaces(colorSpaces);

    int width = 256;
    int barSize = 40;

    std::vector<deColorSpace>::iterator i;
    for (i = colorSpaces.begin(); i != colorSpaces.end(); i++)
    {
        deColorSpace c = *i;

        wxSizer* sizerB = new wxStaticBoxSizer(wxHORIZONTAL, this,  wxString::FromAscii(getColorSpaceName(c).c_str()));
        sizer->Add(sizerB);

        int n = getColorSpaceSize(c);
        int j;
        for (j = 0; j < n; j++)
        {
            wxSizer* sizerBC= new wxStaticBoxSizer(wxHORIZONTAL, this,  wxString::FromAscii(getChannelName(c, j).c_str()));
            sizerB->Add(sizerBC);

            deGradientPanel1* gradient = new deGradientPanel1(this, wxSize(width, barSize), c, j, -1, -1, -1, -1);
            sizerBC->Add(gradient, 0, wxCENTER);
        }
    }

    Fit();

}

deHelpColorSpacesFrame::~deHelpColorSpacesFrame()
{
}


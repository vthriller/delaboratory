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

#include "palette_frame.h"
#include "gradient_panel.h"

dePaletteFrame::dePaletteFrame(wxWindow *parent)
:deHelpFrame(parent, "palette frame")
{
    int tilesW = 20;
    int s = 1;
    int px = 40;
    int py = 40;

    wxSizer* sizer = new wxFlexGridSizer(tilesW, s, s);
    SetSizer(sizer);

    int i;
    for (i = 0; i < 100; i++)
    {
        deColorPanelOld* colorPanel = new deColorPanelOld(this, wxSize(px, py), 0);
        sizer->Add(colorPanel);

        deValue r = (rand() % 1000) / 1000.0;
        deValue g = (rand() % 1000) / 1000.0;
        deValue b = (rand() % 1000) / 1000.0;

        colorPanel->setRGB(r, g, b);
    }

    Fit();

}

dePaletteFrame::~dePaletteFrame()
{
}


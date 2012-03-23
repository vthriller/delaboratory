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

#include "color_matrix_frame.h"
#include "gradient_panel.h"
#include "project.h"
#include "layer_stack.h"
#include "layer.h"
#include "channel_manager.h"
#include "conversion_functions.h"

deColorMatrixFrame::deColorMatrixFrame(wxWindow *parent, deProject& _project, int tileRW, int tileRH, int tileW, int tileH, int palW, int palH, int palSize, deValue margin)
:deHelpFrame(parent, "color matrix"), project(_project)
{
    wxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(mainSizer);

    palette = NULL;
    palette2 = NULL;

    const deViewManager& viewManager = project.getViewManager();
    int view = viewManager.getView();

    deLayerStack& layerStack = project.getLayerStack();
    deLayer* layer = layerStack.getLayer(view);

    if (!layer)
    {
        return;
    }

    const deImage& image = layer->getImage();
    deColorSpace colorSpace = image.getColorSpace();

    deConversion3x3 f3x3 = getConversion3x3(colorSpace, deColorSpaceRGB);
    if (!f3x3)
    {
        return;
    }

    palette = new dePalette3(colorSpace);

    deChannelManager& channelManager = project.getPreviewChannelManager();
    deSize channelSize = channelManager.getChannelSize();

    int cs = channelSize.getW();

    int w = channelSize.getW() / tileW;
    int h = channelSize.getH() / tileH;

    int n = w * h;

    int s = 2;

    wxSizer* sizer = new wxFlexGridSizer(w, s, s);
    mainSizer->Add(sizer);

    int i;

    int y = 0;
    int x = 0;

    for (i = 0; i < n; i++)
    {
        deColorPanel* colorPanel = new deColorPanel(this, wxSize(tileRW, tileRH));
        sizer->Add(colorPanel);

        int x1 = x * tileW;
        int x2 = (x + 1) * tileW;
        int y1 = y * tileH;
        int y2 = (y + 1) * tileH;

        deValue sum1 = 0;
        deValue sum2 = 0;
        deValue sum3 = 0;

        deValue v1;
        deValue v2;
        deValue v3;

        int xx;
        int yy;

        for (yy = y1; yy < y2; yy++)
        {
            int p = cs * yy + x1;
            for (xx = x1; xx < x2; xx++)
            {
                image.getPixel(0, p, v1);
                image.getPixel(1, p, v2);
                image.getPixel(2, p, v3);
                sum1 += v1;
                sum2 += v2;
                sum3 += v3;
                p++;
            }
        }

        int ts = tileW * tileH;

        sum1 /= ts;
        sum2 /= ts;
        sum3 /= ts;

        palette->addColor(deColor3(sum1, sum2, sum3));

        deValue r;
        deValue g;
        deValue b;

        f3x3(sum1, sum2, sum3, r, g, b);

        colorPanel->setRGB(r, g, b);

        x++;
        if (x == w)
        {
            x = 0;
            y++;
        }
    }

    palette2 = new dePalette3(colorSpace);

    palette2->optimize(*palette, palSize, margin);

    int s2 = 5;

    wxSizer* sizerS = new wxStaticBoxSizer(wxHORIZONTAL, this,  _T("palette"));
    mainSizer->Add(sizerS);
    
    wxSizer* paletteSizer = new wxFlexGridSizer(palSize / 2, s2, s2);
    sizerS->Add(paletteSizer);
    for (i = 0; i < palette2->getSize(); i++)
    {
        deColorPanel* colorPanel = new deColorPanel(this, wxSize(palW, palH));
        paletteSizer->Add(colorPanel);

        deValue r;
        deValue g;
        deValue b;

        deColor3 c = palette2->getColor(i);

        f3x3(c.getV1(), c.getV2(), c.getV3(), r, g, b);

        colorPanel->setRGB(r, g, b);
    }

    Fit();

}

deColorMatrixFrame::~deColorMatrixFrame()
{
    if (palette)
    {
        delete palette;
    }        
    if (palette2)
    {
        delete palette2;
    }        
}


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
#include "color_matrix.h"
#include "gradient_panel.h"
#include "project.h"
#include "layer_stack.h"
#include "layer.h"
#include "channel_manager.h"
#include "conversion_functions.h"
#include "wx/notebook.h"
#include "conversion_functions.h"
#include "conversion_processor.h"

#define LAB_TILE_SIZE 20
#define ALL_TILES_SIZE 800

deColorMatrixFrame2::deColorMatrixFrame2(wxWindow *parent, deProject& project, int channelHorizontal, int channelVertical, int channelAverage, int width, int height, dePalette3* palette)
:deHelpFrame(parent, "color matrix")
{

    const deViewManager& viewManager = project.getViewManager();
    int view = viewManager.getView();

    deLayerStack& layerStack = project.getLayerStack();

    deBaseLayer* layer = layerStack.getLayer(view);

    if (!layer)
    {
        return;
    }

    const deImage& originalImage = layer->getLayerImage();

    deChannelManager& channelManager = project.getPreviewChannelManager();
    deSize channelSize = originalImage.getChannelSize();
    int n = channelSize.getN();

    deImage LABImage(deColorSpaceLAB, channelManager);
    LABImage.enableAllChannels();

    deConversionProcessor p;
    p.convertImage(originalImage, LABImage, channelManager);

    int nn = width * height;

    //int min = (n / 2) / nn;
    int min = (n / 1.5) / nn;

    const deValue* valuesVertical = LABImage.getValues(channelVertical);
    const deValue* valuesHorizontal = LABImage.getValues(channelHorizontal);
    const deValue* valuesAverage = LABImage.getValues(channelAverage);

    int tilesW = width;
    int tilesH = height;

    deColorMatrix colorMatrix(tilesW , tilesH);

    colorMatrix.buildZoomed(valuesHorizontal, valuesVertical, valuesAverage, n, min);

    wxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(mainSizer);

    //int s = 2;
    int s = 0;

    wxSizer* LABMapSizer = new wxFlexGridSizer(tilesW, s, s);
    mainSizer->Add(LABMapSizer);

    int x;
    int y;
    //for (y = 0; y < tilesH; y++)
    for (y = tilesH - 1; y >= 0; y--)
    {
        for (x = 0; x < tilesW; x++)
        {
            int style = 0;

            deValue vx = 0;
            deValue vy = 0;
            bool center;
            deValue a = colorMatrix.get(x, y, min, vx, vy, center);

            style = wxBORDER_NONE;
            if ((center) && (a < 0))
            {
                style = wxBORDER_SIMPLE;
            }

            deColorPanel* colorPanel = new deColorPanel(this, wxSize(ALL_TILES_SIZE / width, ALL_TILES_SIZE / height), palette, style);
            LABMapSizer->Add(colorPanel);

            if (a >= 0)
            {

                deValue v1 = 0;
                deValue v2 = 0;
                deValue v3 = 0;
                switch (channelHorizontal)
                {
                    case 0:
                        v1 = vx;
                        break;
                    case 1:
                    {
                        v2 = vx;
                        break;
                    }                        
                    case 2:
                        v3 = vx;
                        break;
                    default:
                        break;
                }
                switch (channelVertical)
                {
                    case 0:
                        v1 = vy;
                        break;
                    case 1:
                        v2 = vy;
                        break;
                    case 2:
                        v3 = vy;
                        break;
                    default:
                        break;
                }
                switch (channelAverage)
                {
                    case 0:
                        v1 = a;
                        break;
                    case 1:
                        v2 = a;
                        break;
                    case 2:
                        v3 = a;
                        break;
                    default:
                        break;
                }

                deValue r;
                deValue g;
                deValue b;

                lab2rgb(v1, v2, v3, r, g, b);
                colorPanel->setRGB(r, g, b);
            }                
            else
            {
//                colorPanel->setRGB(0, 0, 0);
            }

        }
    }     
    Fit();
}

deColorMatrixFrame2::~deColorMatrixFrame2()
{
}

#define LAB_TILES 20

deColorMatrixFrame::deColorMatrixFrame(wxWindow *parent, deProject& _project, int tileRW, int tileRH, int tileW, int tileH, int palW, int palH, int palSize, deValue margin)
:deHelpFrame(parent, "color matrix"), project(_project)
{
    wxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(mainSizer);

    wxNotebook* notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, _T("notebook"));
    mainSizer->Add(notebook, 1, wxEXPAND);

    wxPanel* imagePanel = new wxPanel(notebook);
    notebook->AddPage(imagePanel, _T("image"));
    wxSizer* imageSizer = new wxBoxSizer(wxVERTICAL);
    imagePanel->SetSizer(imageSizer);

    wxPanel* LABPanel = new wxPanel(notebook);
    notebook->AddPage(LABPanel, _T("LAB"));
    wxSizer* LABSizer = new wxBoxSizer(wxVERTICAL);
    LABPanel->SetSizer(LABSizer);

    wxPanel* palettePanel = new wxPanel(notebook);
    notebook->AddPage(palettePanel, _T("palette"));
    wxSizer* paletteSizer = new wxBoxSizer(wxVERTICAL);
    palettePanel->SetSizer(paletteSizer);

    palette = NULL;
    palette2 = NULL;
    paletteLAB = NULL;

    const deViewManager& viewManager = project.getViewManager();
    int view = viewManager.getView();

    deLayerStack& layerStack = project.getLayerStack();

/*
    deBaseLayer* baseLayer = layerStack.getLayer(view);
    deLayer* layer = dynamic_cast<deLayer*>(baseLayer);
    */
    deBaseLayer* layer = layerStack.getLayer(view);

    if (!layer)
    {
        return;
    }

    const deImage& image = layer->getLayerImage();
    deColorSpace colorSpace = image.getColorSpace();

    deConversion3x3 f3x3 = getConversion3x3(colorSpace, deColorSpaceRGB);
    if (!f3x3)
    {
        return;
    }

    deConversion3x3 fLAB = getConversion3x3(colorSpace, deColorSpaceLAB);
    if (!fLAB)
    {
        return;
    }


    palette = new dePalette3(colorSpace);
    paletteLAB = new dePalette3(deColorSpaceLAB);

    deSize channelSize = image.getChannelSize();

    int cs = channelSize.getW();

    int w = channelSize.getW() / tileW;
    int h = channelSize.getH() / tileH;

    int n = w * h;

    int s = 2;

    wxSizer* sizer = new wxFlexGridSizer(w, s, s);
    imageSizer->Add(sizer);

    int i;

    int y = 0;
    int x = 0;

    for (i = 0; i < n; i++)
    {
        deColorPanel* colorPanel = new deColorPanel(imagePanel, wxSize(tileRW, tileRH), NULL, 0);
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
        const deValue* values1 = image.getValues(0);
        const deValue* values2 = image.getValues(1);
        const deValue* values3 = image.getValues(2);

        for (yy = y1; yy < y2; yy++)
        {
            int p = cs * yy + x1;
            for (xx = x1; xx < x2; xx++)
            {
                v1 = values1[p];
                v2 = values2[p];
                v3 = values3[p];
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

        deValue L;
        deValue A;
        deValue B;

        fLAB(sum1, sum2, sum3, L, A, B);
        paletteLAB->addColor(deColor3(L, A, B));

        colorPanel->setRGB(r, g, b);

        x++;
        if (x == w)
        {
            x = 0;
            y++;
        }
    }

    deValue minA;
    deValue maxA;
    paletteLAB->getMinMax(2, minA, maxA);
    deValue minB;
    deValue maxB;
    paletteLAB->getMinMax(3, minB, maxB);

    deValue deltaA = maxA - minA;
    deValue deltaB = maxB - minB;
    deValue delta = deltaA;
    if (deltaB > delta)
    {
        delta = deltaB;
    }

    deValue step = delta / LAB_TILES;

    int tiles = deltaA / step + 1;

    wxSizer* LABMapSizer = new wxFlexGridSizer(tiles, s, s);
    LABSizer->Add(LABMapSizer);

    deValue A;
    deValue B;
    deValue rB;

    for (rB = minB; rB < maxB; rB += step)
    {
        B = maxB + minB - rB;

        for (A = minA; A < maxA; A += step)
        {
            deColorPanel* colorPanel = new deColorPanel(LABPanel, wxSize(LAB_TILE_SIZE, LAB_TILE_SIZE), NULL, 0);
            LABMapSizer->Add(colorPanel);

            deValue L = 1.0;

            if (paletteLAB->find23(A, A + step, B, B + step, L))
            {
                deValue r;
                deValue g;
                deValue b;
                lab2rgb(L, A, B, r, g, b);
                colorPanel->setRGB(r, g, b);
            }                
        }
    }

    palette2 = new dePalette3(colorSpace);

    palette2->optimize(*palette, palSize, margin);

    int s2 = 5;

    wxSizer* sizerS = new wxStaticBoxSizer(wxHORIZONTAL, palettePanel,  _T("palette"));
    paletteSizer->Add(sizerS);
    
    wxSizer* palSizer = new wxFlexGridSizer(palSize / 2, s2, s2);
    sizerS->Add(palSizer);

    for (i = 0; i < palette2->getSize(); i++)
    {
        deColorPanel* colorPanel = new deColorPanel(palettePanel, wxSize(palW, palH), NULL, 0);
        palSizer->Add(colorPanel);

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
    if (paletteLAB)
    {
        delete paletteLAB;
    }        
}


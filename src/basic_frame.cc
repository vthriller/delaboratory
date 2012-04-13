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

#include "basic_frame.h"
#include "basic_layer.h"
#include <iostream>
#include "property_value_slider.h"
#include "layer_processor.h"

deBasicFrame::deBasicFrame(wxWindow *parent, deActionLayer& _layer, deLayerProcessor& _layerProcessor, deLayerFrameManager& _frameManager)
:deActionFrame(parent, _layer, _frameManager), layerProcessor(_layerProcessor)
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    deBasicLayer& basicLayer = dynamic_cast<deBasicLayer&>(_layer);

    int n = basicLayer.getNumberOfSettings();
    int s = basicLayer.getSeparator();

    int range = 400;

    wxSizer* sizerS = NULL;

    wxSizer* sizerS1 = new wxStaticBoxSizer(wxVERTICAL, this, _T("primary settings"));
    sizer->Add(sizerS1);

    sizerS = sizerS1;

    int i;
    for (i = 0; i < n; i++)
    {
        if (i == s)
        {
            wxSizer* sizerS2 = new wxStaticBoxSizer(wxVERTICAL, this, _T("secondary settings"));
            sizer->Add(sizerS2);
            sizerS = sizerS2;
        }

        dePropertyValue* p = basicLayer.getBasicProperty(i);

        if (p)
        {
            dePropertyValueSlider* s = new dePropertyValueSlider(this, range, *p, basicLayer, layerProcessor);
            basicSliders.push_back(s);
            sizerS->Add(s);
        }
    }

    wxSizer* sizerB = new wxStaticBoxSizer(wxHORIZONTAL, this, _T(""));
    sizer->Add(sizerB, 0);

    reset = new wxButton(this, wxID_ANY, _T("reset"), wxDefaultPosition, wxSize(100,25));
    sizerB->Add(reset, 0);

    Fit();

    Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deBasicFrame::click));
}

deBasicFrame::~deBasicFrame()
{
}

void deBasicFrame::click(wxCommandEvent &event)
{
    int id = event.GetId();
    deBasicLayer& basicLayer = dynamic_cast<deBasicLayer&>(layer);


    if (reset->GetId() == id)
    {
        basicLayer.reset();
    }      

    std::vector<dePropertyValueSlider*>::iterator i;
    for (i = basicSliders.begin(); i != basicSliders.end(); i++)
    {
        (*i)->setFromProperty();
    }

    int index = layer.getIndex();
    layerProcessor.markUpdateAllChannels(index);
}   

void applyWhiteBalanceLAB(deLayer& layer, deValue x, deValue y)
{
    const deImage& image = layer.getImage();
    deBasicLayer& basicLayer = dynamic_cast<deBasicLayer&>(layer);
    deSize size = basicLayer.getChannelSize();
    int p = (y * size.getH() )  * size.getW() + (x * size.getW());

    deValue A;
    bool res1 = image.getPixel(1, p, A);
    if (res1)
    {
        dePropertyValue* ATint = basicLayer.getBasicProperty("A tint");
        if (ATint)
        {
            deValue old = ATint->get();
            A = 0.5 - A + old;
            ATint->set(A);
        }
    }

    deValue B;
    bool res2 = image.getPixel(2, p, B);
    if (res2)
    {
        dePropertyValue* BTint = basicLayer.getBasicProperty("B tint");
        if (BTint)
        {
            deValue old = BTint->get();
            B = 0.5 - B + old;
            BTint->set(B);
        }
    }
}

void applyWhiteBalanceRGB(deLayer& layer, deValue x, deValue y)
{
    const deImage& image = layer.getImage();
    deBasicLayer& basicLayer = dynamic_cast<deBasicLayer&>(layer);
    deSize size = basicLayer.getChannelSize();
    int p = (y * size.getH() )  * size.getW() + (x * size.getW());

    deValue r;
    bool res1 = image.getPixel(0, p, r);
    deValue g;
    bool res2 = image.getPixel(1, p, g);
    deValue b;
    bool res3 = image.getPixel(2, p, b);

    if ((!res1) || (!res2) || (!res3))
    {
        return;
    }

    deValue a = (r + g + b) / 3.0;

    dePropertyValue* rTint = basicLayer.getBasicProperty("Red tint");
    if (rTint)
    {
        deValue old = rTint->get();
        r = old - (r - a);
        rTint->set(r);
    }
    dePropertyValue* gTint = basicLayer.getBasicProperty("Green tint");
    if (gTint)
    {
        deValue old = gTint->get();
        g = old - (g - a);
        gTint->set(g);
    }
    dePropertyValue* bTint = basicLayer.getBasicProperty("Blue tint");
    if (bTint)
    {
        deValue old = bTint->get();
        b = old - (b - a);
        bTint->set(b);
    }

}

void applyWhiteBalanceCMYK(deLayer& layer, deValue x, deValue y)
{
    const deImage& image = layer.getImage();
    deBasicLayer& basicLayer = dynamic_cast<deBasicLayer&>(layer);
    deSize size = basicLayer.getChannelSize();
    int p = (y * size.getH() )  * size.getW() + (x * size.getW());

    deValue r;
    bool res1 = image.getPixel(0, p, r);
    deValue g;
    bool res2 = image.getPixel(1, p, g);
    deValue b;
    bool res3 = image.getPixel(2, p, b);

    if ((!res1) || (!res2) || (!res3))
    {
        return;
    }

    deValue a = (r + g + b) / 3.0;

    dePropertyValue* rTint = basicLayer.getBasicProperty("Cyan tint");
    if (rTint)
    {
        deValue old = rTint->get();
        r = old - (r - a);
        rTint->set(r);
    }
    dePropertyValue* gTint = basicLayer.getBasicProperty("Magenta tint");
    if (gTint)
    {
        deValue old = gTint->get();
        g = old - (g - a);
        gTint->set(g);
    }
    dePropertyValue* bTint = basicLayer.getBasicProperty("Yellow tint");
    if (bTint)
    {
        deValue old = bTint->get();
        b = old - (b - a);
        bTint->set(b);
    }

}

bool deBasicFrame::onImageClick(deValue x, deValue y)
{
    if ((x < 0) || (y < 0) || (x >= 1) || (y >= 1))
    {
        return false;
    }

    if (layer.getColorSpace() == deColorSpaceLAB)
    {
        applyWhiteBalanceLAB(layer, x, y);
    }

    if (layer.getColorSpace() == deColorSpaceCMYK)
    {
        applyWhiteBalanceCMYK(layer, x, y);
    }

    if ((layer.getColorSpace() == deColorSpaceRGB) || (layer.getColorSpace() == deColorSpaceProPhoto))
    {
        applyWhiteBalanceRGB(layer, x, y);
    }

    std::vector<dePropertyValueSlider*>::iterator i;
    for (i = basicSliders.begin(); i != basicSliders.end(); i++)
    {
        (*i)->setFromProperty();
    }

    int index = layer.getIndex();

    layerProcessor.markUpdateAllChannels(index);

    return true;
}

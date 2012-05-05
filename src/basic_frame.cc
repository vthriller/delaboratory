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
#include "channel_manager.h"

deBasicFrame::deBasicFrame(wxWindow *parent, deLayerOld& _layer, deLayerProcessor& _layerProcessor, deLayerFrameManager& _frameManager, int _layerIndex, deChannelManager& _channelManager)
:deActionFrame(parent, _layer, _frameManager, _layerIndex), layerProcessor(_layerProcessor), channelManager(_channelManager)
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
            dePropertyValueSlider* s = new dePropertyValueSlider(this, range, *p, layerProcessor, layerIndex);
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

    layerProcessor.markUpdateAllChannels(layerIndex);
}   

void applyWhiteBalanceLAB(deBaseLayer& layer, deValue x, deValue y, deSize& size)
{
    const deImage& image = layer.getLayerImage();
    deBasicLayer& basicLayer = dynamic_cast<deBasicLayer&>(layer);
    int p = (y * size.getH() )  * size.getW() + (x * size.getW());

    const deValue* valuesA = image.startRead(1);
    const deValue* valuesB = image.startRead(2);
    if ((!valuesA) || (!valuesB))
    {
        image.finishRead(1);
        image.finishRead(2);
        return;
    }
    deValue A = valuesA[p];
    deValue B = valuesB[p];

    dePropertyValue* ATint = basicLayer.getBasicProperty("A tint");
    if (ATint)
    {
        deValue old = ATint->get();
        A = 0.5 - A + old;
        ATint->set(A);
    }

    dePropertyValue* BTint = basicLayer.getBasicProperty("B tint");
    if (BTint)
    {
        deValue old = BTint->get();
        B = 0.5 - B + old;
        BTint->set(B);
    }

    image.finishRead(1);
    image.finishRead(2);
}

void applyWhiteBalanceRGB(deBaseLayer& layer, deValue x, deValue y, deSize& size)
{
/*
    const deImage& image = layer.getLayerImage();
    deBasicLayer& basicLayer = dynamic_cast<deBasicLayer&>(layer);
    int p = (y * size.getH() )  * size.getW() + (x * size.getW());

    const deValue* valuesR = image.getValues(0);
    if (!valuesR)
    {
        return;
    }
    deValue r = valuesR[p];
    const deValue* valuesG = image.getValues(1);
    if (!valuesG)
    {
        return;
    }
    deValue g = valuesG[p];
    const deValue* valuesB = image.getValues(2);
    if (!valuesB)
    {
        return;
    }
    deValue b = valuesB[p];

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
    */
}

void applyWhiteBalanceCMYK(deBaseLayer& layer, deValue x, deValue y, deSize& size)
{
/*
    const deImage& image = layer.getLayerImage();
    deBasicLayer& basicLayer = dynamic_cast<deBasicLayer&>(layer);
    int p = (y * size.getH() )  * size.getW() + (x * size.getW());

    const deValue* valuesR = image.getValues(0);
    if (!valuesR)
    {
        return;
    }
    deValue r = valuesR[p];
    const deValue* valuesG = image.getValues(1);
    if (!valuesG)
    {
        return;
    }
    deValue g = valuesG[p];
    const deValue* valuesB = image.getValues(2);
    if (!valuesB)
    {
        return;
    }
    deValue b = valuesB[p];

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
*/
}

bool deBasicFrame::onImageClick(deValue x, deValue y)
{
    if ((x < 0) || (y < 0) || (x >= 1) || (y >= 1))
    {
        return false;
    }

    deSize size = channelManager.getChannelSizeFromChannelManager();

    if (layer.getColorSpace() == deColorSpaceLAB)
    {
        applyWhiteBalanceLAB(layer, x, y, size);
    }

    if (layer.getColorSpace() == deColorSpaceCMYK)
    {
        applyWhiteBalanceCMYK(layer, x, y, size);
    }

    if ((layer.getColorSpace() == deColorSpaceRGB) || (layer.getColorSpace() == deColorSpaceProPhoto))
    {
        applyWhiteBalanceRGB(layer, x, y, size);
    }

    std::vector<dePropertyValueSlider*>::iterator i;
    for (i = basicSliders.begin(); i != basicSliders.end(); i++)
    {
        (*i)->setFromProperty();
    }

    layerProcessor.markUpdateAllChannels(layerIndex);

    return true;
}

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

#include "hue_saturation_frame.h"
#include "conversion_bw2hue_layer.h"
#include <iostream>
#include "property_value_slider.h"
#include "layer_processor.h"
#include "layer_frame_manager.h"

deHueSaturationFrame::deHueSaturationFrame(wxWindow *parent, deConversionBW2HueLayer& _layer, deLayerProcessor& _layerProcessor, deLayerFrameManager& _frameManager)
:deLayerFrame(parent, _layer, "BW -> hue/saturation", _frameManager), layerProcessor(_layerProcessor), layer(_layer)
{
    frameManager.addActionFrame(this);

    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    deConversionBW2HueLayer& hsLayer = dynamic_cast<deConversionBW2HueLayer&>(_layer);

    int range = 400;

    hue = new dePropertyValueSlider(this, range, hsLayer.getPropertyHue(), hsLayer, layerProcessor);
    sizer->Add(hue);

    saturation = new dePropertyValueSlider(this, range, hsLayer.getPropertySaturation(), hsLayer, layerProcessor);
    sizer->Add(saturation);


    wxSizer* sizerB = new wxStaticBoxSizer(wxHORIZONTAL, this, _T(""));
    sizer->Add(sizerB, 0);

    reset = new wxButton(this, wxID_ANY, _T("reset"), wxDefaultPosition, wxSize(100,25));
    sizerB->Add(reset, 0);

    preset1 = new wxButton(this, wxID_ANY, _T("sepia"), wxDefaultPosition, wxSize(100,25));
    sizerB->Add(preset1, 0);

    preset2 = new wxButton(this, wxID_ANY, _T("platine"), wxDefaultPosition, wxSize(100,25));
    sizerB->Add(preset2, 0);

    preset3 = new wxButton(this, wxID_ANY, _T("silver"), wxDefaultPosition, wxSize(100,25));
    sizerB->Add(preset3, 0);

    Fit();

    Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deHueSaturationFrame::click));
}

deHueSaturationFrame::~deHueSaturationFrame()
{
    frameManager.removeActionFrame(this);
}

void deHueSaturationFrame::click(wxCommandEvent &event)
{
    int id = event.GetId();
    deConversionBW2HueLayer& hsLayer = dynamic_cast<deConversionBW2HueLayer&>(layer);

    if (reset->GetId() == id)
    {
        hsLayer.reset();
    }      

    if (preset1->GetId() == id)
    {
        hsLayer.preset(0.075);
    }      

    if (preset2->GetId() == id)
    {
        hsLayer.preset(0.16);
    }      

    if (preset3->GetId() == id)
    {
        hsLayer.preset(0.6);
    }      

    hue->setFromProperty();
    saturation->setFromProperty();

    int index = layer.getIndex();
    layerProcessor.markUpdateAllChannels(index);
}   

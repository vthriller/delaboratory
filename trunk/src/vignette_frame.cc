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

#include "vignette_frame.h"
#include "vignette_layer.h"
#include <iostream>
#include "property_value_slider.h"

deVignetteFrame::deVignetteFrame(wxWindow *parent, deActionLayer& _layer, deLayerProcessor& _layerProcessor, deLayerFrameManager& _frameManager)
:deActionFrame(parent, _layer, _frameManager), layerProcessor(_layerProcessor)
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    deVignetteLayer& vignetteLayer = dynamic_cast<deVignetteLayer&>(_layer);

    int range = 400;

    light = new dePropertyValueSlider(this, range, vignetteLayer.getPropertyLight(), vignetteLayer, layerProcessor);
    sizer->Add(light);

    centerX = new dePropertyValueSlider(this, range, vignetteLayer.getPropertyCenterX(), vignetteLayer, layerProcessor);
    sizer->Add(centerX);

    centerY = new dePropertyValueSlider(this, range, vignetteLayer.getPropertyCenterY(), vignetteLayer, layerProcessor);
    sizer->Add(centerY);

    radiusX = new dePropertyValueSlider(this, range, vignetteLayer.getPropertyRadiusX(), vignetteLayer, layerProcessor);
    sizer->Add(radiusX);

    radiusY = new dePropertyValueSlider(this, range, vignetteLayer.getPropertyRadiusY(), vignetteLayer, layerProcessor);
    sizer->Add(radiusY);


    wxSizer* sizerB = new wxStaticBoxSizer(wxHORIZONTAL, this, _T(""));
    sizer->Add(sizerB, 0);

    reset = new wxButton(this, wxID_ANY, _T("reset"), wxDefaultPosition, wxSize(100,25));
    sizerB->Add(reset, 0);

    Fit();

    Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deVignetteFrame::click));
}

deVignetteFrame::~deVignetteFrame()
{
}

void deVignetteFrame::click(wxCommandEvent &event)
{
    int id = event.GetId();
    deVignetteLayer& vignetteLayer = dynamic_cast<deVignetteLayer&>(layer);

    if (reset->GetId() == id)
    {
        vignetteLayer.reset();
    }      

    light->setFromProperty();
    radiusX->setFromProperty();
    radiusY->setFromProperty();
    centerX->setFromProperty();
    centerY->setFromProperty();
}   

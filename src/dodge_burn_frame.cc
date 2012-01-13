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

#include "dodge_burn_frame.h"
#include "dodge_burn_layer.h"
#include <iostream>
#include "property_value_slider.h"

deDodgeBurnFrame::deDodgeBurnFrame(wxWindow *parent, deActionLayer& _layer, deLayerProcessor& _layerProcessor, deLayerFrameManager& _frameManager)
:deActionFrame(parent, _layer, _frameManager), layerProcessor(_layerProcessor)
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    deDodgeBurnLayer& dodgeBurnLayer = dynamic_cast<deDodgeBurnLayer&>(_layer);

    int range = 400;

    radius = new dePropertyValueSlider(this, range, dodgeBurnLayer.getPropertyRadius(), dodgeBurnLayer, layerProcessor);
    sizer->Add(radius);

    dodgeAmount = new dePropertyValueSlider(this, range, dodgeBurnLayer.getPropertyDodgeAmount(), dodgeBurnLayer, layerProcessor);
    sizer->Add(dodgeAmount);

    dodgeMin = new dePropertyValueSlider(this, range, dodgeBurnLayer.getPropertyDodgeMin(), dodgeBurnLayer, layerProcessor);
    sizer->Add(dodgeMin);

    dodgeMax = new dePropertyValueSlider(this, range, dodgeBurnLayer.getPropertyDodgeMax(), dodgeBurnLayer, layerProcessor);
    sizer->Add(dodgeMax);

    burnAmount = new dePropertyValueSlider(this, range, dodgeBurnLayer.getPropertyBurnAmount(), dodgeBurnLayer, layerProcessor);
    sizer->Add(burnAmount);

    burnMin = new dePropertyValueSlider(this, range, dodgeBurnLayer.getPropertyBurnMin(), dodgeBurnLayer, layerProcessor);
    sizer->Add(burnMin);

    burnMax = new dePropertyValueSlider(this, range, dodgeBurnLayer.getPropertyBurnMax(), dodgeBurnLayer, layerProcessor);
    sizer->Add(burnMax);

    wxSizer* sizerB = new wxStaticBoxSizer(wxHORIZONTAL, this, _T(""));
    sizer->Add(sizerB, 0);

    reset = new wxButton(this, wxID_ANY, _T("reset"), wxDefaultPosition, wxSize(100,25));
    sizerB->Add(reset, 0);

    Fit();

    Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deDodgeBurnFrame::click));
}

deDodgeBurnFrame::~deDodgeBurnFrame()
{
}

void deDodgeBurnFrame::click(wxCommandEvent &event)
{
    int id = event.GetId();
    deDodgeBurnLayer& dodgeBurnLayer = dynamic_cast<deDodgeBurnLayer&>(layer);

    if (reset->GetId() == id)
    {
        dodgeBurnLayer.reset();
    }      

    radius->setFromProperty();
    dodgeAmount->setFromProperty();
    dodgeMin->setFromProperty();
    dodgeMax->setFromProperty();
    burnAmount->setFromProperty();
    burnMin->setFromProperty();
    burnMax->setFromProperty();
}   

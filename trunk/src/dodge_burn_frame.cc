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

deDodgeBurnFrame::deDodgeBurnFrame(wxWindow *parent, deActionLayer& _layer)
:deActionFrame(parent, _layer)
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    deDodgeBurnLayer& dodgeBurnLayer = dynamic_cast<deDodgeBurnLayer&>(_layer);

    int range = 400;

    radius = new dePropertyValueSlider(this, range, dodgeBurnLayer.getPropertyRadius(), dodgeBurnLayer);
    sizer->Add(radius);

    dodgeAmount = new dePropertyValueSlider(this, range, dodgeBurnLayer.getPropertyDodgeAmount(), dodgeBurnLayer);
    sizer->Add(dodgeAmount);

    dodgeMin = new dePropertyValueSlider(this, range, dodgeBurnLayer.getPropertyDodgeMin(), dodgeBurnLayer);
    sizer->Add(dodgeMin);

    dodgeMax = new dePropertyValueSlider(this, range, dodgeBurnLayer.getPropertyDodgeMax(), dodgeBurnLayer);
    sizer->Add(dodgeMax);

    burnAmount = new dePropertyValueSlider(this, range, dodgeBurnLayer.getPropertyBurnAmount(), dodgeBurnLayer);
    sizer->Add(burnAmount);

    burnMin = new dePropertyValueSlider(this, range, dodgeBurnLayer.getPropertyBurnMin(), dodgeBurnLayer);
    sizer->Add(burnMin);

    burnMax = new dePropertyValueSlider(this, range, dodgeBurnLayer.getPropertyBurnMax(), dodgeBurnLayer);
    sizer->Add(burnMax);

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

}   

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

#include "usm_frame.h"
#include "usm_layer.h"
#include <iostream>
#include "property_value_slider.h"

deUSMFrame::deUSMFrame(wxWindow *parent, deActionLayer& _layer)
:deActionFrame(parent, _layer)
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    deUSMLayer& usmLayer = dynamic_cast<deUSMLayer&>(_layer);

    int range = 400;

    radius = new dePropertyValueSlider(this, range, usmLayer.getPropertyRadius(), usmLayer);
    sizer->Add(radius);

    amount = new dePropertyValueSlider(this, range, usmLayer.getPropertyAmount(), usmLayer);
    sizer->Add(amount);

    threshold = new dePropertyValueSlider(this, range, usmLayer.getPropertyThreshold(), usmLayer);
    sizer->Add(threshold);

    wxSizer* sizerB = new wxStaticBoxSizer(wxHORIZONTAL, this, _T(""));
    sizer->Add(sizerB, 0);

    reset = new wxButton(this, wxID_ANY, _T("reset"), wxDefaultPosition, wxSize(100,25));
    sizerB->Add(reset, 0);

    sharp = new wxButton(this, wxID_ANY, _T("sharp"), wxDefaultPosition, wxSize(100,25));
    sizerB->Add(sharp, 0);

    hiraloam1 = new wxButton(this, wxID_ANY, _T("hiraloam 1"), wxDefaultPosition, wxSize(100,25));
    sizerB->Add(hiraloam1, 0);

    hiraloam2 = new wxButton(this, wxID_ANY, _T("hiraloam 2"), wxDefaultPosition, wxSize(100,25));
    sizerB->Add(hiraloam2, 0);

    Fit();

    Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deUSMFrame::click));
}

deUSMFrame::~deUSMFrame()
{
}

void deUSMFrame::click(wxCommandEvent &event)
{
    int id = event.GetId();
    deUSMLayer& usmLayer = dynamic_cast<deUSMLayer&>(layer);

    if (reset->GetId() == id)
    {
        usmLayer.reset();
    }      

    if (sharp->GetId() == id)
    {
        usmLayer.sharp();
    }      

    if (hiraloam1->GetId() == id)
    {
        usmLayer.hiraloam1();
    }      

    if (hiraloam2->GetId() == id)
    {
        usmLayer.hiraloam2();
    }      

    radius->setFromProperty();
    amount->setFromProperty();
    threshold->setFromProperty();
}   

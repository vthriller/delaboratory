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

    int range = 400;

    int i;
    for (i = 0; i < n; i++)
    {
        dePropertyValue* p = basicLayer.getBasicProperty(i);

        if (p)
        {
            dePropertyValueSlider* s = new dePropertyValueSlider(this, range, *p, basicLayer, layerProcessor);
            basicSliders.push_back(s);
            sizer->Add(s);
        }

    }

    wxSizer* sizerB = new wxStaticBoxSizer(wxHORIZONTAL, this, _T(""));
    sizer->Add(sizerB, 0);

    reset = new wxButton(this, wxID_ANY, _T("reset"), wxDefaultPosition, wxSize(100,25));
    sizerB->Add(reset, 0);


/*
    int range = 400;

    radius = new dePropertyValueSlider(this, range, basicLayer.getPropertyRadius(), basicLayer, layerProcessor);
    sizer->Add(radius);

    amount = new dePropertyValueSlider(this, range, basicLayer.getPropertyAmount(), basicLayer, layerProcessor);
    sizer->Add(amount);

    threshold = new dePropertyValueSlider(this, range, basicLayer.getPropertyThreshold(), basicLayer, layerProcessor);
    sizer->Add(threshold);



    sharp = new wxButton(this, wxID_ANY, _T("sharp"), wxDefaultPosition, wxSize(100,25));
    sizerB->Add(sharp, 0);

    hiraloam1 = new wxButton(this, wxID_ANY, _T("hiraloam 1"), wxDefaultPosition, wxSize(100,25));
    sizerB->Add(hiraloam1, 0);

    hiraloam2 = new wxButton(this, wxID_ANY, _T("hiraloam 2"), wxDefaultPosition, wxSize(100,25));
    sizerB->Add(hiraloam2, 0);

    */

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

    /*

    if (sharp->GetId() == id)
    {
        basicLayer.sharp();
    }      

    if (hiraloam1->GetId() == id)
    {
        basicLayer.hiraloam1();
    }      

    if (hiraloam2->GetId() == id)
    {
        basicLayer.hiraloam2();
    }      

    radius->setFromProperty();
    amount->setFromProperty();
    threshold->setFromProperty();

    */

    int index = layer.getIndex();
    layerProcessor.markUpdateAllChannels(index);
}   

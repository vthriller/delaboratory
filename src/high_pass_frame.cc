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

#include "high_pass_frame.h"
#include "high_pass_layer.h"
#include <iostream>
#include "property_value_slider.h"
#include "layer_processor.h"

deHighPassFrame::deHighPassFrame(wxWindow *parent, deActionLayer& _layer, deLayerProcessor& _layerProcessor, deLayerFrameManager& _frameManager, int _layerIndex)
:deActionFrame(parent, _layer, _frameManager, _layerIndex), layerProcessor(_layerProcessor)
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    deHighPassLayer& highPassLayer = dynamic_cast<deHighPassLayer&>(_layer);

    int range = 400;

    int n = highPassLayer.getNumberOfProperties();

    int i;

    for (i = 0; i < n; i++)
    {
        dePropertyValue* p = highPassLayer.getPropertyValue(i);
        if (p)
        {
            dePropertyValueSlider* s = new dePropertyValueSlider(this, range, *p, highPassLayer, layerProcessor, layerIndex);
            valueSliders.push_back(s);
            sizer->Add(s);
        }            
    }

/*
    radius = new dePropertyValueSlider(this, range, highPassLayer.getPropertyRadius(), highPassLayer, layerProcessor);
    sizer->Add(radius);
    */

    wxSizer* sizerB = new wxStaticBoxSizer(wxHORIZONTAL, this, _T(""));
    sizer->Add(sizerB, 0);

    reset = new wxButton(this, wxID_ANY, _T("reset"), wxDefaultPosition, wxSize(100,25));
    sizerB->Add(reset, 0);

    Fit();

    Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deHighPassFrame::click));
}

deHighPassFrame::~deHighPassFrame()
{
}

void deHighPassFrame::click(wxCommandEvent &event)
{
    int id = event.GetId();
    deHighPassLayer& highPassLayer = dynamic_cast<deHighPassLayer&>(layer);

    if (reset->GetId() == id)
    {
        highPassLayer.reset();
    }      

    //radius->setFromProperty();
    std::vector<dePropertyValueSlider*>::iterator j;
    for (j = valueSliders.begin(); j != valueSliders.end(); j++)
    {
        (*j)->setFromProperty();
    }

    //int index = layer.getIndex();
    layerProcessor.markUpdateAllChannels(layerIndex);
}   

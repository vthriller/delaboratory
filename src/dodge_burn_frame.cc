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
#include "property_boolean_ui.h"
#include "layer_processor.h"

deDodgeBurnFrame::deDodgeBurnFrame(wxWindow *parent, deActionLayer& _layer, deLayerProcessor& _layerProcessor, deLayerFrameManager& _frameManager)
:deActionFrame(parent, _layer, _frameManager), layerProcessor(_layerProcessor)
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    deDodgeBurnLayer& dodgeBurnLayer = dynamic_cast<deDodgeBurnLayer&>(_layer);

    int range = 400;

    int n = dodgeBurnLayer.getNumberOfValueProperties();

    int i;

    for (i = 0; i < n; i++)
    {
        dePropertyValue* p = dodgeBurnLayer.getPropertyValue(i);
        if (p)
        {
            dePropertyValueSlider* s = new dePropertyValueSlider(this, range, *p, dodgeBurnLayer, layerProcessor);
            valueSliders.push_back(s);
            sizer->Add(s);
        }            
    }

    alternate = new dePropertyBooleanUI(this, dodgeBurnLayer.getPropertyAlternate(), dodgeBurnLayer, layerProcessor);
    sizer->Add(alternate);

    wxSizer* sizerB = new wxFlexGridSizer(5);
    sizer->Add(sizerB, 0);

    std::vector<std::string> presets;
    dodgeBurnLayer.getPresets(presets);

    std::vector<std::string>::iterator j;
    for (j = presets.begin(); j != presets.end(); j++)
    {
        wxButton* b = new wxButton(this, wxID_ANY, wxString::FromAscii(j->c_str()), wxDefaultPosition, wxSize(100,25));
        sizerB->Add(b, 0);
        buttons[*j] = b;
    }

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

    std::map<std::string, wxButton*>::iterator i;

    for (i = buttons.begin(); i != buttons.end(); i++)
    {
        if (i->second->GetId() == id)
        {
            dodgeBurnLayer.applyPreset(i->first);
        }
    }

    std::vector<dePropertyValueSlider*>::iterator j;
    for (j = valueSliders.begin(); j != valueSliders.end(); j++)
    {
        (*j)->setFromProperty();
    }

    int index = layer.getIndex();
    layerProcessor.markUpdateAllChannels(index);
}   

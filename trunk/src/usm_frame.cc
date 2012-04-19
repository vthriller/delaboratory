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
#include "layer_processor.h"

deUSMFrame::deUSMFrame(wxWindow *parent, deActionLayer& _layer, deLayerProcessor& _layerProcessor, deLayerFrameManager& _frameManager, int _layerIndex)
:deActionFrame(parent, _layer, _frameManager, _layerIndex), layerProcessor(_layerProcessor)
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    deUSMLayer& usmLayer = dynamic_cast<deUSMLayer&>(_layer);

    int range = 400;

    int n = usmLayer.getNumberOfProperties();

    int i;

    for (i = 0; i < n; i++)
    {
        dePropertyValue* p = usmLayer.getPropertyValue(i);
        if (p)
        {
            dePropertyValueSlider* s = new dePropertyValueSlider(this, range, *p, layerProcessor, layerIndex);
            valueSliders.push_back(s);
            sizer->Add(s);
        }            
    }

    wxSizer* sizerB = new wxStaticBoxSizer(wxHORIZONTAL, this, _T(""));
    sizer->Add(sizerB, 0);

    std::vector<std::string> presets;
    usmLayer.getPresets(presets);

    std::vector<std::string>::iterator j;
    for (j = presets.begin(); j != presets.end(); j++)
    {
        wxButton* b = new wxButton(this, wxID_ANY, wxString::FromAscii(j->c_str()), wxDefaultPosition, wxSize(100,25));
        sizerB->Add(b, 0);
        buttons[*j] = b;
    }

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

    std::map<std::string, wxButton*>::iterator i;

    for (i = buttons.begin(); i != buttons.end(); i++)
    {
        if (i->second->GetId() == id)
        {
            usmLayer.applyPreset(i->first);
        }
    }

    std::vector<dePropertyValueSlider*>::iterator j;
    for (j = valueSliders.begin(); j != valueSliders.end(); j++)
    {
        (*j)->setFromProperty();
    }

    //int index = layer.getIndex();
    layerProcessor.markUpdateAllChannels(layerIndex);
}   

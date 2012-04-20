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
#include "layer_processor.h"
#include "opacity_slider.h"

deVignetteFrame::deVignetteFrame(wxWindow *parent, deLayer& _layer, deLayerProcessor& _layerProcessor, deLayerFrameManager& _frameManager, int _layerIndex)
:deActionFrame(parent, _layer, _frameManager, _layerIndex), layerProcessor(_layerProcessor)
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    deVignetteLayer& vignetteLayer = dynamic_cast<deVignetteLayer&>(_layer);

    int range = 400;

    int n = vignetteLayer.getNumberOfProperties();

    int i;

    for (i = 0; i < n; i++)
    {
        dePropertyValue* p = vignetteLayer.getPropertyValue(i);
        if (p)
        {
            dePropertyValueSlider* s = new dePropertyValueSlider(this, range, *p, layerProcessor, layerIndex);
            valueSliders.push_back(s);
            sizer->Add(s);
        }            
    }

    deLayerWithBlending& lwb  = dynamic_cast<deLayerWithBlending&>(_layer);
    deSlider* alphaSlider = new deOpacitySlider(this, range, lwb, layerProcessor, layerIndex);
    sizer->Add(alphaSlider, 0);

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

    std::vector<dePropertyValueSlider*>::iterator j;
    for (j = valueSliders.begin(); j != valueSliders.end(); j++)
    {
        (*j)->setFromProperty();
    }

    layerProcessor.markUpdateAllChannels(layerIndex);
}   

bool deVignetteFrame::onImageClick(deValue x, deValue y)
{
    if ((x < 0) || (y < 0) || (x >= 1) || (y >= 1))
    {
        return false;
    }
    deVignetteLayer& vignetteLayer = dynamic_cast<deVignetteLayer&>(layer);
    bool result = vignetteLayer.setCenter(x, y);

    std::vector<dePropertyValueSlider*>::iterator j;
    for (j = valueSliders.begin(); j != valueSliders.end(); j++)
    {
        (*j)->setFromProperty();
    }

    layerProcessor.markUpdateAllChannels(layerIndex);

    return result;
}

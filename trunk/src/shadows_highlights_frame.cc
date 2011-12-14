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

#include "shadows_highlights_frame.h"
#include "shadows_highlights_layer.h"
#include <iostream>
#include "property_value_slider.h"

deShadowsHighlightsFrame::deShadowsHighlightsFrame(wxWindow *parent, deActionLayer& _layer, deLayerProcessor& _layerProcessor)
:deActionFrame(parent, _layer), layerProcessor(_layerProcessor)
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    deShadowsHighlightsLayer& shLayer = dynamic_cast<deShadowsHighlightsLayer&>(_layer);

    int range = 400;

    radius = new dePropertyValueSlider(this, range, shLayer.getPropertyRadius(), shLayer, layerProcessor);
    sizer->Add(radius);

    shAmount = new dePropertyValueSlider(this, range, shLayer.getPropertyShadowsHighlightsAmount(), shLayer, layerProcessor);
    sizer->Add(shAmount);

    darkenAmount = new dePropertyValueSlider(this, range, shLayer.getPropertyDarkenAmount(), shLayer, layerProcessor);
    sizer->Add(darkenAmount);

    lightenAmount = new dePropertyValueSlider(this, range, shLayer.getPropertyLightenAmount(), shLayer, layerProcessor);
    sizer->Add(lightenAmount);

    Fit();

    Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deShadowsHighlightsFrame::click));
}

deShadowsHighlightsFrame::~deShadowsHighlightsFrame()
{
}

void deShadowsHighlightsFrame::click(wxCommandEvent &event)
{
/*
    int id = event.GetId();
    deShadowsHighlightsLayer& shLayer = dynamic_cast<deShadowsHighlightsLayer&>(layer);
*/
}   

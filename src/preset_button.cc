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

#include "preset_button.h"
#include "layer_processor.h"
#include "layer_frame.h"
#include "base_layer_with_properties.h"

dePresetButton::dePresetButton(deWindow& window, deBaseLayerWithProperties& _layer, const std::string& name, deLayerProcessor& _layerProcessor, int _layerIndex, deLayerFrame& _frame)
:deButton(window, name), preset(name), layerProcessor(_layerProcessor), layer(_layer), layerIndex(_layerIndex), frame(_frame)
{
}

dePresetButton::~dePresetButton()
{
}

void dePresetButton::onClick()
{
    layer.applyPreset(preset);        
    frame.setUIFromLayer();
    layerProcessor.markUpdateAllChannels(layerIndex);
}

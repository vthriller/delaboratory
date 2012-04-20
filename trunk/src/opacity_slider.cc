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

#include "opacity_slider.h"
#include "layer_with_blending.h"
#include "layer_processor.h"

deOpacitySlider::deOpacitySlider(wxWindow *parent, int range, deLayerWithBlending& _layer, deLayerProcessor& _layerProcessor, int _layerIndex)
:deSlider(parent, "opacity", range, 0.0, 1.0, 1.0), layer(_layer),
layerProcessor(_layerProcessor), layerIndex(_layerIndex)
{
    setValue(layer.getOpacity());
}

deOpacitySlider::~deOpacitySlider()
{
}

void deOpacitySlider::onValueChange(deValue value, bool finished)
{
    layer.setOpacity(value);

    layerProcessor.markUpdateBlendAllChannels(layerIndex);
}


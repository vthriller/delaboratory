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

#include "property_value_slider.h"
#include "property_value.h"
#include "layer.h"
#include "layer_processor.h"

dePropertyValueSlider::dePropertyValueSlider(wxWindow *parent, int _sliderRange, dePropertyValue& _property, deLayer& _layer, deLayerProcessor& _layerProcessor)
:deSlider(parent, _property.getLabel(), _sliderRange, _property.getMin(), _property.getMax(), _property.getDefault(), _layerProcessor),
property(_property),
layer(_layer),
layerProcessor(_layerProcessor)
{
    setFromProperty();
}

dePropertyValueSlider::~dePropertyValueSlider()
{
}

void dePropertyValueSlider::onValueChange(deValue value, bool finished)
{
    if ((finished) || (layerProcessor.isRealtime()))
    {
        property.set(value);

        int index = layer.getIndex();
        layerProcessor.markUpdateAllChannels(index);
    }                
}

void dePropertyValueSlider::setFromProperty()
{
    setValue(property.get());
}


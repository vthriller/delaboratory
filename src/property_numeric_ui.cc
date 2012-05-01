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

#include "property_numeric_ui.h"
#include "property_numeric.h"
#include "layer_processor.h"

dePropertyNumericUI::dePropertyNumericUI(deWindow& window, dePropertyNumeric& _property, deLayerProcessor& _layerProcessor, int _layerIndex, int _width, int widthn, int widthl)
:deSlider(window, _property.getName(), _property.getMin(), _property.getMax(), _width, widthn, widthl), property(_property), layerProcessor(_layerProcessor), layerIndex(_layerIndex)
{
    setFromProperty();
}

dePropertyNumericUI::~dePropertyNumericUI()
{
}

void dePropertyNumericUI::onValueChange(deValue value, bool finished)
{
    property.set(value);

    if (property.updateBlendOnly())
    {
        layerProcessor.markUpdateBlendAllChannels(layerIndex);
    }
    else
    {
        layerProcessor.markUpdateAllChannels(layerIndex);
    }
}

void dePropertyNumericUI::setFromProperty()
{
    setValue(property.get());
}


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

#include "property_choice_ui.h"
#include "property_choice.h"
#include "base_layer.h"
#include "layer_processor.h"

dePropertyChoiceUI::dePropertyChoiceUI(deWindow& window, dePropertyChoice& _property, deLayerProcessor& _layerProcessor, int _layerIndex)
:deChoice(window, _property.getName(), _property.getChoices()),
property(_property),
layerProcessor(_layerProcessor),
layerIndex(_layerIndex)
{
    setFromProperty();
}

dePropertyChoiceUI::~dePropertyChoiceUI()
{
}

void dePropertyChoiceUI::onChoose(int index)
{
    property.setIndex(index);

    if (property.affectsSize())
    {
        layerProcessor.forceUpdateSize();
    }

    if (property.updateBlendOnly())
    {
        layerProcessor.markUpdateBlendAllChannels(layerIndex);
    }
    else
    {
        layerProcessor.markUpdateAllChannels(layerIndex);
    }
}

void dePropertyChoiceUI::setFromProperty()
{
    int index = property.getIndex();
    set(index);
}

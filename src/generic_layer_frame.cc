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

#include "generic_layer_frame.h"
#include "base_layer.h"
#include "base_layer_with_source.h"
#include "property_numeric_ui.h"
#include "property_levels_ui.h"
#include "property_numeric.h"
#include "property_levels.h"
#include "preset_button.h"

deGenericLayerFrame::deGenericLayerFrame(deWindow& parent, const std::string& name, deBaseLayer& _layer, deLayerProcessor& _layerProcessor, deLayerFrameManager& _frameManager, int _index)
:deLayerFrame(parent, name, _layer, _frameManager, _index)
{
    std::vector<std::string> names;
    std::vector<std::string>::iterator i;

    deWindow& window = getWindow();

    names.clear();
    layer.getProperties(names);

    int width = 600;
    int widthn = 120;
    int widthl = 35;

    for (i = names.begin(); i != names.end(); i++)
    {
        std::string n = *i;
        deProperty* property = layer.getProperty(n);

        dePropertyNumeric* numeric = dynamic_cast<dePropertyNumeric*>(property);
        if (numeric)
        {
            dePropertyNumericUI* p = new dePropertyNumericUI(window, *numeric, _layerProcessor, _index, width - widthn - widthl, widthn, widthl);

            numerics.push_back(p);

            addWidget(p->getWindow());
        }            

        dePropertyLevels* propertyLevels = dynamic_cast<dePropertyLevels*>(property);
        if (propertyLevels)
        {
            deBaseLayerWithSource& layerWithSource = dynamic_cast<deBaseLayerWithSource&>(layer);
            dePropertyLevelsUI* p = new dePropertyLevelsUI(window, *propertyLevels, _layerProcessor, _index, layerWithSource, width);

            levels.push_back(p);

            addWidget(p->getWindow());
        }            
    }

    names.clear();
    layer.getPresets(names);

    for (i = names.begin(); i != names.end(); i++)
    {
        std::string n = *i;
        dePresetButton* b = new dePresetButton(window, layer, n, _layerProcessor, _index, *this);

        addWidget(b->getWindow());
    }


    fit();
}

deGenericLayerFrame::~deGenericLayerFrame()
{
}

void deGenericLayerFrame::setUIFromLayer()
{
    std::vector<dePropertyNumericUI*>::iterator i;
    for (i = numerics.begin(); i != numerics.end(); i++)
    {
        (*i)->setFromProperty();
    }
    std::vector<dePropertyLevelsUI*>::iterator j;
    for (j = levels.begin(); j != levels.end(); j++)
    {
        (*j)->setFromProperty();
    }
}


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
#include "property_choice_ui.h"
#include "property_boolean_ui.h"
#include "property_levels_ui.h"
#include "property_curves_ui.h"
#include "property_mixer_ui.h"
#include "property_numeric.h"
#include "property_levels.h"
#include "property_curves.h"
#include "property_mixer.h"
#include "property_boolean.h"
#include "preset_button.h"
#include "layer_processor.h"

deGenericLayerFrame::deGenericLayerFrame(deWindow& parent, const std::string& name, deBaseLayer& _layer, deLayerProcessor& _layerProcessor, deLayerFrameManager& _frameManager, int _index)
:deLayerFrame(parent, name, _layer, _frameManager, _index), layerProcessor(_layerProcessor)
{

    addSizer("channels");

    std::vector<std::string> names;
    std::vector<std::string>::iterator i;

    deWindow& window = getWindow();

    names.clear();
    layer.getProperties(names);

    int width = 400;
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

        dePropertyChoice* choice = dynamic_cast<dePropertyChoice*>(property);
        if (choice)
        {
            dePropertyChoiceUI* p = new dePropertyChoiceUI(window, *choice, _layerProcessor, _index);

            choices.push_back(p);

            std::string s = property->getSizer();
            if (s.size() > 0)
            {
                addWidget(s, p->getWindow());
            }
            else
            {
                addWidget(p->getWindow());
            }


        }            

        dePropertyBoolean* boolean = dynamic_cast<dePropertyBoolean*>(property);
        if (boolean)
        {
            dePropertyBooleanUI* p = new dePropertyBooleanUI(window, *boolean, _layerProcessor, _index);

            booleans.push_back(p);

            std::string s = property->getSizer();
            if (s.size() > 0)
            {
                addWidget(s, p->getWindow());
            }
            else
            {
                addWidget(p->getWindow());
            }
        }            

        dePropertyLevels* propertyLevels = dynamic_cast<dePropertyLevels*>(property);
        if (propertyLevels)
        {
            deBaseLayerWithSource& layerWithSource = dynamic_cast<deBaseLayerWithSource&>(layer);
            dePropertyLevelsUI* p = new dePropertyLevelsUI(window, *propertyLevels, _layerProcessor, _index, layerWithSource, width);

            levels.push_back(p);

            addWidget(p->getWindow());
        }            

        dePropertyCurves* propertyCurves = dynamic_cast<dePropertyCurves*>(property);
        if (propertyCurves)
        {
            deBaseLayerWithSource& layerWithSource = dynamic_cast<deBaseLayerWithSource&>(layer);
            dePropertyCurvesUI* p = new dePropertyCurvesUI(window, *propertyCurves, _layerProcessor, _index, layerWithSource, width);

            curves.push_back(p);

            addWidget(p->getWindow());
        }            

        dePropertyMixer* propertyMixer = dynamic_cast<dePropertyMixer*>(property);
        if (propertyMixer)
        {
            deBaseLayerWithSource& layerWithSource = dynamic_cast<deBaseLayerWithSource&>(layer);
            dePropertyMixerUI* p = new dePropertyMixerUI(window, *propertyMixer, _layerProcessor, _index, layerWithSource, width);

            mixers.push_back(p);

            addWidget(p->getWindow());
        }            
    }

    addSizer("presets");

    names.clear();
    layer.getPresets(names);

    for (i = names.begin(); i != names.end(); i++)
    {
        std::string n = *i;
        dePresetButton* b = new dePresetButton(window, layer, n, _layerProcessor, _index, *this);

        addWidget("presets", b->getWindow());
    }

    fit();
}

deGenericLayerFrame::~deGenericLayerFrame()
{
    // we need to destroy our UI data here

    std::vector<dePropertyNumericUI*>::iterator i;
    for (i = numerics.begin(); i != numerics.end(); i++)
    {
        delete (*i);
    }
    std::vector<dePropertyLevelsUI*>::iterator j;
    for (j = levels.begin(); j != levels.end(); j++)
    {
        delete (*j);
    }
    {
        std::vector<dePropertyCurvesUI*>::iterator j;
        for (j = curves.begin(); j != curves.end(); j++)
        {
            delete (*j);
        }
    }
    std::vector<dePropertyChoiceUI*>::iterator k;
    for (k = choices.begin(); k != choices.end(); k++)
    {
        delete (*k);
    }
    std::vector<dePropertyBooleanUI*>::iterator l;
    for (l = booleans.begin(); l != booleans.end(); l++)
    {
        delete (*l);
    }
    {
        std::vector<dePropertyMixerUI*>::iterator l;
        for (l = mixers.begin(); l != mixers.end(); l++)
        {
            delete (*l);
        }
    }
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
    {
        std::vector<dePropertyCurvesUI*>::iterator j;
        for (j = curves.begin(); j != curves.end(); j++)
        {
            (*j)->setFromProperty();
        }
    }
    std::vector<dePropertyChoiceUI*>::iterator k;
    for (k = choices.begin(); k != choices.end(); k++)
    {
        (*k)->setFromProperty();
    }
    std::vector<dePropertyBooleanUI*>::iterator l;
    for (l = booleans.begin(); l != booleans.end(); l++)
    {
        (*l)->setFromProperty();
    }
    {
        std::vector<dePropertyMixerUI*>::iterator l;
        for (l = mixers.begin(); l != mixers.end(); l++)
        {
            (*l)->setFromProperty();
        }
    }
}

bool deGenericLayerFrame::onImageClick(deValue x, deValue y)
{
    if ((x < 0) || (y < 0) || (x >= 1) || (y >= 1))
    {
        return false;
    }

    {
        std::vector<dePropertyCurvesUI*>::iterator j;
        for (j = curves.begin(); j != curves.end(); j++)
        {
            (*j)->onImageClick(x,y);
        }
    }

    if (layer.onImageClick(x, y))
    {
        setUIFromLayer();
        layerProcessor.markUpdateAllChannels(index);
        return true;
    }        

    return false;
}

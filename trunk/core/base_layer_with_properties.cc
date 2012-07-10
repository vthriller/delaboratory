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

#include "base_layer_with_properties.h"
#include "color_space_utils.h"
#include "logger.h"
#include "semaphore.h"
#include "str.h"
#include "property_numeric.h"
#include "property_boolean.h"
#include "property_choice.h"
#include "preset.h"

deBaseLayerWithProperties::deBaseLayerWithProperties(deColorSpace _colorSpace, deChannelManager& _channelManager)
:deBaseLayer(_colorSpace, _channelManager)
{
}

deBaseLayerWithProperties::~deBaseLayerWithProperties()
{
    std::vector<deProperty*>::iterator i;
    while (properties.size() > 0)
    {
        i = properties.begin();
        delete *i;
        properties.erase(i);
    }
    std::vector<dePreset*>::iterator j;
    while (presets.size() > 0)
    {
        j = presets.begin();
        delete *j;
        presets.erase(j);
    }
}

void deBaseLayerWithProperties::createPropertyNumeric(const std::string& _name, deValue _min, deValue _max)
{
    std::vector<deProperty*>::iterator i;
    for (i = properties.begin(); i != properties.end(); i++)
    {
        deProperty* property = *i;
        if (property->getName() == _name)
        {
            return;
        }
    }
    properties.push_back(new dePropertyNumeric(_name, _min, _max));
}

void deBaseLayerWithProperties::createPropertyChoice(const std::string& _name, const std::vector<std::string>& _choices)
{
    std::vector<deProperty*>::iterator i;
    for (i = properties.begin(); i != properties.end(); i++)
    {
        deProperty* property = *i;
        if (property->getName() == _name)
        {
            return;
        }
    }
    properties.push_back(new dePropertyChoice(_name, _choices));
}

void deBaseLayerWithProperties::createPropertyBoolean(const std::string& _name)
{
    std::vector<deProperty*>::iterator i;
    for (i = properties.begin(); i != properties.end(); i++)
    {
        deProperty* property = *i;
        if (property->getName() == _name)
        {
            return;
        }
    }
    properties.push_back(new dePropertyBoolean(_name));
}

deProperty* deBaseLayerWithProperties::getProperty(const std::string& _name)
{
    std::vector<deProperty*>::iterator i;
    for (i = properties.begin(); i != properties.end(); i++)
    {
        deProperty* property = *i;
        if (property->getName() == _name)
        {
            return property;
        }
    }
    logError("property " + _name + " not found");

    return NULL;
}

const deProperty* deBaseLayerWithProperties::getProperty(const std::string& _name) const
{
    std::vector<deProperty*>::const_iterator i;
    for (i = properties.begin(); i != properties.end(); i++)
    {
        deProperty* property = *i;
        if (property->getName() == _name)
        {
            return property;
        }
    }

    return NULL;
}

void deBaseLayerWithProperties::getProperties(std::vector<std::string>& names) const
{
    std::vector<deProperty*>::const_iterator i;
    for (i = properties.begin(); i != properties.end(); i++)
    {
        deProperty* property = *i;
        names.push_back(property->getName());
    }

}

dePropertyNumeric* deBaseLayerWithProperties::getPropertyNumeric(const std::string& _name)
{
    return dynamic_cast<dePropertyNumeric*>(getProperty(_name));
}

const dePropertyNumeric* deBaseLayerWithProperties::getPropertyNumeric(const std::string& _name) const
{
    return dynamic_cast<const dePropertyNumeric*>(getProperty(_name));
}

dePropertyChoice* deBaseLayerWithProperties::getPropertyChoice(const std::string& _name)
{
    return dynamic_cast<dePropertyChoice*>(getProperty(_name));
}

const dePropertyChoice* deBaseLayerWithProperties::getPropertyChoice(const std::string& _name) const
{
    return dynamic_cast<const dePropertyChoice*>(getProperty(_name));
}

dePropertyBoolean* deBaseLayerWithProperties::getPropertyBoolean(const std::string& _name)
{
    return dynamic_cast<dePropertyBoolean*>(getProperty(_name));
}

const dePropertyBoolean* deBaseLayerWithProperties::getPropertyBoolean(const std::string& _name) const
{
    return dynamic_cast<const dePropertyBoolean*>(getProperty(_name));
}

deValue deBaseLayerWithProperties::getNumericValue(const std::string& name) const
{
    const dePropertyNumeric* p = getPropertyNumeric(name);
    if (p)
    {
        return p->get();
    }
    return 0;
}

void deBaseLayerWithProperties::applyPreset(const std::string& name)
{
    std::vector<dePreset*>::iterator i;
    for (i = presets.begin(); i != presets.end(); i++)
    {
        dePreset* preset = *i;
        if (preset->getName() == name)
        {
            preset->apply(*this);
        }
    }
}

void deBaseLayerWithProperties::getPresets(std::vector<std::string>& names)
{
    std::vector<dePreset*>::iterator i;
    for (i = presets.begin(); i != presets.end(); i++)
    {
        dePreset* preset = *i;
        if (preset)
        {
            names.push_back(preset->getName());
        }
        else
        {   
            logError("NULL preset found");
        }
    }
}

dePreset* deBaseLayerWithProperties::createPreset(const std::string& name)
{
    dePreset* preset = new dePreset(name);
    presets.push_back(preset);
    return preset;
}

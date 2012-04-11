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

#include "layer.h"
#include <sstream>
#include "xml.h"
#include "str.h"
#include "layer_processor.h"
#include "logger.h"
#include "property_value.h"
#include "preset.h"

deLayer::deLayer(deColorSpace _colorSpace, int _index, int _sourceLayer)
:mutex(wxMUTEX_RECURSIVE),
colorSpace(_colorSpace), 
index(_index), 
sourceLayer(_sourceLayer)
{
}

deLayer::~deLayer()
{
    while (properties.size() > 0)
    {
       std::vector<deProperty*>::iterator i = properties.begin();
       deProperty* p = *i;
       delete p;
       properties.erase(i);
    }       
    while (presets.size() > 0)
    {
        std::map<std::string, dePresetLayer*>::iterator i = presets.begin();
        delete i->second;
        presets.erase(i);
    }
}

deColorSpace deLayer::getColorSpace() const
{
    return colorSpace;
}

bool deLayer::hasAction() const
{
    return false;
}

bool deLayer::hasBlending() const
{
    return false;
}

bool deLayer::canDisable() const
{
    return false;
}

bool deLayer::isEnabled() const
{
    return true;
}

void deLayer::setEnabled(bool e)
{
}

void deLayer::saveCommon(xmlNodePtr node)
{
    saveChild(node, "type", getType());
    saveChild(node, "index", str(index));
    saveChild(node, "source", str(sourceLayer));
    saveChild(node, "color_space", getColorSpaceName(colorSpace));
}

bool deLayer::updateImageThreadCall()
{
    return updateImage();
}

void deLayer::lockLayer()
{
    lockWithLog(mutex, "layer mutex");
}

void deLayer::unlockLayer()
{
    mutex.Unlock();
}

void deLayer::process(deLayerProcessType type, int channel)
{
    switch (type)
    {
        case deLayerProcessFull:
        {
            processFull();
            break;
        }
        case deLayerProcessSingleChannel:
        {
            processChannel(channel);
            break;
        }
        case deLayerProcessBlend:
        {
            processBlend();
            break;
        }
        default:
            break;
    }
}

void deLayer::processFull()
{
    updateImageThreadCall();
}

void deLayer::processBlend()
{
}

void deLayer::processChannel(int channel)
{
}

std::string deLayer::getLabel() const 
{
    return getType();
}    

int deLayer::registerPropertyValue(const std::string& _name)
{
    dePropertyValue* p = new dePropertyValue(_name);
    properties.push_back(p);
    return properties.size() - 1;
}

dePropertyValue* deLayer::getPropertyValue(int index)
{
    if ((index < 0) || ((unsigned int)(index) >= properties.size()))
    {
        return NULL;
    }
    deProperty* property = properties[index];

    return dynamic_cast<dePropertyValue*>(property);
}

void deLayer::saveValueProperties(xmlNodePtr root)
{
    int n = getNumberOfProperties();
    int i;
    for (i = 0; i < n; i++)
    {
        dePropertyValue* p = getPropertyValue(i);
        p->save(root);
    }
}    

void deLayer::loadValueProperties(xmlNodePtr root)
{
    int n = getNumberOfProperties();
    int i;
    for (i = 0; i < n; i++)
    {
        dePropertyValue* p = getPropertyValue(i);
        p->load(root);
    }
}       

dePropertyValue* deLayer::getPropertyValue(const std::string& _name)
{
    int n = getNumberOfProperties();
    int i;
    for (i = 0; i < n; i++)
    {
        dePropertyValue* p = getPropertyValue(i);
        if (p->getName() == _name)
        {
            return p;
        }
    }
    return NULL;
}

bool deLayer::applyPreset(const std::string& _name)
{
    std::map<std::string, dePresetLayer*>::iterator i = presets.find(_name);

    if (i == presets.end())
    {
        return false;
    }

    dePresetLayer* preset = i->second;

    preset->apply();

    return true;
}

void deLayer::getPresets(std::vector<std::string>& result)
{
    std::map<std::string, dePresetLayer*>::iterator i;
    for (i = presets.begin(); i != presets.end(); i++)
    {
        std::string n = i->first;
        result.push_back(n);
    }
}


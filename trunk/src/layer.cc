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
#include "color_space_utils.h"

deLayerOld::deLayerOld(deColorSpace _colorSpace, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager)
:deLayerWithBlending(_colorSpace, _channelManager, _sourceLayer, _layerStack)
{
}

deLayerOld::~deLayerOld()
{
    while (properties.size() > 0)
    {
       std::vector<deProperty*>::iterator i = properties.begin();
       deProperty* p = *i;
       delete p;
       properties.erase(i);
    }       
//    unlockLayer();
}

std::string deLayerOld::getLabel() const 
{
    return getType();
}    

int deLayerOld::registerPropertyValue(const std::string& _name)
{
    dePropertyValue* p = new dePropertyValue(_name);
    properties.push_back(p);
    return properties.size() - 1;
}

dePropertyValue* deLayerOld::getPropertyValue(int index)
{
    if ((index < 0) || ((unsigned int)(index) >= properties.size()))
    {
        return NULL;
    }
    deProperty* property = properties[index];

    return dynamic_cast<dePropertyValue*>(property);
}

void deLayerOld::saveValueProperties(xmlNodePtr root)
{
    int n = getNumberOfProperties();
    int i;
    for (i = 0; i < n; i++)
    {
        dePropertyValue* p = getPropertyValue(i);
        p->save(root);
    }
}    

void deLayerOld::loadValueProperties(xmlNodePtr root)
{
    int n = getNumberOfProperties();
    int i;
    for (i = 0; i < n; i++)
    {
        dePropertyValue* p = getPropertyValue(i);
        p->load(root);
    }
}       

dePropertyValue* deLayerOld::getPropertyValue(const std::string& _name)
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


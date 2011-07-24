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

#include "layer_stack.h"
#include "layer.h"
#include "exception.h"
#include <iostream>
#include "action_frame.h"
#include "layer_factory.h"

deLayerStack::deLayerStack()
{
}

deLayerStack::~deLayerStack()
{
    clear();
}

void deLayerStack::clear()
{
    while (layers.size() > 0)
    {
        removeTopLayer();
    }
}

deLayer* deLayerStack::getLayer(int id) const
{
    unsigned int i = id;
    if ((i >= layers.size()) || (id < 0))
    {
        return NULL;
    }
    return layers[i];
}

void deLayerStack::addLayer(deLayer* layer)
{
    layers.push_back(layer);
}

void deLayerStack::removeTopLayer()
{
    std::vector<deLayer*>::iterator i;
    i = layers.end();    
    i--;
    deLayer* layer = *i;
    delete layer;
    layers.erase(i);
}

int deLayerStack::getSize() const
{
    return layers.size();
}

void deLayerStack::traceSampler(deSampler* sampler)
{
    std::vector<deLayer*>::iterator i;
    for (i = layers.begin(); i != layers.end(); i++)
    {
        deActionFrame* frame = (*i)->getActionFrame();
        if (frame)
        {
            frame->traceSampler(sampler);
        }
    }
}

void deLayerStack::updateColorSpaces()
{
    std::vector<deLayer*>::iterator i;
    for (i = layers.begin(); i != layers.end(); i++)
    {
        (*i)->updateColorSpace();
    }        
}

void deLayerStack::save(xmlNodePtr node)
{
    std::vector<deLayer*>::iterator i;
    for (i = layers.begin(); i != layers.end(); i++)
    {
        xmlNodePtr child = xmlNewChild(node, NULL, xmlCharStrdup("layer"), NULL);
        (*i)->save(child);
    }
}

void deLayerStack::loadLayer(xmlNodePtr node, deLayerFactory& factory)
{
    xmlNodePtr child = node->xmlChildrenNode;
    std::string type = "";
    std::string name = "";

    while (child)
    {
        if ((!xmlStrcmp(child->name, xmlCharStrdup("type")))) 
        {
            xmlChar* s = xmlNodeGetContent(child);            
            type = (char*)(s);
            xmlFree(s);
        }

        if ((!xmlStrcmp(child->name, xmlCharStrdup("name")))) 
        {
            xmlChar* s = xmlNodeGetContent(child);            
            name = (char*)(s);
            xmlFree(s);
        }

        child = child->next;
    }

    if (type.size() > 0)
    {
        int index = layers.size();

        deLayer* layer = factory.createLayer(type, *this, index, name);

        if (layer)
        {
            layer->load(node);
            addLayer(layer);
        }
    }
    
}

void deLayerStack::load(xmlNodePtr node, deLayerFactory& factory)
{
    xmlNodePtr child = node->xmlChildrenNode;

    while (child)
    {
        loadLayer(child, factory);
        child = child->next;
    }
}

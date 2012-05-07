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
#include "channel_manager.h"
#include "str.h"
#include "memory_info_frame.h"
#include "project.h"

deLayerStack::deLayerStack()
{
}

deLayerStack::~deLayerStack()
{
    clear();
}

void deLayerStack::lock() const
{
    mutex.lock();
}

void deLayerStack::unlock() const
{
    mutex.unlock();
}

void deLayerStack::clear()
{
    while (layers.size() > 0)
    {
        removeTopLayer();
    }
}

void deLayerStack::removeTopLayer()
{
    logInfo("layer stack remove top layer...");
    lock();

    std::vector<deBaseLayer*>::iterator i;
    i = layers.end();    
    i--;
    deBaseLayer* layer = *i;

    layers.erase(i);
    unlock();

    layer->lockLayer();
    logInfo("layer stack before delete layer");
    delete layer;
    logInfo("layer stack after delete layer");

}

void deLayerStack::addLayer(deBaseLayer* layer)
{
    logInfo("layer stack add layer");
    lock();

    layers.push_back(layer);

    unlock();
}

int deLayerStack::getSize() const
{
    lock();
    int n = layers.size();
    unlock();
    return n;
}

deBaseLayer* deLayerStack::getLayer(int id) const
{
    lock();
    unsigned int i = id;
    if ((i >= layers.size()) || (id < 0))
    {
        unlock();
        return 0;
    }
    unlock();
    return layers[i];
}

void deLayerStack::save(xmlNodePtr node)
{
    std::vector<deBaseLayer*>::iterator i;
    for (i = layers.begin(); i != layers.end(); i++)
    {
        xmlNodePtr child = xmlNewChild(node, NULL, BAD_CAST("layer"), NULL);
        (*i)->save(child);
    }
}

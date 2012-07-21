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
#include "base_layer.h"
#include "channel_manager.h"
#include "str.h"
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
#ifdef DEBUG_LOG
    logInfo("layer stack lock...");
#endif    
    mutex.lock();
#ifdef DEBUG_LOG
    logInfo("layer stack locked");
#endif    
}

void deLayerStack::unlock() const
{
#ifdef DEBUG_LOG
    logInfo("layer stack unlock");
#endif    
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
    lock();
    logInfo("layer stack remove top layer...");

    int index = layers.size() - 1;

    mutexes[index]->lockWrite();

    std::vector<deBaseLayer*>::iterator i;
    i = layers.end();    
    i--;
    deBaseLayer* layer = *i;

    layers.erase(i);

#ifdef DEBUG_LOG
    logInfo("layer stack before delete layer");
#endif    
    delete layer;
#ifdef DEBUG_LOG
    logInfo("layer stack after delete layer");
#endif    

    mutexes[index]->unlockWrite();

    unlock();

}

void deLayerStack::addLayer(deBaseLayer* layer)
{
    lock();
    logInfo("layer stack add layer");

    layers.push_back(layer);
    mutexes.push_back(new deMutexReadWrite(4));

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

    unsigned int index = id;
    deBaseLayer* layer = NULL;

    if ((index < layers.size()) && (id >= 0))
    {
        layer = layers[index];
    }

    unlock();

    return layer;
}

const deBaseLayer* deLayerStack::startReadLayer(int id) const
{
#ifdef DEBUG_LOG
    logInfo("start read layer " + str(id));
#endif    
    const deBaseLayer* layer = NULL;
    lock();
    if ((id >= 0) && (id < layers.size()))
    {
        mutexes[id]->lockRead();
        layer = getLayer(id);
    }        
    unlock();
#ifdef DEBUG_LOG
    logInfo("start read layer DONE");
#endif    

    return layer;
}

void deLayerStack::finishReadLayer(int id) const
{
#ifdef DEBUG_LOG
    logInfo("finish read layer " + str(id));
#endif    
    lock();
    if ((id >= 0) && (id < layers.size()))
    {
        mutexes[id]->unlockRead();
    }        
    unlock();
}

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

deLayerStack::deLayerStack()
{
}

deLayerStack::~deLayerStack()
{
//    std::cout << " delete layer stack..." << std::endl;
    clear();
//    std::cout << " delete layer stack done" << std::endl;
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
    std::vector<deLayer*>::iterator i;
    i = layers.end();    
    i--;
    deLayer* layer = *i;
    delete layer;
    layers.erase(i);
}

void deLayerStack::addLayer(deLayer* layer)
{
    layers.push_back(layer);
}

int deLayerStack::getSize() const
{
    return layers.size();
}

deLayer* deLayerStack::getLayer(int id) const
{
    unsigned int i = id;
    if ((i >= layers.size()) || (id < 0))
    {
        return 0;
    }
    return layers[i];
}

void deLayerStack::updateImages()
{
    updateImages(0, layers.size() - 1);
}

void deLayerStack::updateImages(int a, int b)
{
    unsigned int i;
    assert(b < layers.size() );
    for (i = a; i <= b; i++)
    {
//        std::cout << "update image of layer " << i << std::endl;
        layers[i]->updateImage();
    }
}

void deLayerStack::updateImagesSmart(deChannelManager& channelManager, int view)
{
    // FIXME channels are enabled/disabled/allocated during update, can't use this method

//    updateImages();
    channelManager.lock();

    std::map<int, int> channelUsage;
    generateChannelUsage(channelUsage);


    std::map<int, int>::iterator i;
    for (i = channelUsage.begin(); i != channelUsage.end(); i++)
    {
//        std::cout << "c: " << i->first << " l: " << i->second << std::endl;
    }

    unsigned int index;
    assert(view < layers.size());
    for (index = 0; index <= view; index++)
    {
//        std::cout << "update layer " << index << std::endl;
        int previous = index - 1;
        if (previous >= 0)
        {
            for (i = channelUsage.begin(); i != channelUsage.end(); i++)
            {
                int c = i->first;
                int l = i->second;
                if (l == previous)
                {
//                    std::cout << "destroy channel " << c << std::endl;
                    channelManager.destroyChannel(c);
                }
            }
        }

        layers[index]->updateImage();
//        std::cout << "channels: " << channelManager.getNumberOfAllocatedChannels() << std::endl;
    }

    channelManager.unlock();
}

void deLayerStack::generateChannelUsage(std::map<int, int>& channelUsage)
{
    channelUsage.clear();
    int i;
    int n = layers.size();
    for (i = 0; i < n; i++)
    {
        deLayer* layer = layers[i];
        layer->updateChannelUsage(channelUsage);
    }
}

void deLayerStack::onKey(int key)
{
    int i;
    int n = layers.size();
    for (i = 0; i < n; i++)
    {
        deLayer* layer = layers[i];
        layer->onKey(key);
    }
}

void deLayerStack::save(xmlNodePtr node)
{
    std::vector<deLayer*>::iterator i;
    for (i = layers.begin(); i != layers.end(); i++)
    {
        xmlNodePtr child = xmlNewChild(node, NULL, BAD_CAST("layer"), NULL);
        (*i)->save(child);
    }
}

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

void deLayerStack::updateAllImages()
{
    updateImages(0, layers.size() - 1);
}

void deLayerStack::updateImages(int a, int b)
{
    unsigned int i;
    assert((unsigned int)b < layers.size() );
    for (i = (unsigned int)a; i <= (unsigned int)b; i++)
    {
        layers[i]->updateImage();
    }
}

void deLayerStack::updateImagesSmart(deChannelManager& channelManager, int view, wxProgressDialog* progressDialog, deMemoryInfoFrame* memoryInfoFrame)
{
    channelManager.lock();

    std::map<int, int> channelUsage;
    generateChannelUsage(channelUsage);

    unsigned int index;
    int progress = 0;
    assert((unsigned int)view < layers.size());
    for (index = 0; index <= (unsigned int)view; index++)
    {
        std::map<int, int>::iterator i;
        int previous = index - 1;
        if (previous >= 0)
        {
            for (i = channelUsage.begin(); i != channelUsage.end(); i++)
            {
                int c = i->first;
                int l = i->second;
                if (l == previous)
                {
                    channelManager.destroyChannel(c);
                }
            }
        }

        deLayer* layer = layers[index];
        std::string label = str(index) + " " + layer->getName();

        progressDialog->Update(progress, wxString::FromAscii(label.c_str()));

        layer->updateImage();

        if (memoryInfoFrame)
        {
            memoryInfoFrame->update();
        }

        if (view > 0)
        {
            progress = 100 * index / view;
        }
        else
        {
            progress = 100;
        }

        progressDialog->Update(progress, wxString::FromAscii(label.c_str()));
    }

    progressDialog->Update(100, _T("finished"));

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

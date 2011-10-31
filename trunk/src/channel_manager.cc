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

#include "channel_manager.h"
#include <cassert>
#include <iostream>

deChannelManager::deChannelManager()
:channelSize(0,0)
{
    locked = false;
    primaryIndex = -1;
}

deChannelManager::~deChannelManager()
{
    unsigned int i;
    for (i = 0; i < channels.size(); i++)
    {
        freeChannel(i);
    }
}

void deChannelManager::setPrimary(int index)
{
    assert(primaryIndex < 0);
    primaryIndex = index;
}

void deChannelManager::setChannelSize(const deSize& size)
{
    channelSize = size;
    destroyAllChannels();
}

int deChannelManager::allocateNewChannel(deImage& image)
{
    assert(!locked);

    deChannel* channel = new deChannel(image);
    channel->allocate(channelSize.getN());

    if (trashed.size() > 0)
    {
        std::set<int>::iterator i = trashed.begin();
        int c = *i;
        trashed.erase(c);
        channels[c] = channel;
        return c;
    }
    else
    {
        channels.push_back(channel);
        int c = channels.size() - 1;
        return c;
    }        
}

void deChannelManager::freeChannel(int index)
{
    assert(!locked);
    assert(index >= 0);
    assert((unsigned int)index < channels.size());

    if (channels[index])
    {
        delete channels[index];
        channels[index] = NULL;
        trashed.insert(index);
    }        
    else
    {
    }
}

deChannel* deChannelManager::getChannel(int index)
{
    if (index < 0)
    {
        return NULL;
    }
    assert((unsigned int)index < channels.size());
    deChannel* channel = channels[index];
    assert(channel);
    tryAllocateChannel(index);
    return channels[index];
}

void deChannelManager::destroyAllChannels()
{
    unsigned int i;
    for (i = 0; i < channels.size(); i++)
    {
        if (channels[i])
        {
            tryDeallocateChannel(i);
        }            
    }
}

deSize deChannelManager::getChannelSize() const
{
    return channelSize;
}

void deChannelManager::lock()
{
    assert(!locked);
    locked = true;
}

void deChannelManager::unlock()
{
    assert(locked);
    locked = false;
}

int deChannelManager::getNumberOfAllocatedChannels() const
{
    int n = 0;
    unsigned int i;
    for (i = 0; i < channels.size(); i++)
    {
        if (channels[i])
        {
            if (channels[i]->isAllocated())
            {
                n++;
            }
        }
    }
    return n;
}

void deChannelManager::tryAllocateChannel(int index)
{
    assert(index >= 0);
    assert((unsigned int)index < channels.size());
    if (!channels[index]->isAllocated())
    {
        channels[index]->allocate(channelSize.getN());
    }        
}

void deChannelManager::tryDeallocateChannel(int index)
{
    assert(index >= 0);
    assert((unsigned int)index < channels.size());
    if (channels[index]->isAllocated())
    {
        channels[index]->deallocate();
    }        
}

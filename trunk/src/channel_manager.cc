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
}

deChannelManager::~deChannelManager()
{
    destroyAllChannels();
}

void deChannelManager::setChannelSize(const deSize& size)
{
    channelSize = size;
    destroyAllChannels();
}

int deChannelManager::allocateNewChannel()
{
    assert(!locked);

    deChannel* channel = new deChannel(channelSize.getN());

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
        return channels.size() - 1;
    }        
}

void deChannelManager::destroyChannel(int index)
{
    assert(index >= 0);
    assert(index < channels.size());

    delete channels[index];
    channels[index] = NULL;
}

void deChannelManager::freeChannel(int index)
{
    assert(!locked);
    assert(index >= 0);
    assert(index < channels.size());

    if (channels[index])
    {
        delete channels[index];
        channels[index] = NULL;
        trashed.insert(index);
    }        
}

void deChannelManager::recreateChannel(int index)
{
    assert(index >= 0);
    assert(index < channels.size());
    assert(!channels[index]);
    channels[index] = new deChannel(channelSize.getN());
}

deChannel* deChannelManager::getChannel(int index)
{
    if (index < 0)
    {
        return NULL;
    }
    if (index >= channels.size())
    {
        return NULL;
    }
    deChannel* channel = channels[index];
    if (!channel)
    {
        recreateChannel(index);
    }
    return channels[index];
}

void deChannelManager::destroyAllChannels()
{
    int i;
    for (i = 0; i < channels.size(); i++)
    {
        destroyChannel(i);
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
    int i;
    for (i = 0; i < channels.size(); i++)
    {
        if (channels[i])
        {
            n++;
        }
    }
    return n;
}

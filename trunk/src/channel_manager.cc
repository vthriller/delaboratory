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
#include <wx/wx.h>
#include "logger.h"

static wxMutex channelManagerMutex;

deChannelManager::deChannelManager(deLogger& _logger)
:channelSize(0,0),
 logger(_logger)
{
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
//    std::cout << "deChannelManager::setChannelSize" << std::endl;
    channelSize = size;
    destroyAllChannels();
}

int deChannelManager::allocateNewChannel()
{
    lock();

    deChannel* channel = new deChannel();
    channel->allocate(channelSize.getN());

    if (trashed.size() > 0)
    {
//        std::cout << "from trash allocate channel " << channelSize.getN() << std::endl;
        std::set<int>::iterator i = trashed.begin();
        int c = *i;
        trashed.erase(c);
        channels[c] = channel;
        unlock();
        return c;
    }
    else
    {
//        std::cout << "allocate channel " << channelSize.getN() << std::endl;
        channels.push_back(channel);
        int c = channels.size() - 1;
        unlock();
        return c;
    }        

}

void deChannelManager::freeChannel(int index)
{
    lock();

    assert(index >= 0);
    assert((unsigned int)index < channels.size());

    if (channels[index])
    {
//        std::cout << "free channel " << index << " " << channelSize.getN() << std::endl;
        delete channels[index];
        channels[index] = NULL;
        trashed.insert(index);
    }        
    else
    {
    }

    unlock();
}

deChannel* deChannelManager::getChannel(int index)
{
    if (index < 0)
    {
        return NULL;
    }
    assert((unsigned int)index < channels.size());
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
    logger.log("channel manager lock");
    channelManagerMutex.Lock();
}

void deChannelManager::unlock()
{
    logger.log("channel manager unlock");
    channelManagerMutex.Unlock();
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
    lock();

    assert(index >= 0);
    assert((unsigned int)index < channels.size());
    if ((channels[index]))
    {
        if (!channels[index]->isAllocated())
        {
    //        std::cout << " try allocate channel " << index << " " << channelSize.getN() << std::endl;
            channels[index]->allocate(channelSize.getN());
        }
    }

    unlock();
}

void deChannelManager::tryDeallocateChannel(int index)
{
    lock();

    assert(index >= 0);
    assert((unsigned int)index < channels.size());
    if ((channels[index]))
    {
        if (channels[index]->isAllocated())
        {
    //        std::cout << " try deallocate channel " << index << " " << channelSize.getN() << std::endl;
            channels[index]->deallocate();
        }
    }

    unlock();
}

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
#include "str.h"

deChannelManager::deChannelManager()
:channelSize(0,0)
{
}

deChannelManager::~deChannelManager()
{
    unsigned int i;
    for (i = 0; i < channels.size(); i++)
    {
        freeChannel(i);
    }
}

void deChannelManager::setChannelSize(const deSize& size)
{
    lock();

    channelSize = size;
    destroyAllChannels();

    unlock();
}

int deChannelManager::allocateNewChannel()
{
    lock();

    deChannel* channel = new deChannel();
    channel->allocate(channelSize.getN());

    if (trashed.size() > 0)
    {
        std::set<int>::iterator i = trashed.begin();
        int c = *i;
        trashed.erase(c);
        channels[c] = channel;
        logMessage("reused trashed channel " + str(c));
        unlock();
        return c;
    }
    else
    {
        channels.push_back(channel);
        int c = channels.size() - 1;
        logMessage("added channel " + str(c));
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
    lock();
    assert((unsigned int)index < channels.size());
    tryAllocateChannel(index);
    deChannel* c = channels[index];
    unlock();
    return c;
}

void deChannelManager::destroyAllChannels()
{
    lock();
    unsigned int i;
    for (i = 0; i < channels.size(); i++)
    {
        if (channels[i])
        {
            tryDeallocateChannel(i);
        }            
    }
    unlock();
}

deSize deChannelManager::getChannelSize() const
{
    return channelSize;
}

void deChannelManager::lock() const
{
    lockWithLog(mutex, "channel manager mutex");
}

void deChannelManager::unlock() const
{
    mutex.unlock();
}

int deChannelManager::getNumberOfAllocatedChannels() const
{
    lock();
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
    unlock();
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
            logMessage("allocate channel " + str(index));
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
            logMessage("deallocate channel " + str(index));
            channels[index]->deallocate();
        }
    }

    unlock();
}

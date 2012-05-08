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
        delete mutexes[i];
    }
}

void deChannelManager::setChannelSize(const deSize& size)
{
    channelSize = size;
    destroyAllChannels();
}

int deChannelManager::reserveNewChannel()
{
    mutex.lock();

    deChannel* channel = new deChannel();
    channel->allocate(channelSize.getN());

    if (trashed.size() > 0)
    {
        std::set<int>::iterator i = trashed.begin();
        int c = *i;
        trashed.erase(c);
        channels[c] = channel;
        logInfo("reused trashed channel " + str(c));
        mutex.unlock();
        return c;
    }
    else
    {
        channels.push_back(channel);
        mutexes.push_back(new deMutexReadWrite(4));
        int c = channels.size() - 1;
        logInfo("added channel " + str(c));
        mutex.unlock();
        return c;
    }        

}

void deChannelManager::freeChannel(int index)
{
    if (index < 0)
    {
        logError("freeChannel index < 0");
        return;
    } 
    else if ((unsigned int)index >= channels.size())
    {
        int s = channels.size();
        logError("freeChannel index >= " + str(s));
        return;
    }


    mutex.lock();
    mutexes[index]->lockWrite();

    assert(index >= 0);
    assert((unsigned int)index < channels.size());

    if (channels[index])
    {
        delete channels[index];
        channels[index] = NULL;
        trashed.insert(index);
        logInfo("destroyed channel " + str(index));
    }        
    else
    {
    }

    mutexes[index]->unlockWrite();
    mutex.unlock();

}

deChannel* deChannelManager::getChannel(int index)
{
    if (index < 0)
    {
        logError("getChannel index < 0");
        return NULL;
    } 
    else if ((unsigned int)index >= channels.size())
    {
        int s = channels.size();
        logError("getChannel index >= " + str(s));
        return NULL;
    }


    tryAllocateChannel(index);
    deChannel* c = channels[index];

    return c;
}

void deChannelManager::destroyAllChannels()
{
    logInfo("destroy all channels start");

    unsigned int i;
    for (i = 0; i < channels.size(); i++)
    {
        tryDeallocateChannel(i);
    }

    logInfo("destroy all channels DONE");
}

deSize deChannelManager::getChannelSizeFromChannelManager() const
{
    return channelSize;
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

    if (index < 0)
    {
        logError("tryAllocateChannel index < 0");
    } 
    else if ((unsigned int)index >= channels.size())
    {
        int s = channels.size();
        logError("tryAllocateChannel index >= " + str(s));
    }

    mutex.lock();

    if ((channels[index]))
    {
        if (!channels[index]->isAllocated())
        {
            channels[index]->allocate(channelSize.getN());
        }
    }

    mutex.unlock();

}

void deChannelManager::tryDeallocateChannel(int index)
{

    if (index < 0)
    {
        logError("tryDeallocateChannel index < 0");
    } 
    else if ((unsigned int)index >= channels.size())
    {
        int s = channels.size();
        logError("tryDeallocateChannel index >= " + str(s));
    }

    mutex.lock();

    if ((channels[index]))
    {
        if (channels[index]->isAllocated())
        {
            channels[index]->deallocate();
        }
    }

    mutex.unlock();

}

bool deChannelManager::isImageEmpty() const
{
    return channelSize.getN() == 0;
}    

const deValue* deChannelManager::startRead(int index)
{
    logInfo("startRead " + str(index));

    if (index < 0)
    {
        logError("startRead index < 0");
        return NULL;
    } 
    else if ((unsigned int)index >= channels.size())
    {
        int s = channels.size();
        logError("startRead index >= " + str(s));
        return NULL;
    }

    mutexes[index]->lockRead();

    return channels[index]->getPixels();
}

void deChannelManager::finishRead(int index)
{
    logInfo("finishRead " + str(index));

    if (index < 0)
    {
        logError("finishRead index < 0");
        return;
    } 
    else if ((unsigned int)index >= channels.size())
    {
        int s = channels.size();
        logError("finishRead index >= " + str(s));
        return;
    }

    mutexes[index]->unlockRead();
}

deValue* deChannelManager::startWrite(int index)
{
    logInfo("startWrite " + str(index));

    if (index < 0)
    {
        logError("startWrite index < 0");
        return NULL;
    } 
    else if ((unsigned int)index >= channels.size())
    {
        int s = channels.size();
        logError("startWrite index >= " + str(s));
        return NULL;
    }

    mutexes[index]->lockWrite();

    logInfo("startWrite " + str(index) + " locked");

    return channels[index]->getPixels();
}

void deChannelManager::finishWrite(int index)
{
    logInfo("finishWrite " + str(index));

    if (index < 0)
    {
        logError("finishWrite index < 0");
        return;
    } 
    else if ((unsigned int)index >= channels.size())
    {
        int s = channels.size();
        logError("finishWrite index >= " + str(s));
        return;
    }

    mutexes[index]->unlockWrite();
}

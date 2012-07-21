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
#include "logger.h"
#include "str.h"

deChannelManager::deChannelManager()
:channelSize(0,0)
{
#ifdef DEBUG_LOG
    logInfo("channel manager constructor");
#endif    
}

deChannelManager::~deChannelManager()
{
#ifdef DEBUG_LOG
    logInfo("channel manager destructor");
#endif    
    unsigned int i;
    for (i = 0; i < channels.size(); i++)
    {
        lock();

        freeChannel(i);
        delete mutexes[i];

        unlock();
    }
#ifdef DEBUG_LOG
    logInfo("channel manager destructor DONE");
#endif    
}

void deChannelManager::setChannelSize(const deSize& size, bool reallocate)
{
    lock();

    channelSize = size;

    if (channelSize.getW() < 0)
    {
        logError("w: " + str(channelSize.getW()) + " when setting channel size in channel manager");
    }
    if (channelSize.getH() < 0)
    {
        logError("h: " + str(channelSize.getH()) + " when setting channel size in channel manager");
    }

    unsigned int i;
    for (i = 0; i < channels.size(); i++)
    {
        logInfo("destroy channel " + str(i));
        mutexes[i]->lockWrite();
        tryDeallocateChannel(i);
        if (reallocate)
        {
            tryAllocateChannel(i);
        }
        mutexes[i]->unlockWrite();
    }

    unlock();
}

void deChannelManager::lock()
{
#ifdef DEBUG_LOG
    logInfo("channel manager before lock");
#endif    
    mutex.lock();
#ifdef DEBUG_LOG
    logInfo("channel manager locked");
#endif    
}

void deChannelManager::unlock()
{
    mutex.unlock();
#ifdef DEBUG_LOG
    logInfo("channel manager unlocked");
#endif    
}

int deChannelManager::reserveNewChannel()
{
    lock();

    deValue* channel = NULL;

    int c = -1;
    if (trashed.size() > 0)
    {
        std::set<int>::iterator i = trashed.begin();
        c = *i;
        trashed.erase(c);
        channels[c] = channel;
        logInfo("reused trashed channel " + str(c));
    }
    else
    {
        channels.push_back(channel);
        mutexes.push_back(new deMutexReadWrite(4));
        c = channels.size() - 1;
        logInfo("added channel " + str(c));
    }        

    unlock();

    return c;
}

void deChannelManager::freeChannel(int index)
{
    lock();

    bool error = false;

    if (index < 0)
    {
        logError("freeChannel index < 0");
        error = true;
    } 
    else if ((unsigned int)index >= channels.size())
    {
        int s = channels.size();
        logError("freeChannel index >= " + str(s));
        error = true;
    }

    if (!error)
    {
        logInfo("freeChannel before lockWrite");
        mutexes[index]->lockWrite();
        logInfo("freeChannel after locks");

        assert(index >= 0);
        assert((unsigned int)index < channels.size());

        if (channels[index])
        {
            delete [] channels[index];
            channels[index] = NULL;
            trashed.insert(index);
            logInfo("destroyed channel " + str(index));
        }        
        else
        {
        }

        mutexes[index]->unlockWrite();
    }

    unlock();

    logInfo("freeChannel after unlocks");

}

deSize deChannelManager::getChannelSizeFromChannelManager() const
{
    if (channelSize.getW() < 0)
    {
        logError("w: " + str(channelSize.getW()) + " when getting channel size from channel manager");
    }
    if (channelSize.getH() < 0)
    {
        logError("h: " + str(channelSize.getH()) + " when getting channel size from channel manager");
    }

    deSize size = channelSize;

    return size;
}

void deChannelManager::tryAllocateChannel(int index)
{
    lock();

    bool error = false;

    if (index < 0)
    {
        logError("tryAllocateChannel index < 0");
        error = true;
    } 
    else if ((unsigned int)index >= channels.size())
    {
        int s = channels.size();
        logError("tryAllocateChannel index >= " + str(s));
        error = true;
    }

    if (!error)
    {
        if (!channels[index])
        {
            logInfo("allocating channel " + str(index));
            try
            {
                deValue* c = new deValue [channelSize.getN()];
                if (c)
                {
                    logInfo("allocated channel " + str(index));
                }
                else
                {
                    logError("unable to allocate channel " + str(index));
                }
                channels[index] = c;
            }                
            catch (std::bad_alloc)
            {
                logError("bad_alloc exception on allocate channel " + str(index));
                channels[index] = NULL;
            }
        }            
    }        

    unlock();

}

void deChannelManager::tryDeallocateChannel(int index)
{
    lock();

    bool error = false;

    if (index < 0)
    {
        logError("tryDeallocateChannel index < 0");
        error = true;
    } 
    else if ((unsigned int)index >= channels.size())
    {
        int s = channels.size();
        logError("tryDeallocateChannel index >= " + str(s));
        error = true;
    }

    if (!error)
    {
        if (channels[index])
        {
            logInfo("deallocating channel " + str(index));
            delete [] channels[index];
            channels[index] = NULL;
        }
    }        

    unlock();

}

bool deChannelManager::isImageEmpty() const
{
    return channelSize.getN() == 0;
}    

const deValue* deChannelManager::startRead(int index)
{
#ifdef DEBUG_LOG
    logInfo("startRead " + str(index));
#endif    

    bool error = false;

    if (index < 0)
    {
        logError("startRead index < 0");
        error = true;
    } 
    else if ((unsigned int)index >= channels.size())
    {
        int s = channels.size();
        logError("startRead index >= " + str(s));
        error = true;
    }

    deValue* p = NULL;

    if (!error)
    {
        mutexes[index]->lockRead();

        p = channels[index];
    }        

    return p;
}

void deChannelManager::finishRead(int index)
{
#ifdef DEBUG_LOG
    logInfo("finishRead " + str(index));
#endif    

    bool error = false;

    if (index < 0)
    {
        logError("finishRead index < 0");
        error = true;
    } 
    else if ((unsigned int)index >= channels.size())
    {
        int s = channels.size();
        logError("finishRead index >= " + str(s));
        error = true;
    }

    if (!error)
    {
        mutexes[index]->unlockRead();
    }
}

deValue* deChannelManager::startWrite(int index)
{
#ifdef DEBUG_LOG
    logInfo("startWrite " + str(index));
#endif    

    bool error = false;

    if (index < 0)
    {
        logError("startWrite index < 0");
        error = true;
    } 
    else if ((unsigned int)index >= channels.size())
    {
        int s = channels.size();
        logError("startWrite index >= " + str(s));
        error = true;
    }

    deValue* p = NULL;

    if (!error)
    {
        mutexes[index]->lockWrite();

#ifdef DEBUG_LOG
        logInfo("startWrite " + str(index) + " locked");
#endif    

        p = channels[index];
    }        

    return p;
}

void deChannelManager::finishWrite(int index)
{
#ifdef DEBUG_LOG
    logInfo("finishWrite " + str(index));
#endif    

    bool error = false;

    if (index < 0)
    {
        logError("finishWrite index < 0");
        error = true;
    } 
    else if ((unsigned int)index >= channels.size())
    {
        int s = channels.size();
        logError("finishWrite index >= " + str(s));
        error = true;
    }

    if (!error)
    {
        mutexes[index]->unlockWrite();
    }        
}

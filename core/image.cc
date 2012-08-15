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

#include "image.h"
#include "channel_manager.h"
#include "logger.h"
#include "str.h"
#include "color_space_utils.h"
#include <cassert>

deImageOld::deImageOld(const deColorSpace& _colorSpace, deChannelManager& _channelManager)
:colorSpace(_colorSpace), channelManager(_channelManager)
{
    logInfo("image constructor");
    int i;
    int s = getColorSpaceSize(colorSpace);
    for (i = 0; i < MAX_COLOR_SPACE_SIZE; i++)
    {
        if (i < s)
        {
            channelsAllocated[i] = channelManager.reserveNewChannel();
        }
        else
        {
            channelsAllocated[i] = -1;
        }
    }        
}

deImageOld::~deImageOld()
{
    logInfo("image destructor");
    int i;
    int s = getColorSpaceSize(colorSpace);
    for (i = 0; i < s; i++)
    {
        int a = channelsAllocated[i];
        logInfo("destroying channel " + str(a));
        channelManager.freeChannel(a);
    }        
}

void deImageOld::allocateChannels()
{
    int s = getColorSpaceSize(colorSpace);
    int i;
    for (i = 0; i < s; i++)
    {
        channelManager.tryAllocateChannel(channelsAllocated[i]);
    }        
}

int deImageOld::getChannelIndex(int n) const
{
    if (n < 0)
    {
        logError("deImageOld::getChannelIndex n: " + str(n));
    }
    else if (n >= MAX_COLOR_SPACE_SIZE)
    {
        logError("deImageOld::getChannelIndex n: " + str(n));
    }
    return channelsAllocated[n];
}

deColorSpace deImageOld::getColorSpace() const
{
    return colorSpace;
}

void deImageOld::updateChannelUsage(std::map<int, int>& channelUsage, int index) const
{
    int i;
    int s = getColorSpaceSize(colorSpace);
    for (i = 0; i < s; i++)
    {
        int c = channelsAllocated[i];
        channelUsage[c] = index;
    }        
}

const deValue* deImageOld::startRead(int channel) const
{
#ifdef DEBUG_LOG
    logInfo("image start read " + str(channel));
#endif    

    if (channel < 0)
    {
        logError("image start read " + str(channel));
        return NULL;
    }
    else if (channel >= MAX_COLOR_SPACE_SIZE)
    {
        logError("image start read " + str(channel));
        return NULL;
    }
    int index = channelsAllocated[channel];

    const deValue* values = channelManager.startRead(index);

    return values;
}

void deImageOld::finishRead(int channel) const
{
#ifdef DEBUG_LOG
    logInfo("image finish read " + str(channel));
#endif    
    if (channel < 0)
    {
        logError("image finish read " + str(channel));
        return;
    }
    else if (channel >= MAX_COLOR_SPACE_SIZE)
    {
        logError("image finish read " + str(channel));
        return;
    }
    int index = channelsAllocated[channel];
    channelManager.finishRead(index);

}

deValue* deImageOld::startWrite(int channel) 
{
#ifdef DEBUG_LOG
    logInfo("image start write " + str(channel));
#endif    

    if (channel < 0)
    {
        logError("image start write " + str(channel));
        return NULL;
    }
    else if (channel >= MAX_COLOR_SPACE_SIZE)
    {
        logError("image start write " + str(channel));
        return NULL;
    }

    int index = channelsAllocated[channel];

    deValue* values = channelManager.startWrite(index);

    return values;
}

void deImageOld::finishWrite(int channel) 
{
#ifdef DEBUG_LOG
    logInfo("image finish write " + str(channel));
#endif    
    if (channel < 0)
    {
        logError("image finish write " + str(channel));
        return;
    }
    else if (channel >= MAX_COLOR_SPACE_SIZE)
    {
        logError("image finish write " + str(channel));
        return;
    }
    int index = channelsAllocated[channel];
    channelManager.finishWrite(index);

}    

const deSize deImageOld::getChannelSize() const
{
    return channelManager.getChannelSizeFromChannelManager();
}


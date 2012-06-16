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

deImage::deImage(const deColorSpace& _colorSpace, deChannelManager& _channelManager)
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

deImage::~deImage()
{
    logInfo("image destructor");
//    mutex.lock();
    int i;
    int s = getColorSpaceSize(colorSpace);
    for (i = 0; i < s; i++)
    {
        int a = channelsAllocated[i];
        logInfo("destroying channel " + str(a));
        channelManager.freeChannel(a);
    }        
//    mutex.unlock();
}

void deImage::allocateChannels()
{
    int s = getColorSpaceSize(colorSpace);
    int i;
    for (i = 0; i < s; i++)
    {
        channelManager.tryAllocateChannel(channelsAllocated[i]);
    }        
}

int deImage::getChannelIndex(int n) const
{
    if (n < 0)
    {
        logError("deImage::getChannelIndex n: " + str(n));
    }
    else if (n >= MAX_COLOR_SPACE_SIZE)
    {
        logError("deImage::getChannelIndex n: " + str(n));
    }
    return channelsAllocated[n];
}

deColorSpace deImage::getColorSpace() const
{
    return colorSpace;
}

void deImage::updateChannelUsage(std::map<int, int>& channelUsage, int index) const
{
    int i;
    int s = getColorSpaceSize(colorSpace);
    for (i = 0; i < s; i++)
    {
        int c = channelsAllocated[i];
        channelUsage[c] = index;
    }        
}

const deValue* deImage::startRead(int channel) const
{
    logInfo("image start read " + str(channel));
//    mutex.lock();

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

void deImage::finishRead(int channel) const
{
    logInfo("image finish read " + str(channel));
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

//    mutex.unlock();
}

deValue* deImage::startWrite(int channel) 
{
    logInfo("image start write " + str(channel));
//    mutex.lock();

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

void deImage::finishWrite(int channel) 
{
    logInfo("image finish write " + str(channel));
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

//    mutex.unlock();
}    

const deSize deImage::getChannelSize() const
{
    return channelManager.getChannelSizeFromChannelManager();
}


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

deImage::deImage(const deColorSpace& _colorSpace, deChannelManager& _channelManager)
:colorSpace(_colorSpace), channelManager(_channelManager)
{
    logMessage("create new image");
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
        channelsVisible[i] = -1;
    }        
}

deImage::~deImage()
{
    logMessage("destroy image");
    int i;
    int s = getColorSpaceSize(colorSpace);
    for (i = 0; i < s; i++)
    {
        assert (channelsAllocated[i] >= 0);
        channelManager.freeChannel(channelsAllocated[i]);
    }        
}

void deImage::enableChannel(int n)
{
    logMessage("enable channel " + str(n) + " in image");
    assert(n >= 0);
    assert(n < MAX_COLOR_SPACE_SIZE);
    assert (channelsAllocated[n] >= 0);
    channelManager.tryAllocateChannel(channelsAllocated[n]);
    channelsVisible[n] = channelsAllocated[n];
}

void deImage::disableChannel(int n, int c)
{
    logMessage("disable channel " + str(n) + " in image, replace with " +str(c));
    assert(n >= 0);
    assert(n < MAX_COLOR_SPACE_SIZE);
    channelsVisible[n] = c;
    channelManager.tryDeallocateChannel(channelsAllocated[n]);
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
    return channelsVisible[n];
}

deColorSpace deImage::getColorSpace() const
{
    return colorSpace;
}

void deImage::disableAllChannels()
{
    int i;
    int s = getColorSpaceSize(colorSpace);
    for (i = 0; i < s; i++)
    {
        disableChannel(i, -1);
    }        
    
}

void deImage::enableAllChannels()
{
    int i;
    int s = getColorSpaceSize(colorSpace);
    for (i = 0; i < s; i++)
    {
        enableChannel(i);
    }        
}

void deImage::updateChannelUsage(std::map<int, int>& channelUsage, int index) const
{
    int i;
    int s = getColorSpaceSize(colorSpace);
    for (i = 0; i < s; i++)
    {
        int c = channelsVisible[i];
        channelUsage[c] = index;
    }        
}

const deValue* deImage::startRead(int channel) const
{
    logMessage("image start read " + str(channel));
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
    int index = channelsVisible[channel];
    channelManager.startRead(index);
    deChannel* c = channelManager.getChannel(index);
    if (!c)
    {
        return NULL;
    }
    return c->getPixels();
}

void deImage::finishRead(int channel) const
{
    int index = channelsVisible[channel];
    channelManager.finishRead(index);
}

deValue* deImage::startWrite(int channel) 
{
    logMessage("image start write " + str(channel));
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

    int index = channelsVisible[channel];
    channelManager.startWrite(index);

    deChannel* c = channelManager.getChannel(index);
    if (!c)
    {
        return NULL;
    }
    return c->getPixels();
}

void deImage::finishWrite(int channel) 
{
    int index = channelsVisible[channel];
    channelManager.finishWrite(index);
}    

const deSize deImage::getChannelSize() const
{
    return channelManager.getChannelSizeFromChannelManager();
}

bool deImage::isReady() const
{
    int i;
    int n = getColorSpaceSize(colorSpace);
    for (i = 0; i < n; i++)
    {
        if (channelsVisible[i] < 0)
        {
            return false;
        }
    }
    return true;

}

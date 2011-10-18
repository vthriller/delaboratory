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

deImage::deImage(const deColorSpace& _colorSpace, deChannelManager& _channelManager)
:colorSpace(_colorSpace), channelManager(_channelManager)
{
    int i;
    for (i = 0; i < 4; i++)
    {
        channelsAllocated[i] = -1;
        channelsVisible[i] = -1;
    }        
}

deImage::~deImage()
{
    int i;
    int s = getColorSpaceSize(colorSpace);
    for (i = 0; i < s; i++)
    {
        if (channelsAllocated[i] >= 0)
        {
            channelManager.freeChannel(channelsAllocated[i]);
        }            
    }        
}

void deImage::enableChannel(int n)
{
    if (channelsAllocated[n] < 0)
    {
        channelsAllocated[n] = channelManager.allocateNewChannel();
    }
    channelsVisible[n] = channelsAllocated[n];
}

void deImage::disableChannel(int n, int c)
{
    channelsVisible[n] = c;
    if (channelsAllocated[n] >= 0)
    {
        channelManager.freeChannel(channelsAllocated[n]);
        channelsAllocated[n] = -1;
    }            
}

int deImage::getChannelIndex(int n) const
{
    assert(n >= 0);
    assert(n < 4);
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

deValue deImage::getPixel(int channel, int p) const
{
    deChannel* c = channelManager.getChannel(channelsVisible[channel]);
    if (!c)
    {
        return -1;
    }
    return c->getValue(p);

}

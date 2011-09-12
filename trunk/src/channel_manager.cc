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

deChannelManager::deChannelManager()
:channelSize(0,0)
{
}

deChannelManager::~deChannelManager()
{
}

void deChannelManager::setChannelSize(const deSize& size)
{
    channelSize = size;
}

int deChannelManager::allocateNewChannel()
{
    deChannel* channel = new deChannel(channelSize.getN());
    return insertNewChannel(channel);
}

int deChannelManager::destroyChannel(int index)
{
    delete channels[index];
    channels[index] = NULL;
}

int deChannelManager::insertNewChannel(deChannel* channel)
{
    std::vector<deChannel*>::iterator i;
    int n = 0;
    for (i = channels.begin(); i != channels.end(); i++)
    {
        deChannel* c = *i;
        if (c == NULL)
        {
            channels[n] = channel;
            return n;
        }
        n++;
    }
    channels.push_back(channel);
    return n;
}

deChannel* deChannelManager::getChannel(int index)
{
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


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

#include "switchable_layer.h"
#include "color_space_utils.h"

deSwitchableLayer::deSwitchableLayer(deColorSpace _colorSpace, deChannelManager& _channelManager, int _sourceLayerIndex, deLayerStack& _layerStack)
:deBaseLayerWithSource(_colorSpace, _channelManager, _sourceLayerIndex, _layerStack)
{
    enabled = true;

    int n = getColorSpaceSize(colorSpace);
    int i;
    for (i = 0; i < n; i++)
    {
        channels.insert(i);
    }
}

deSwitchableLayer::~deSwitchableLayer()
{
}

bool deSwitchableLayer::isEnabled() const
{
    return enabled;
}

void deSwitchableLayer::setEnabled(bool e)
{
    enabled = e;
}

void deSwitchableLayer::disableNotLuminance()
{
    int n = getColorSpaceSize(colorSpace);
    int i;
    for (i = 0; i < n; i++)
    {
        if (!isChannelLuminance(colorSpace, i))
        {
            channels.erase(i);
        }            
    }
}

void deSwitchableLayer::disableNotForSharpen()
{
    int n = getColorSpaceSize(colorSpace);
    int i;
    for (i = 0; i < n; i++)
    {
        if (!shouldChannelBeSharpened(colorSpace, i))
        {
            channels.erase(i);
        }            
    }
}

bool deSwitchableLayer::isChannelEnabled(int index) const
{
    return channels.count(index) > 0;
}

void deSwitchableLayer::enableChannel(int index)
{
    channels.insert(index);
}

void deSwitchableLayer::disableChannel(int index)
{
    channels.erase(index);
}

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

#include "apply_original_layer.h"
#include "color_space_utils.h"
#include "logger.h"

deApplyOriginalLayer::deApplyOriginalLayer(deColorSpace _colorSpace, deChannelManager& _channelManager, int _sourceLayer, deLayerStack& _layerStack)
:deLayerWithBlending(_colorSpace, _channelManager, _sourceLayer, _layerStack)
{

}

deApplyOriginalLayer::~deApplyOriginalLayer()
{
}

bool deApplyOriginalLayer::updateMainImageSingleChannel(int i)
{
    if (!isChannelEnabled(i))
    {
        mainLayerImage.disableChannel(i, getSourceImage().getChannelIndex(i));
        return true;
    }

    const deImage& appliedImage = getOriginalImage();

    int n = getColorSpaceSize(appliedImage.getColorSpace());

    if (i < n)
    {
        int c = appliedImage.getChannelIndex(i);
        mainLayerImage.disableChannel(i, c);
    }
    else
    {
        mainLayerImage.disableChannel(i, getSourceImage().getChannelIndex(i));
    }

    return true;
}

void deApplyOriginalLayer::updateChannelUsage(std::map<int, int>& channelUsage, int layerIndex) const
{
    deLayerWithBlending::updateChannelUsage(channelUsage, layerIndex);

    getOriginalImage().updateChannelUsage(channelUsage, layerIndex);
}

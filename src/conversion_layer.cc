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

#include "conversion_layer.h"
#include "project.h"
#include "layer_processor.h"
#include "layer_stack.h"
#include "conversion_processor.h"

deConversionLayer::deConversionLayer(deColorSpace _colorSpace, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager)
:deBaseLayerWithSource( _colorSpace, _channelManager, _sourceLayer, _layerStack)
{

}

deConversionLayer::~deConversionLayer()
{
}

bool deConversionLayer::updateMainImageNotThreadedWay()
{
    logMessage("conversion start");

    mainLayerImage.enableAllChannels();

    deConversionProcessor p;
    p.convertImage(getSourceImage(), mainLayerImage, channelManager);

    logMessage("conversion end");

    return true;
}

void deConversionLayer::updateChannelUsage(std::map<int, int>& channelUsage, int layerIndex) const
{
    getSourceImage().updateChannelUsage(channelUsage, layerIndex);

    deBaseLayer::updateChannelUsage(channelUsage, layerIndex);
}


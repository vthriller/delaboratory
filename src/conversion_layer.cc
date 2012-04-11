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
#include "convert_image.h"
#include "layer_processor.h"
#include "layer_stack.h"

deConversionLayer::deConversionLayer(deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager)
:deLayer( _colorSpace, _index, _sourceLayer), 
 layerStack(_layerStack),
 channelManager(_channelManager),
 image(_colorSpace, _channelManager) 
{

}

deConversionLayer::~deConversionLayer()
{
}

const deImage& deConversionLayer::getImage() const
{
    return image;
}

bool deConversionLayer::updateImage()
{
    logMessage("conversion start");

    deLayer* source = layerStack.getLayer(sourceLayer);
    const deImage& sourceImage = source->getImage();

    image.enableAllChannels();
    convertImage(sourceImage, image, channelManager);

    logMessage("conversion end");

    return true;
}

void deConversionLayer::updateChannelUsage(std::map<int, int>& channelUsage) const
{
    deLayer* source = layerStack.getLayer(sourceLayer);
    const deImage& sourceImage = source->getImage();
    sourceImage.updateChannelUsage(channelUsage, index);
    image.updateChannelUsage(channelUsage, index);
}


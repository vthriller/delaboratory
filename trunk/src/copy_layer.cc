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

#include "copy_layer.h"
#include "project.h"
#include "layer_processor.h"
#include "layer_stack.h"
#include "conversion_processor.h"
#include "str.h"
#include "preset.h"
#include "films.h"

deCopyLayer::deCopyLayer(deColorSpace _colorSpace, deChannelManager& _channelManager, int _sourceLayer, deLayerStack& _layerStack)
:deLayerWithBlending( _colorSpace, _channelManager, _sourceLayer, _layerStack)
{
    int size = getLayerStackSize();
    std::vector<std::string> numbers = createNumbers(size);

    createPropertyChoice("layer", numbers);
}

deCopyLayer::~deCopyLayer()
{
}

bool deCopyLayer::updateMainImageNotThreadedWay()
{
    logInfo("conversion layer start");

    mainLayerImage.enableAllChannels();

    deConversionProcessor p;

    deConversionCPU cpu(4);

    int l = getPropertyChoice("layer")->getIndex();
    const deImage& image = getOtherLayerImage(l);
    p.convertImage(image, mainLayerImage, cpu);

    deValue overflow = cpu.registers[CPU_REGISTER_OVERFLOW];
    int n = image.getChannelSize().getN();
    int percentage = overflow * 10000 / n;

    logInfo("conversion layer DONE");

    return true;
}

void deCopyLayer::updateChannelUsage(std::map<int, int>& channelUsage, int layerIndex) const
{
    int l = getPropertyChoice("layer")->getIndex();
    const deImage& image = getOtherLayerImage(l);
    image.updateChannelUsage(channelUsage, layerIndex);

    deLayerWithBlending::updateChannelUsage(channelUsage, layerIndex);
}


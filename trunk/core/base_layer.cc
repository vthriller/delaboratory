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

#include "base_layer.h"
#include "logger.h"
#include "str.h"
#include "update_main_layer_image.h"


deBaseLayer::deBaseLayer(deColorSpace _colorSpace, deChannelManager& _channelManager)
:colorSpace(_colorSpace),
 mainLayerImage(_colorSpace, _channelManager)
{
}

deBaseLayer::~deBaseLayer()
{
}

deColorSpace deBaseLayer::getColorSpace() const
{
    return colorSpace;
}

void deBaseLayer::lockLayer()
{
#ifdef DEBUG_LOG
    logInfo("base layer before lock");
#endif    
    mutex.lock();
#ifdef DEBUG_LOG
    logInfo("base layer locked");
#endif    
}

void deBaseLayer::unlockLayer()
{
    mutex.unlock();
#ifdef DEBUG_LOG
    logInfo("base layer unlocked");
#endif    
}

void deBaseLayer::processLayer(deLayerProcessType type, int channel)
{
#ifdef DEBUG_LOG
    logInfo("processLayer channel: " + str(channel));
#endif    
    switch (type)
    {
        case deLayerProcessFull:
        {
            processFull();
            break;
        }
        case deLayerProcessSingleChannel:
        {
            processSingleChannel(channel);
            break;
        }
        case deLayerProcessBlend:
        {
            updateBlendAllChannels();
            break;
        }
        default:
            break;
    }
#ifdef DEBUG_LOG
    logInfo("processLayer DONE");
#endif    
}

bool deBaseLayer::processFull()
{
    return updateImage();
}

const deImageOld& deBaseLayer::getLayerImage() const
{
    return mainLayerImage;
}

void deBaseLayer::updateChannelUsage(std::map<int, int>& channelUsage, int layerIndex) const
{
    mainLayerImage.updateChannelUsage(channelUsage, layerIndex);
}

void deBaseLayer::processSingleChannel(int channel)
{
    processMainImageSingleChannel(channel);
}    

bool deBaseLayer::processMainImageSingleChannel(int channel)
{
    bool result = true;
#ifdef DEBUG_LOG
    logInfo("base layer process main image single channel");
#endif    
    result = updateMainImageSingleChannel(channel);
#ifdef DEBUG_LOG
    logInfo("base layer process main image single channel DONE");
#endif    
    return result;
}

bool deBaseLayer::updateImage()
{
    bool result = updateMainImageNotThreadedWay();
    if (!result)
    {
        updateMainImageAllChannels(*this);
        result = true;
    }

    return result;
}

void deBaseLayer::allocateChannels()
{
    mainLayerImage.allocateChannels();
}

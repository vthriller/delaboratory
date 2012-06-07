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
#include "color_space_utils.h"
#include "logger.h"
#include "semaphore.h"
#include "str.h"
#include "property_numeric.h"
#include "property_boolean.h"
#include "property_choice.h"
#include "preset.h"
#include <wx/wx.h>

class deUpdateActionThread:public wxThread
{
    private:
        virtual void *Entry()
        {
            bool result = layer.processMainImageSingleChannel(channel);
            if (!result)
            {
                logError("update action failed");
            }
            semaphore.post();
            return NULL;
        }
        deBaseLayer& layer;
        int channel;
        deSemaphore& semaphore;
    public:    
        deUpdateActionThread(deBaseLayer& _layer, int _channel, deSemaphore& _semaphore)
        :layer(_layer),
         channel(_channel),
         semaphore(_semaphore)
        {
        }
        virtual ~deUpdateActionThread()
        {
        }
};


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
    logInfo("base layer before lock");
    mutex.lock();
    logInfo("base layer locked");
}

void deBaseLayer::unlockLayer()
{
    mutex.unlock();
    logInfo("base layer unlocked");
}

void deBaseLayer::processLayer(deLayerProcessType type, int channel)
{
    logInfo("processLayer channel: " + str(channel));
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
    logInfo("processLayer DONE");
}

bool deBaseLayer::processFull()
{
    return updateImage();
}

const deImage& deBaseLayer::getLayerImage() const
{
    return mainLayerImage;
}

void deBaseLayer::updateChannelUsage(std::map<int, int>& channelUsage, int layerIndex) const
{
    mainLayerImage.updateChannelUsage(channelUsage, layerIndex);
}

void updateMainImageAllChannels(deBaseLayer& layer)
{
    logInfo("update action all channels start");

    int n = getColorSpaceSize(layer.getColorSpace());
    int i;

    deSemaphore semaphore(0, n);

    for (i = 0; i < n; i++)
    {
        logInfo("creating update action thread for channel " + str(i));
        deUpdateActionThread* thread = new deUpdateActionThread(layer, i, semaphore);

        if ( thread->Create() != wxTHREAD_NO_ERROR )
        {
        }

        if ( thread->Run() != wxTHREAD_NO_ERROR )
        {
        }
    }

    for (i = 0; i < n; i++)
    {
        logInfo("waiting for update action thread for channel " + str(i));
        semaphore.wait();
    }

    logInfo("update action all channels DONE");
}

void deBaseLayer::processSingleChannel(int channel)
{
    processMainImageSingleChannel(channel);
}    

bool deBaseLayer::processMainImageSingleChannel(int channel)
{
    bool result = true;
    logInfo("base layer process main image single channel");
    if (setChannelInMainImage(channel))
    {
        logInfo("base layer process main image single channel after set");
        result = updateMainImageSingleChannel(channel);
    }
    logInfo("base layer process main image single channel DONE");
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

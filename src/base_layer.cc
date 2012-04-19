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
#include "xml.h"
#include "color_space_utils.h"
#include "logger.h"
#include "semaphore.h"
#include "str.h"

class deUpdateActionThread:public wxThread
{
    private:
        virtual void *Entry()
        {
            bool result = layer.updateMainImageSingleChannel(channel);
            if (!result)
            {
                logError("update action failed");
                layer.setErrorOnUpdateFromThread();
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
 channelManager(_channelManager),
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

void deBaseLayer::saveCommon(xmlNodePtr node)
{
    saveChild(node, "type", getType());
    //saveChild(node, "index", str(index));
    //saveChild(node, "source", str(sourceLayer));
    saveChild(node, "color_space", getColorSpaceName(colorSpace));
}

void deBaseLayer::lockLayer()
{
    lockWithLog(mutex, "layer mutex");
}

void deBaseLayer::unlockLayer()
{
    mutex.unlock();
}

void deBaseLayer::processLayer(deLayerProcessType type, int channel)
{
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

void deBaseLayer::setErrorOnUpdateFromThread()
{
    errorOnUpdate = true;
}

bool deBaseLayer::updateMainImageAllChannels()
{
    logMessage("update action all channels start");

    int n = getColorSpaceSize(colorSpace);
    int i;

    deSemaphore semaphore(0, n);

    errorOnUpdate = false;

    for (i = 0; i < n; i++)
    {
        logMessage("creating update action thread for channel " + str(i));
        deUpdateActionThread* thread = new deUpdateActionThread(*this, i, semaphore);

        if ( thread->Create() != wxTHREAD_NO_ERROR )
        {
        }

        if ( thread->Run() != wxTHREAD_NO_ERROR )
        {
        }
    }

    for (i = 0; i < n; i++)
    {
        logMessage("waiting for update action thread for channel " + str(i));
        semaphore.wait();
    }

    logMessage("update action all channels end");

    if (errorOnUpdate)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void deBaseLayer::processSingleChannel(int channel)
{
    updateMainImageSingleChannel(channel);
}    

bool deBaseLayer::updateImage()
{
    bool result = updateMainImageNotThreadedWay();
    if (!result)
    {
        result = updateMainImageAllChannels();
    }

    return result;
}


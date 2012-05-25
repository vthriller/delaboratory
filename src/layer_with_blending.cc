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

#include "layer_with_blending.h"
#include "channel_manager.h"
#include "blend_channel.h"
#include <wx/thread.h>
#include "color_space_utils.h"
#include <iostream>
#include "str.h"
#include "property_numeric.h"
#include "logger.h"

class deUpdateBlendThread:public wxThread
{
    private:
        virtual void *Entry()
        {
            layer.updateBlend(channel);
            semaphore.post();
            return NULL;
        }
        deLayerWithBlending& layer;
        int channel;
        deSemaphore& semaphore;
    public:    
        deUpdateBlendThread(deLayerWithBlending& _layer, int _channel, deSemaphore& _semaphore)
        :layer(_layer),
         channel(_channel),
         semaphore(_semaphore)
        {
        }
        virtual ~deUpdateBlendThread()
        {
        }
};


deLayerWithBlending::deLayerWithBlending(deColorSpace _colorSpace, deChannelManager& _channelManager, int _sourceLayerIndex, deLayerStack& _layerStack)
:deSwitchableLayer(_colorSpace, _channelManager, _sourceLayerIndex, _layerStack),
 imageBlendPass(_colorSpace, _channelManager)
{
    createPropertyNumeric("opacity", 0, 1);
    createPropertyChoice("blend mode", getSupportedBlendModeNames());

    dePropertyChoice* blendMode = getPropertyChoice("blend mode");
    blendMode->setSizer("channels");
    blendMode->setBlendOnly();

    setBlendMode(deBlendNormal);

    deProperty* p = getPropertyNumeric("opacity");
    p->setBlendOnly();

    setOpacity(1.0);
}

deLayerWithBlending::~deLayerWithBlending()
{
}

deValue deLayerWithBlending::getOpacity() const
{
    return getNumericValue("opacity");
}

void deLayerWithBlending::setOpacity(deValue _opacity)
{
    dePropertyNumeric* p = getPropertyNumeric("opacity");
    p->set(_opacity);
}

bool deLayerWithBlending::isBlendingEnabled() const
{
    if (getNumericValue("opacity") < 1.0)
    {
        return true;
    }

    if (getBlendMode() != deBlendNormal)
    {
        return true;
    }

    return false;
}

void deLayerWithBlending::setBlendMode(deBlendMode mode)
{
    dePropertyChoice* blendMode = getPropertyChoice("blend mode");
    if (blendMode)
    {
        blendMode->set(getBlendModeName(mode));
    }
}

bool deLayerWithBlending::updateBlend(int i)
{
    logInfo("update blend " + str(i));

    int channelSize = mainLayerImage.getChannelSize().getN();

    int s = getSourceImage().getChannelIndex(i);

    if (!isBlendingEnabled())
    {
        return true;
    }

    if (isChannelNeutral(i))
    {
        if (getBlendMode() == deBlendNormal)
        {
            imageBlendPass.disableChannel(i, s);
            return true;
        }
    }

    if (!isChannelEnabled(i))
    {
        imageBlendPass.disableChannel(i, s);
        return true;
    }

    deChannel* sourceChannel = channelManager.getChannel(s);
    if (!sourceChannel)
    {
        logError("update blend no channel s");
        return false;
    }

    int c = mainLayerImage.getChannelIndex(i);
    deChannel* channel = channelManager.getChannel(c);
    if (!channel)
    {
        logError("update blend no channel c");
        return false;
    }

    imageBlendPass.enableChannel(i);
    int b = imageBlendPass.getChannelIndex(i);
    deChannel* blendChannel_ = channelManager.getChannel(b);
    if (!blendChannel_)
    {
        logError("update blend no channel b");
        return false;
    }

    deValue* maskPixels = NULL;

    deValue* sourcePixels = sourceChannel->getPixels();
    deValue* overlayPixels = channel->getPixels();
    deValue* resultPixels = blendChannel_->getPixels();

    deValue o = getOpacity();
    blendChannel(sourcePixels, overlayPixels, resultPixels, maskPixels, getBlendMode(), o, channelSize);

    return true;

}

const deImage& deLayerWithBlending::getLayerImage() const
{
    if (isBlendingEnabled())
    {
        return imageBlendPass;
    }

    return mainLayerImage;
}

void deLayerWithBlending::updateChannelUsage(std::map<int, int>& channelUsage, int layerIndex) const
{
    getSourceImage().updateChannelUsage(channelUsage, layerIndex);

    mainLayerImage.updateChannelUsage(channelUsage, layerIndex);

    if (isBlendingEnabled())
    {
        imageBlendPass.updateChannelUsage(channelUsage, layerIndex);
    }
}

bool deLayerWithBlending::updateBlendAllChannels()
{
    int n = getColorSpaceSize(colorSpace);
    int i;

    deSemaphore semaphore(0, n);

    for (i = 0; i < n; i++)
    {
        deUpdateBlendThread* thread = new deUpdateBlendThread(*this, i, semaphore);

        if ( thread->Create() != wxTHREAD_NO_ERROR )
        {
        }

        if ( thread->Run() != wxTHREAD_NO_ERROR )
        {
        }
    }

    for (i = 0; i < n; i++)
    {
        semaphore.wait();
    }

    return true;

}

void deLayerWithBlending::processSingleChannel(int channel)
{
    deBaseLayer::processSingleChannel(channel);
    updateBlend(channel);
}    

bool deLayerWithBlending::updateImage()
{
    bool result = deBaseLayer::updateImage();

    if (result)
    {
        result = updateBlendAllChannels();
    }

    return result;
}

deBlendMode deLayerWithBlending::getBlendMode() const
{
    const dePropertyChoice* blendMode = getPropertyChoice("blend mode");
    std::string m = blendMode->get();
    deBlendMode mode = blendModeFromString(m);
    return mode;
}

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
#include "property_boolean.h"
#include "logger.h"
#include "blend_color_luminosity.h"

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
    createPropertyChoice("blend mode", getSupportedBlendModeNames(colorSpace));

    dePropertyChoice* blendMode = getPropertyChoice("blend mode");
    blendMode->setSizer("blend");
    blendMode->setBlendOnly();

    createPropertyBoolean("invert");
    getPropertyBoolean("invert")->setSizer("blend");

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
    bool inverted = getPropertyBoolean("invert")->get();
    if (inverted)
    {
        return true;
    }

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
    if (!isBlendingEnabled())
    {
        return true;
    }

    if (isChannelNeutral(i))
    {
        if (getBlendMode() == deBlendNormal)
        {
            int s = getSourceImage().getChannelIndex(i);
            imageBlendPass.disableChannel(i, s);
            return true;
        }
    }

    if (!isChannelEnabled(i))
    {
        int s = getSourceImage().getChannelIndex(i);
        imageBlendPass.disableChannel(i, s);
        return true;
    }

    logInfo("update blend " + str(i) + " START");

    const deValue* source = getSourceImage().startRead(i);

    const deValue* overlay = mainLayerImage.startRead(i);

    imageBlendPass.enableChannel(i);
    deValue* destination = imageBlendPass.startWrite(i);

    deValue* maskPixels = NULL;

    int channelSize = mainLayerImage.getChannelSize().getN();

    deValue o = getOpacity();
    bool inverted = getPropertyBoolean("invert")->get();

    if (inverted)
    {
        blendChannel(overlay, source, destination, maskPixels, getBlendMode(), o, channelSize);
    }        
    else
    {
        blendChannel(source, overlay, destination, maskPixels, getBlendMode(), o, channelSize);
    }        

    getSourceImage().finishRead(i);
    mainLayerImage.finishRead(i);
    imageBlendPass.finishWrite(i);

    logInfo("update blend " + str(i) + " DONE");

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
    if (tryBlendSpecial())
    {
        return true;
    }

    logInfo("update blend all channels START");

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
    logInfo("update blend all channels DONE");

    return true;

}

void deLayerWithBlending::processSingleChannel(int channel)
{
    logInfo("layer with blending processs single channel " + str(channel));
    deBaseLayer::processSingleChannel(channel);
    if (!tryBlendSpecial())
    {
        updateBlend(channel);
    }        
    logInfo("layer with blending processs single channel DONE");
}    

bool deLayerWithBlending::updateImage()
{
    logInfo("layer with blending update image");

    bool result = deBaseLayer::updateImage();

    if (result)
    {
        result = updateBlendAllChannels();
    }

    logInfo("layer with blending update image DONE");

    return result;
}

deBlendMode deLayerWithBlending::getBlendMode() const
{
    const dePropertyChoice* blendMode = getPropertyChoice("blend mode");
    std::string m = blendMode->get();
    deBlendMode mode = blendModeFromString(m);
    return mode;
}

void deLayerWithBlending::blendSpecial()
{
    logInfo("blend special START");

    deBlendMode mode = getBlendMode();
    int cs = getColorSpaceSize(colorSpace);

    const deValue* source0 = getSourceImage().startRead(0);
    const deValue* source1 = getSourceImage().startRead(1);
    const deValue* source2 = getSourceImage().startRead(2);
    const deValue* source3 = NULL;
    if (cs == 4)
    {
        source3 = getSourceImage().startRead(3);
    }        

    const deValue* overlay0 = mainLayerImage.startRead(0);
    const deValue* overlay1 = mainLayerImage.startRead(1);
    const deValue* overlay2 = mainLayerImage.startRead(2);
    const deValue* overlay3 = NULL;
    if (cs == 4)
    {
        overlay3 = mainLayerImage.startRead(3);
    }        

    imageBlendPass.enableChannel(0);
    imageBlendPass.enableChannel(1);
    imageBlendPass.enableChannel(2);
    if (cs == 4)
    {
        imageBlendPass.enableChannel(3);
    }        

    deValue* destination0 = imageBlendPass.startWrite(0);
    deValue* destination1 = imageBlendPass.startWrite(1);
    deValue* destination2 = imageBlendPass.startWrite(2);
    deValue* destination3 = NULL;
    if (cs == 4)
    {
        destination3 = imageBlendPass.startWrite(3);
    }        

    int n = mainLayerImage.getChannelSize().getN();

    deValue o = getOpacity();

    if (mode == deBlendColor)
    {
        if (colorSpace == deColorSpaceRGB)
        {
            blendColorRGB(source0, source1, source2, overlay0, overlay1, overlay2, destination0, destination1, destination2, n, o);
        }
        if (colorSpace == deColorSpaceProPhoto)
        {
            blendColorProPhoto(source0, source1, source2, overlay0, overlay1, overlay2, destination0, destination1, destination2, n, o);
        }
        if (colorSpace == deColorSpaceCMYK)
        {
            blendColorCMYK(source0, source1, source2, source3, overlay0, overlay1, overlay2, overlay3, destination0, destination1, destination2, destination3, n, o);
        }
    }

    if (mode == deBlendLuminosity)
    {
        if (colorSpace == deColorSpaceRGB)
        {
            blendLuminosityRGB(source0, source1, source2, overlay0, overlay1, overlay2, destination0, destination1, destination2, n, o);
        }
        if (colorSpace == deColorSpaceProPhoto)
        {
            blendLuminosityProPhoto(source0, source1, source2, overlay0, overlay1, overlay2, destination0, destination1, destination2, n, o);
        }
        if (colorSpace == deColorSpaceCMYK)
        {
            blendLuminosityCMYK(source0, source1, source2, source3, overlay0, overlay1, overlay2, overlay3, destination0, destination1, destination2, destination3, n, o);
        }
    }

    getSourceImage().finishRead(0);
    getSourceImage().finishRead(1);
    getSourceImage().finishRead(2);
    mainLayerImage.finishRead(0);
    mainLayerImage.finishRead(1);
    mainLayerImage.finishRead(2);
    imageBlendPass.finishWrite(0);
    imageBlendPass.finishWrite(1);
    imageBlendPass.finishWrite(2);
    if (cs == 4)
    {
        getSourceImage().finishRead(3);
        mainLayerImage.finishRead(3);
        imageBlendPass.finishWrite(3);
    }
    logInfo("blend special DONE");
}

bool deLayerWithBlending::tryBlendSpecial()
{
    deBlendMode mode = getBlendMode();
    if (mode == deBlendColor)
    {
        blendSpecial();
        return true;
    }
    if (mode == deBlendLuminosity)
    {
        blendSpecial();
        return true;
    }
    return false;
}

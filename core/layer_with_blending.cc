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
#include "color_space_utils.h"
#include <iostream>
#include "str.h"
#include "property_numeric.h"
#include "property_boolean.h"
#include "logger.h"
#include "blend_color_luminosity.h"
#include "update_blend.h"


deLayerWithBlending::deLayerWithBlending(deColorSpace _colorSpace, deChannelManager& _channelManager, int _sourceLayerIndex, deLayerStack& _layerStack)
:deSwitchableLayer(_colorSpace, _channelManager, _sourceLayerIndex, _layerStack),
 imageBlendPass(_colorSpace, _channelManager)
{
    createPropertyNumeric("opacity", 0, 1);
    createPropertyChoice("blend mode", getSupportedBlendModeNames(colorSpace));

    dePropertyChoice* blendMode = getPropertyChoice("blend mode");
    if (blendMode)
    {
        blendMode->setSizer("blend");
        blendMode->setBlendOnly();
    }

    createPropertyBoolean("invert");
    getPropertyBoolean("invert")->setSizer("blend");

    setBlendMode(deBlendNormal);

    deProperty* p = getPropertyNumeric("opacity");
    if (p)
    {
        p->setBlendOnly();
    }        

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
    if (p)
    {
        p->set(_opacity);
    }        
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
#ifdef DEBUG_LOG
    logInfo("update blend " + str(i) + " START");
#endif    

    const deValue* source = getSourceImage().startRead(i);

    const deValue* overlay = mainLayerImage.startRead(i);

    deValue* destination = imageBlendPass.startWrite(i);

    deValue* maskPixels = NULL;

    int channelSize = mainLayerImage.getChannelSize().getN();

    deValue o = getOpacity();
    if (!isChannelEnabled(i))
    {
        o = 0.0;
    }

    dePropertyBoolean* pinv = getPropertyBoolean("invert");
    bool inverted = false;
    if (pinv)
    {
        inverted = pinv->get();
    }        

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

#ifdef DEBUG_LOG
    logInfo("update blend " + str(i) + " DONE");
#endif    

    return true;

}

const deImage& deLayerWithBlending::getLayerImage() const
{
    return imageBlendPass;
}

void deLayerWithBlending::updateChannelUsage(std::map<int, int>& channelUsage, int layerIndex) const
{
    getSourceImage().updateChannelUsage(channelUsage, layerIndex);

    mainLayerImage.updateChannelUsage(channelUsage, layerIndex);

    imageBlendPass.updateChannelUsage(channelUsage, layerIndex);
}

bool deLayerWithBlending::updateBlendAllChannels()
{
    if (tryBlendSpecial())
    {
        return true;
    }

    updateBlendOnThread(*this);

    return true;

}

void deLayerWithBlending::processSingleChannel(int channel)
{
#ifdef DEBUG_LOG
    logInfo("layer with blending processs single channel " + str(channel));
#endif    
    deBaseLayer::processSingleChannel(channel);
    if (!tryBlendSpecial())
    {
        updateBlend(channel);
    }        
#ifdef DEBUG_LOG
    logInfo("layer with blending processs single channel DONE");
#endif    
}    

bool deLayerWithBlending::updateImage()
{
#ifdef DEBUG_LOG
    logInfo("layer with blending update image");
#endif

    bool result = deBaseLayer::updateImage();

    if (result)
    {
        result = updateBlendAllChannels();
    }

#ifdef DEBUG_LOG
    logInfo("layer with blending update image DONE");
#endif    

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
#ifdef DEBUG_LOG
    logInfo("blend special START");
#endif    

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
#ifdef DEBUG_LOG
    logInfo("blend special DONE");
#endif    
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

void deLayerWithBlending::allocateChannels()
{
    deBaseLayer::allocateChannels();
    imageBlendPass.allocateChannels();
}

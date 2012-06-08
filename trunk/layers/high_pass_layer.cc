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

#include "high_pass_layer.h"
#include "preset.h"
#include "view_manager.h"
#include "fill_channel.h"
#include "color_space_utils.h"
#include "blur.h"
#include "blend_channel.h"
#include "logger.h"

deHighPassLayer::deHighPassLayer(deColorSpace _colorSpace, deChannelManager& _channelManager, int _sourceLayer, deLayerStack& _layerStack, deViewManager& _viewManager)
:deLayerWithBlending(_colorSpace, _channelManager, _sourceLayer, _layerStack), viewManager(_viewManager)
{
    dePreset* reset = createPreset("reset");

    createPropertyNumeric("radius", 1, 20);
    reset->addNumericValue("radius", 4);

    createPropertyNumeric("power", 1.0, 20.0);
    reset->addNumericValue("power", 1.0);

    applyPreset("reset");

}

deHighPassLayer::~deHighPassLayer()
{
}

bool deHighPassLayer::isChannelNeutral(int channel)
{   
    return false;
}

bool deHighPassLayer::updateMainImageNotThreadedWay()
{
    deSize size = mainLayerImage.getChannelSize();
    int n = size.getN();

    deValue* mask = NULL;
    try
    {
        mask = new deValue [n];
    }
    catch (std::bad_alloc)
    {
        logError("allocating memory in USM");
        if (mask)
        {
            delete [] mask;
        }
        return false;
    }

    int channel;

    int nc = getColorSpaceSize(colorSpace);
    int no = 3; // assume original image has 3 channels

    // clear destination
    fillChannel(mask, n, 0.0);

    deValue t = 0.0;
    deBlurType type = deGaussianBlur;
    deValue r = getNumericValue("radius") * viewManager.getRealScale();

    int i;
    for (channel = 0; channel < nc; channel++)
    {
        mainLayerImage.enableChannel(channel);
    }

    // calc high pass
    for (channel = 0; channel < no; channel++)
    {
        const deValue* source = getOriginalImage().startRead(channel);
        deValue* destination = mainLayerImage.startWrite(0);    // use 0 channel for blur
        blurChannel(source, destination, size, r, r, type, t);
        for (i = 0; i < n; i++)
        {
            deValue d = source[i] - destination[i];
            mask[i] += d;
        }
        getOriginalImage().finishRead(channel);
        mainLayerImage.finishWrite(0);
    }

    deValue p = getNumericValue("power");

    for (i = 0; i < n; i++)
    {
        deValue d = mask[i];

        d = 0.5 + p * d;

        if (d < 0)
        {
            d = 0;
        } 
        else if (d > 1)
        {
            d = 1;
        }
        
        mask[i] = d;
    }

    for (channel = 0; channel < nc; channel++)
    {
        if (!isChannelEnabled(channel))
        {
            int s = getSourceImage().getChannelIndex(channel);
            mainLayerImage.disableChannel(channel, s);
        }
        else
        {
            deValue* destination = mainLayerImage.startWrite(channel);
            int i;
            for (i = 0; i < n; i++)
            {
                destination[i] = mask[i];
            }
            mainLayerImage.finishWrite(channel);
        }
    }

    delete [] mask;

    return true;                
}            

void deHighPassLayer::updateChannelUsage(std::map<int, int>& channelUsage, int layerIndex) const
{
    deLayerWithBlending::updateChannelUsage(channelUsage, layerIndex);

    getOriginalImage().updateChannelUsage(channelUsage, layerIndex);
}


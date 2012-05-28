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

#include "c2g_layer.h"
#include "preset.h"
#include "view_manager.h"
#include "fill_channel.h"
#include "color_space_utils.h"
#include "blend_channel.h"
#include "logger.h"
#include "c2g.h"

deC2GLayer::deC2GLayer(deColorSpace _colorSpace, deChannelManager& _channelManager, int _sourceLayer, deLayerStack& _layerStack, deViewManager& _viewManager)
:deLayerWithBlending(_colorSpace, _channelManager, _sourceLayer, _layerStack), viewManager(_viewManager)
{
    dePreset* reset = createPreset("reset");

    createPropertyNumeric("radius", 1, 1000);
    reset->addNumericValue("radius", 300);

    createPropertyNumeric("samples", 1, 50);
    reset->addNumericValue("samples", 10);

    applyPreset("reset");

}

deC2GLayer::~deC2GLayer()
{
}

bool deC2GLayer::updateMainImageNotThreadedWay()
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
        logError("allocating memory in c2g");
        if (mask)
        {
            delete [] mask;
        }
        return false;
    }


    // clear destination
    fillChannel(mask, n, 0.0);

    deValue r = getNumericValue("radius") * viewManager.getRealScale();
    deValue samples = getNumericValue("samples");

    const deValue* source0 = getOriginalImage().startRead(0);
    const deValue* source1 = getOriginalImage().startRead(1);
    const deValue* source2 = getOriginalImage().startRead(2);

    c2g(source0, source1, source2, mask, size, r, samples);

    getOriginalImage().finishRead(0);
    getOriginalImage().finishRead(1);
    getOriginalImage().finishRead(2);

    int nc = getColorSpaceSize(colorSpace);
    int channel;
    for (channel = 0; channel < nc; channel++)
    {
        if (!isChannelEnabled(channel))
        {
            int s = getSourceImage().getChannelIndex(channel);
            mainLayerImage.disableChannel(channel, s);
        }
        else
        {
            mainLayerImage.enableChannel(channel);
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

void deC2GLayer::updateChannelUsage(std::map<int, int>& channelUsage, int layerIndex) const
{
    deLayerWithBlending::updateChannelUsage(channelUsage, layerIndex);

    getOriginalImage().updateChannelUsage(channelUsage, layerIndex);
}


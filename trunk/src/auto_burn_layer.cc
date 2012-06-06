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

#include "auto_burn_layer.h"
#include "usm.h"
#include "preset.h"
#include "view_manager.h"
#include "logger.h"

deAutoBurnLayer::deAutoBurnLayer(deColorSpace _colorSpace, deChannelManager& _channelManager, int _sourceLayer, deLayerStack& _layerStack, deViewManager& _viewManager)
:deLayerWithBlending(_colorSpace, _channelManager, _sourceLayer, _layerStack), viewManager(_viewManager)
{
    dePreset* reset = createPreset("reset");
    createPropertyNumeric("radius 1", 10, 100);
    reset->addNumericValue("radius 1", 20);
    createPropertyNumeric("radius 2", 5, 50);
    reset->addNumericValue("radius 2", 10);
    createPropertyNumeric("threshold", 0.0, 1.0);
    reset->addNumericValue("threshold", 0.5);
    applyPreset("reset");
    setOpacity(0.5);
    disableNotForSharpen();
}

deAutoBurnLayer::~deAutoBurnLayer()
{
}

bool deAutoBurnLayer::updateMainImageSingleChannel(int channel)
{
/*
    if ((isChannelNeutral(channel)) || (!isChannelEnabled(channel)))
    {
        int s = getSourceImage().getChannelIndex(channel);
        mainLayerImage.disableChannel(channel, s);
        return true;
    }
    */

    deValue r1 = getNumericValue("radius 1") * viewManager.getRealScale();;
    deValue r2 = getNumericValue("radius 2") * viewManager.getRealScale();;
    deValue t = getNumericValue("threshold");

    deSize size = mainLayerImage.getChannelSize();

//    mainLayerImage.enableChannel(channel);
    const deValue* source = getSourceImage().startRead(channel);
    deValue* destination = mainLayerImage.startWrite(channel);
    
    bool result = false;
    
    if ((source) && (destination))
    {
        result = autoDodgeBurn(source, destination, size, r1, r2, t, true);
    }        

    getSourceImage().finishRead(channel);
    mainLayerImage.finishWrite(channel);

    return result;
}


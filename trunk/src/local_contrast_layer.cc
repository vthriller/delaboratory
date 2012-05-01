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

#include "local_contrast_layer.h"
#include "usm.h"
#include "preset.h"
#include "view_manager.h"

deLocalContrastLayer::deLocalContrastLayer(deColorSpace _colorSpace, deChannelManager& _channelManager, int _sourceLayer, deLayerStack& _layerStack, deViewManager& _viewManager)
:deLayerWithBlending(_colorSpace, _channelManager, _sourceLayer, _layerStack), viewManager(_viewManager)
{
    dePreset* reset = createPreset("reset");
    createPropertyNumeric("radius", 10, 500);
    reset->addNumericValue("radius", 200);
    applyPreset("reset");
    setOpacity(0.2);
    disableNotForSharpen();
}

deLocalContrastLayer::~deLocalContrastLayer()
{
}

bool deLocalContrastLayer::isChannelNeutral(int channel)
{   
    return false;
}

bool deLocalContrastLayer::updateMainImageSingleChannel(int channel)
{
    if ((isChannelNeutral(channel)) || (!isChannelEnabled(channel)))
    {
        int s = getSourceImage().getChannelIndex(channel);
        mainLayerImage.disableChannel(channel, s);
        return true;
    }

    deValue r = getNumericValue("radius") * viewManager.getRealScale();;
    deValue a = 2.0;
    deValue t = 0.0;

    deSize size = mainLayerImage.getChannelSize();

    mainLayerImage.enableChannel(channel);
    const deValue* source = getSourceImage().getValues(channel);
    deValue* destination = mainLayerImage.getValues(channel);
    
    bool result = false;
    
    if ((source) && (destination))
    {
        result = unsharpMask(source, destination, size, a, r, t, deBoxBlur);
    }        

    return result;
}


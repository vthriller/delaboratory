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

#include "gaussian_blur_layer.h"
#include "blur.h"
#include "preset.h"
#include "view_manager.h"

deGaussianBlurLayer::deGaussianBlurLayer(deColorSpace _colorSpace, deChannelManager& _channelManager, int _sourceLayer, deLayerStack& _layerStack, deViewManager& _viewManager)
:deLayerWithBlending(_colorSpace, _channelManager, _sourceLayer, _layerStack), viewManager(_viewManager)
{
    dePreset* reset = createPreset("reset");
    createPropertyNumeric("radius", 2, 600);
    reset->addNumericValue("radius", 200);
    applyPreset("reset");
}

deGaussianBlurLayer::~deGaussianBlurLayer()
{
}

bool deGaussianBlurLayer::updateMainImageSingleChannel(int channel)
{
    if ((isChannelNeutral(channel)) || (!isChannelEnabled(channel)))
    {
        int s = getSourceImage().getChannelIndex(channel);
        mainLayerImage.disableChannel(channel, s);
        return true;
    }

    deValue r = getNumericValue("radius") * viewManager.getRealScale();;
    
    deSize size = mainLayerImage.getChannelSize();

    mainLayerImage.enableChannel(channel);
    const deValue* source = getSourceImage().startRead(channel);
    deValue* destination = mainLayerImage.startWrite(channel);
    
    bool result = false;
    deBlurType type = deGaussianBlur;

    result = blurChannel(source, destination, size, r, r, type, 0.0);

    getSourceImage().finishRead(channel);
    mainLayerImage.finishWrite(channel);
    
    return result;
}


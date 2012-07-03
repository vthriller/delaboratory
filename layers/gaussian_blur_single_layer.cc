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

#include "gaussian_blur_single_layer.h"
#include "blur.h"
#include "preset.h"
#include "view_manager.h"
#include "color_space_utils.h"
#include "copy_channel.h"

deGaussianBlurSingleLayer::deGaussianBlurSingleLayer(deColorSpace _colorSpace, deChannelManager& _channelManager, int _sourceLayer, deLayerStack& _layerStack, deViewManager& _viewManager)
:deLayerWithBlending(_colorSpace, _channelManager, _sourceLayer, _layerStack), viewManager(_viewManager)
{
    dePreset* reset = createPreset("reset");
    createPropertyNumeric("radius", 2, 600);
    reset->addNumericValue("radius", 200);

    createPropertyChoice("channel", getChannelNames(colorSpace));

    applyPreset("reset");
}

deGaussianBlurSingleLayer::~deGaussianBlurSingleLayer()
{
}
bool deGaussianBlurSingleLayer::updateMainImageNotThreadedWay()
{
    deSize size = mainLayerImage.getChannelSize();
    int n = size.getN();

    int nc = getColorSpaceSize(colorSpace);

    deBlurType type = deGaussianBlur;
    deValue r = getNumericValue("radius") * viewManager.getRealScale();

    int channel = getPropertyChoice("channel")->getIndex();

    const deValue* source = getSourceImage().startRead(channel);

    int i;

    deValue* destination = mainLayerImage.startWrite(0);

    blurChannel(source, destination, size, r, r, type, 0.0);

    getSourceImage().finishRead(channel);

    for (i = 1; i < nc; i++)
    {
        deValue* dd = mainLayerImage.startWrite(i);
        copyChannel(destination, dd, n);
        mainLayerImage.finishWrite(i);
    }

    mainLayerImage.finishWrite(0);


    return true;
}    


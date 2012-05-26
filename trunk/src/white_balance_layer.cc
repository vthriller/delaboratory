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

#include "white_balance_layer.h"
#include "preset.h"
#include "view_manager.h"
#include "color_space_utils.h"
#include "copy_channel.h"
#include "logger.h"
#include "str.h"
#include "curve.h"
#include "property_numeric.h"

deWhiteBalanceLayer::deWhiteBalanceLayer(deColorSpace _colorSpace, deChannelManager& _channelManager, int _sourceLayer, deLayerStack& _layerStack)
:deLayerWithBlending(_colorSpace, _channelManager, _sourceLayer, _layerStack)
{
    dePreset* reset = createPreset("reset");

    createPropertyNumeric("green / magenta", -1, 1);
    reset->addNumericValue("green / magenta", 0.0);

    createPropertyNumeric("green / magenta finetune", -1, 1);
    reset->addNumericValue("green / magenta finetune", 0.0);

    createPropertyNumeric("blue / yellow", -1, 1);
    reset->addNumericValue("blue / yellow", 0.0);

    createPropertyNumeric("blue / yellow finetune", -1, 1);
    reset->addNumericValue("blue / yellow finetune", 0.0);

    applyPreset("reset");
    if (colorSpace != deColorSpaceLAB)
    {
        disableAll();
    }
    else
    {
        disableChannel(0);
    }
}

deWhiteBalanceLayer::~deWhiteBalanceLayer()
{
}

bool deWhiteBalanceLayer::updateMainImageSingleChannel(int channel)
{
    bool e = true;

    if (colorSpace != deColorSpaceLAB)
    {
        e = false;
    }

    if (channel == 0)
    {
        e = false;
    }

    if ((isChannelNeutral(channel)) || (!isChannelEnabled(channel)))
    {
        e = false;
    }

    deValue s = 0.0;
    deValue s2 = 0.0;
    if (channel == 1)
    {
        s = getNumericValue("green / magenta");
        s2 = getNumericValue("green / magenta finetune");
    }
    if (channel == 2)
    {
        s = getNumericValue("blue / yellow");
        s2 = getNumericValue("blue / yellow finetune");
    }

    s = 0.5 * s + 0.1 * s2;

    if (s == 0.0)
    {
        e = false;
    }

    if (!e)
    {
        int s = getSourceImage().getChannelIndex(channel);
        mainLayerImage.disableChannel(channel, s);
        return true;
    }

    if (s > 0.49)
    {
        s = 0.49;
    }
    if (s < -0.49)
    {
        s = -0.49;
    }

    const deValue* source = getSourceImage().startRead(channel);
    mainLayerImage.enableChannel(channel);
    deValue* target = mainLayerImage.startWrite(channel);
    int n = mainLayerImage.getChannelSize().getN();

    deBaseCurve curve;

    curve.addPoint(0, 0);
    curve.addPoint(0.5 - s , 0.5);
    curve.addPoint(1, 1);

    curve.build();

    curve.process(source, target, n);

    getSourceImage().finishRead(channel);
    mainLayerImage.finishWrite(channel);

    return true;
}

bool deWhiteBalanceLayer::onImageClick(deValue x, deValue y)
{
    if (colorSpace != deColorSpaceLAB)
    {
        return false;
    }

    const deImage& source = getSourceImage();

    const deSize size = source.getChannelSize();

    int p = (y * size.getH() )  * size.getW() + (x * size.getW());

    const deValue* pixelsA = source.startRead(1);
    const deValue* pixelsB = source.startRead(2);

    deValue a = pixelsA[p] - 0.5;
    deValue b = pixelsB[p] - 0.5;

    source.finishRead(1);
    source.finishRead(2);

    a = -2 * a;
    b = -2 * b;

    getPropertyNumeric("green / magenta")->set(a);
    getPropertyNumeric("green / magenta finetune")->set(0.0);
    getPropertyNumeric("blue / yellow")->set(b);
    getPropertyNumeric("blue / yellow finetune")->set(0.0);

    return true;
}

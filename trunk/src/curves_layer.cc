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

#include "curves_layer.h"
#include "project.h"
#include <iostream>
#include <cassert>

#include "str.h"

#include "color_space_utils.h"
#include "channel_manager.h"

#include "property_curves.h"

#include "preset.h"

deCurvesLayer::deCurvesLayer(deColorSpace _colorSpace, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager)
:deLayerWithBlending(_colorSpace, _channelManager, _sourceLayer, _layerStack)
{
    dePreset* reset = createPreset("reset");
    reset->addOperation("reset");

    int n = getColorSpaceSize(colorSpace);
    properties.push_back(new dePropertyCurves("curves", n));

    applyPreset("reset");
}

deCurvesLayer::~deCurvesLayer()
{
}

bool deCurvesLayer::updateMainImageSingleChannel(int i)
{
    const deImage& sourceImage = getSourceImage();

    int s = sourceImage.getChannelIndex(i);

    if ((isChannelNeutral(i)) || (!isChannelEnabled(i)))
    {
        mainLayerImage.disableChannel(i, s);
        return true;
    }

    dePropertyCurves* p = getPropertyCurves();
    if (!p)
    {
        return false;
    }
    const deBaseCurve* curve = p->getCurve(i);

    mainLayerImage.enableChannel(i);
    const deValue* source = getSourceImage().startRead(i);
    deValue* destination = mainLayerImage.startWrite(i);
    curve->process(source, destination, mainLayerImage.getChannelSize().getN());
    getSourceImage().finishRead(i);
    mainLayerImage.finishWrite(i);

    return true;

}

bool deCurvesLayer::isChannelNeutral(int index)
{
    dePropertyCurves* p = getPropertyCurves();
    if (!p)
    {
        return false;
    }
    deBaseCurve* curve = p->getCurve(index);
    return curve->isNeutral();
}    

dePropertyCurves* deCurvesLayer::getPropertyCurves()
{
    deProperty* p = getProperty("curves");
    return dynamic_cast<dePropertyCurves*>(p);
}

void deCurvesLayer::executeOperation(const std::string& operation)
{
    int n = getColorSpaceSize(colorSpace);
    dePropertyCurves* p = getPropertyCurves();

    int i;
    for (i = 0; i < n; i++)
    {
        deBaseCurve* curve = p->getCurve(i);

        curve->clearPoints();

        curve->addPoint(0, 0);
        curve->addPoint(1, 1);

        curve->build();

    }
}

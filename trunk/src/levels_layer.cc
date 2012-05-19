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

#include "levels_layer.h"
#include "copy_channel.h"
#include "color_space_utils.h"
#include "property_levels.h"
#include "curve.h"
#include "preset.h"
#include "histogram.h"
#include "logger.h"

deLevelsLayer::deLevelsLayer(deColorSpace _colorSpace, deChannelManager& _channelManager, int _sourceLayer, deLayerStack& _layerStack)
:deLayerWithBlending(_colorSpace, _channelManager, _sourceLayer, _layerStack)
{
    dePreset* reset = createPreset("reset");
    reset->addOperation("reset");

    dePreset* autoLevels = createPreset("auto levels");
    autoLevels->addOperation("auto levels");

    dePreset* autoLevelsLight = createPreset("auto levels light");
    autoLevelsLight->addOperation("auto levels light");

    dePreset* autoLevelsHeavy = createPreset("auto levels heavy");
    autoLevelsHeavy->addOperation("auto levels heavy");

    int n = getColorSpaceSize(colorSpace);

    properties.push_back(new dePropertyLevels("levels", n));

}

deLevelsLayer::~deLevelsLayer()
{
}

bool deLevelsLayer::isChannelNeutral(int channel)
{
    dePropertyLevels* propertyLevels = getPropertyLevels();
    if (!propertyLevels)
    {
        return true;
    }

    const deLevels& levels = propertyLevels->getLevels(channel);
    return levels.isNeutral();

}

bool deLevelsLayer::updateMainImageSingleChannel(int channel)
{
    if ((isChannelNeutral(channel)) || (!isChannelEnabled(channel)))
    {
        int s = getSourceImage().getChannelIndex(channel);
        mainLayerImage.disableChannel(channel, s);
        return true;
    }

    dePropertyLevels* propertyLevels = getPropertyLevels();
    if (!propertyLevels)
    {
        return false;
    }


    const deLevels& levels = propertyLevels->getLevels(channel);

    const deValue* source = getSourceImage().startRead(channel);
    mainLayerImage.enableChannel(channel);
    deValue* target = mainLayerImage.startWrite(channel);
    int n = mainLayerImage.getChannelSize().getN();

    deBaseCurve curve;

    curve.addPoint(0, 0);
    curve.addPoint(levels.getMin(), 0);
    curve.addPoint(levels.getMiddle(), 0.5);
    curve.addPoint(levels.getMax(), 1);
    curve.addPoint(1, 1);

    curve.build();

    curve.process(source, target, n);

    getSourceImage().finishRead(channel);
    mainLayerImage.finishWrite(channel);

    return true;
}

dePropertyLevels* deLevelsLayer::getPropertyLevels()
{
    deProperty* p = getProperty("levels");
    return dynamic_cast<dePropertyLevels*>(p);
}

void deLevelsLayer::executeOperation(const std::string& operation)
{
    int n = getColorSpaceSize(colorSpace);

    dePropertyLevels* propertyLevels = getPropertyLevels();
    if (!propertyLevels)
    {
        return;
    }

    int i;
    for (i = 0; i < n; i++)
    {
        deLevels& levels = propertyLevels->getLevels(i);

        deValue min = 0.0;
        deValue middle = 0.5;
        deValue max = 1.0;

        if (operation == "auto levels")
        {
            calcAutoLevels(i, min, middle, max, 0.0005, 0.4);
        }

        if (operation == "auto levels light")
        {
            calcAutoLevels(i, min, middle, max, 0.005, 0.4);
        }

        if (operation == "auto levels heavy")
        {
            calcAutoLevels(i, min, middle, max, 0.02, 0.4);
        }

        levels.setMin(min);
        levels.setMiddle(middle);
        levels.setMax(max);

        if (!shouldUseAutoLevels(colorSpace, i))
        {
            disableChannel(i);
        }
    }

    setOpacity(0.7);
    
}

void deLevelsLayer::calcAutoLevels(int channel, deValue& min, deValue& middle, deValue& max, deValue margin1, deValue margin2)
{
    const deValue* c = getSourceImage().startRead(channel);
    int n = getSourceImage().getChannelSize().getN();

    int s = 1024;
    deHistogram histogram(s);

    histogram.clear();
    histogram.calc(c, n);

    getSourceImage().finishRead(channel);

    histogram.calcLevels(margin1, margin2, min, middle, max);
}

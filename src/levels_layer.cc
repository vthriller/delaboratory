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

    int i;
    for (i = 0; i < n; i++)
    {
        createPropertyLevels(i);
    }
}

deLevelsLayer::~deLevelsLayer()
{
}

bool deLevelsLayer::isChannelNeutral(int channel)
{
    dePropertyLevels* propertyLevels = getPropertyLevels(channel);
    if (!propertyLevels)
    {
        return true;
    }

    const deLevels& levels = propertyLevels->getLevels();
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

    dePropertyLevels* propertyLevels = getPropertyLevels(channel);
    if (!propertyLevels)
    {
        return false;
    }

    const deLevels& levels = propertyLevels->getLevels();

    const deValue* source = getSourceImage().getValues(channel);
    mainLayerImage.enableChannel(channel);
    deValue* target = mainLayerImage.getValues(channel);
    int n = mainLayerImage.getChannelSize().getN();

    deBaseCurve curve;

    curve.addPoint(0, 0);
    curve.addPoint(levels.getMin(), 0);
    curve.addPoint(levels.getMiddle(), 0.5);
    curve.addPoint(levels.getMax(), 1);
    curve.addPoint(1, 1);

    curve.build();

    curve.process(source, target, n);

    return true;
}

void deLevelsLayer::createPropertyLevels(int channel)
{
    const std::string _name = "levels " + getChannelName(colorSpace, channel);

    std::vector<deProperty*>::iterator i;
    for (i = properties.begin(); i != properties.end(); i++)
    {
        deProperty* property = *i;
        if (property->getName() == _name)
        {
            return;
        }
    }
    properties.push_back(new dePropertyLevels(_name, channel));
}

dePropertyLevels* deLevelsLayer::getPropertyLevels(int channel)
{
    const std::string _name = "levels " + getChannelName(colorSpace, channel);

    deProperty* p = getProperty(_name);
    return dynamic_cast<dePropertyLevels*>(p);
}

void deLevelsLayer::executeOperation(const std::string& operation)
{
    int n = getColorSpaceSize(colorSpace);

    int i;
    for (i = 0; i < n; i++)
    {
        dePropertyLevels* propertyLevels = getPropertyLevels(i);
        if (propertyLevels)
        {
            deLevels& levels = propertyLevels->getLevels();

            deValue min = 0.0;
            deValue middle = 0.5;
            deValue max = 1.0;

            if (operation == "auto levels")
            {
                calcAutoLevels(i, min, middle, max, 0.0001);
            }

            if (operation == "auto levels light")
            {
                calcAutoLevels(i, min, middle, max, 0.02);
            }

            if (operation == "auto levels heavy")
            {
                calcAutoLevels(i, min, middle, max, 0.1);
            }

            levels.setMin(min);
            levels.setMiddle(middle);
            levels.setMax(max);
        }            
        

    }
    
}

void deLevelsLayer::calcAutoLevels(int channel, deValue& min, deValue& middle, deValue& max, deValue e)
{
    if (!shouldUseAutoLevels(colorSpace, channel))
    {
        return;
    }

    const deValue* c = getSourceImage().getValues(channel);
    int n = getSourceImage().getChannelSize().getN();

    int s = 1024;
    deHistogram histogram(s);

    histogram.clear();
    histogram.calc(c, n);

    min = histogram.getLeft(e);
    max = histogram.getRight(e);
    middle = (min + max) / 2.0;
}

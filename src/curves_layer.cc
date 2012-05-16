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

#include "frame_factory.h"

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
    const deCurveOld* curve = p->getCurve(i);

    deChannel* sourceChannel = channelManager.getChannel(s);
    if (sourceChannel)
    {
        mainLayerImage.enableChannel(i);
        int c = mainLayerImage.getChannelIndex(i);
        deChannel* channel = channelManager.getChannel(c);

        if (channel)
        {
            curve->process(*sourceChannel, *channel, sourceImage.getChannelSize().getN());
        }
    }

    return true;

}

bool deCurvesLayer::isChannelNeutral(int index)
{
    dePropertyCurves* p = getPropertyCurves();
    if (!p)
    {
        return false;
    }
    deCurveOld* curve = p->getCurve(index);
    return curve->isNeutral();
}    

void deCurvesLayer::save(xmlNodePtr root)
{
    saveCommon(root);
    saveBlend(root);

    dePropertyCurves* p = getPropertyCurves();

    int n = getColorSpaceSize(colorSpace);
    int i;
    for (i = 0; i < n; i++)
    {
        deCurveOld* curve = p->getCurve(i);
        xmlNodePtr child = xmlNewChild(root, NULL, BAD_CAST("curve"), NULL);
        curve->save(child);
    }
}

void deCurvesLayer::load(xmlNodePtr root)
{
    loadBlend(root);

    xmlNodePtr child = root->xmlChildrenNode;

    dePropertyCurves* p = getPropertyCurves();

    int i = 0;
    while (child)
    {
        if ((!xmlStrcmp(child->name, BAD_CAST("curve")))) 
        {
            deCurveOld* curve = p->getCurve(i);
            curve->load(child);
            i++;
        }

        child = child->next;
    }
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
        p->getCurve(i)->reset();
    }
}

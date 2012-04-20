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
#include "curves_editor.h"

#include "str.h"

#include "color_space_utils.h"
#include "channel_manager.h"

deCurvesLayer::deCurvesLayer(deColorSpace _colorSpace, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager)
:deLayer( _colorSpace,  _sourceLayer, _layerStack, _channelManager)
{
    int n = getColorSpaceSize(colorSpace);
    curves = new deCurve[n];
}

deCurvesLayer::~deCurvesLayer()
{
    delete [] curves;
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

    deChannel* sourceChannel = channelManager.getChannel(s);
    if (sourceChannel)
    {
        mainLayerImage.enableChannel(i);
        int c = mainLayerImage.getChannelIndex(i);
        deChannel* channel = channelManager.getChannel(c);

        if (channel)
        {
            channel->lockWrite();
            sourceChannel->lockRead();

            curves[i].process(*sourceChannel, *channel, channelManager.getChannelSize().getN());

            sourceChannel->unlockRead();
            channel->unlockWrite();
        }
    }

    return true;

}

deCurve* deCurvesLayer::getCurve(int index)
{
    int n = getColorSpaceSize(colorSpace);
    if ((index < 0) || (index >= n))
    {
        return NULL;
    }
    return &(curves[index]);
}

bool deCurvesLayer::isChannelNeutral(int index)
{
    return curves[index].isNeutral();
}    

void deCurvesLayer::save(xmlNodePtr root)
{
    saveCommon(root);
    saveBlend(root);

    int n = getColorSpaceSize(colorSpace);
    int i;
    for (i = 0; i < n; i++)
    {
        deCurve& curve = curves[i];
        xmlNodePtr child = xmlNewChild(root, NULL, BAD_CAST("curve"), NULL);
        curve.save(child);
    }
}

void deCurvesLayer::load(xmlNodePtr root)
{
    loadBlend(root);

    xmlNodePtr child = root->xmlChildrenNode;

    int i = 0;
    while (child)
    {
        if ((!xmlStrcmp(child->name, BAD_CAST("curve")))) 
        {
            //assert(i < n);
            deCurve& curve = curves[i];
            curve.load(child);
            i++;
        }

        child = child->next;
    }
}


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

#include "blur_layer.h"
#include "project.h"
#include <iostream>
#include "str.h"
#include "xml.h"
#include "frame_factory.h"
#include "layer_processor.h"

deBlurLayer::deBlurLayer(deColorSpace _colorSpace, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager)
:deLayer( _colorSpace, _sourceLayer, _layerStack, _channelManager),
 blurRadius("blur_radius"),
 threshold("threshold"),
 blurType("blur_type"),
 viewManager(_viewManager)
{
    blurRadius.setLabel("radius");
    blurRadius.setMin(0);
    blurRadius.setMax(50);
    getSupportedBlurTypes(blurType.getChoices());
    blurType.setLabel("blur type");
    reset();
}

deBlurLayer::~deBlurLayer()
{
}

void deBlurLayer::reset()
{
    blurRadius.set(5);
    blurType.set("gaussian");
}

bool deBlurLayer::isChannelNeutral(int index)
{
    return (blurRadius.get() == 0);
}    

void deBlurLayer::save(xmlNodePtr root)
{
    saveCommon(root);
    saveBlend(root);

    blurRadius.save(root);
    blurType.save(root);
    threshold.save(root);
}

void deBlurLayer::load(xmlNodePtr root)
{
    loadBlend(root);

    xmlNodePtr child = root->xmlChildrenNode;

    while (child)
    {

        blurRadius.load(child);
        blurType.load(child);
        threshold.load(child);

        child = child->next;

    }        

}

bool deBlurLayer::updateMainImageSingleChannel(int i)
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

            deValue r = viewManager.getRealScale() * blurRadius.get();

            deBlurType type = blurTypeFromString(blurType.get());
            
            blurChannel(sourceChannel->getPixels(), channel->getPixels(), mainLayerImage.getChannelSize(), r, r, type, threshold.get());

            sourceChannel->unlockRead();
            channel->unlockWrite();
        }
    }

    return true;

}


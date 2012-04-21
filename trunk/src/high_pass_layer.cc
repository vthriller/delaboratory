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

#include "high_pass_layer.h"
#include "project.h"
#include <iostream>
#include "blur.h"
#include "str.h"
#include "xml.h"
#include "frame_factory.h"
#include "copy_channel.h"
#include "blend_channel.h"
#include "process_linear.h"
#include "layer_processor.h"
#include "channel_manager.h"

deHighPassLayer::deHighPassLayer(deColorSpace _colorSpace, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager)
:deLayer(_colorSpace, _sourceLayer, _layerStack, _channelManager),
 viewManager(_viewManager)
{
    blurRadiusIndex = registerPropertyValue("blur_radius");
    dePropertyValue* blurRadius = getPropertyValue(blurRadiusIndex);

    blurRadius->setLabel("radius");
    blurRadius->setMin(1);
    blurRadius->setMax(50);
    reset();
    disableNotForSharpen();
}

void deHighPassLayer::reset()
{
    dePropertyValue* blurRadius = getPropertyValue(blurRadiusIndex);
    blurRadius->set(5);
}

deHighPassLayer::~deHighPassLayer()
{
}

bool deHighPassLayer::processHP(const deChannel& sourceChannel, deChannel& channel)
{
    deSize size = mainLayerImage.getChannelSize();
    logMessage("high pass start");

    const deValue* source = sourceChannel.getPixels();
    deValue* destination = channel.getPixels();

    deValue* blurMap = NULL;
    try
    {
        blurMap = new deValue [size.getN()];
    }
    catch (std::bad_alloc)
    {
        logMessage("ERROR allocating memory in high pass");
        if (blurMap)
        {
            delete [] blurMap;
        }
        return false;
    }

    dePropertyValue* blurRadius = getPropertyValue(blurRadiusIndex);

    deValue r = viewManager.getRealScale() * blurRadius->get();
    deBlurType type = deGaussianBlur;
    bool result = blurChannel(source, blurMap, size, r, r, type, 0.0);

    blendChannel(source, blurMap, destination, NULL, deBlendGrainExtract, 1.0, size.getN());

    delete [] blurMap;

    logMessage("high pass end");

    return result;
}


bool deHighPassLayer::isChannelNeutral(int index)
{
    return false;
}    

void deHighPassLayer::save(xmlNodePtr root)
{
    saveCommon(root);
    saveBlend(root);
    saveValueProperties(root);
}

void deHighPassLayer::load(xmlNodePtr root)
{
    loadBlend(root);

    xmlNodePtr child = root->xmlChildrenNode;

    while (child)
    {
        loadValueProperties(child);

        child = child->next;
    }        
}

bool deHighPassLayer::updateMainImageSingleChannel(int i)
{
    const deImage& sourceImage = getSourceImage();

    int s = sourceImage.getChannelIndex(i);

    if ((isChannelNeutral(i)) || (!isChannelEnabled(i)))
    {
        mainLayerImage.disableChannel(i, s);
        return true;
    }

    bool actionResult = false;

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

            actionResult = processHP(*sourceChannel, *channel);

            sourceChannel->unlockRead();
            channel->unlockWrite();
        }
    }

    return actionResult;

}

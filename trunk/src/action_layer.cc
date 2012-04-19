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

#include "action_layer.h"
#include "layer_stack.h"
#include "layer_processor.h"
#include "view_manager.h"
#include "channel_manager.h"
#include "apply_luminance_color.h"
#include "blend_frame.h"
#include "blur.h"
#include "copy_channel.h"
#include "process_linear.h"
#include "str.h"
#include "xml.h"
#include "logger.h"
#include "color_space_utils.h"

deActionLayer::deActionLayer(deColorSpace _colorSpace, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager)
:deLayer(_colorSpace, _channelManager, _sourceLayer, _layerStack),
 viewManager(_viewManager)
{

}

deActionLayer::~deActionLayer()
{
}

deChannel* deActionLayer::getSourceChannel(int index)
{
    return channelManager.getChannel(getSourceImage().getChannelIndex(index));
}

deSize deActionLayer::getChannelSize() const
{
    return channelManager.getChannelSize();
}

bool deActionLayer::updateMainImageSingleChannel(int i)
{
    bool actionResult = false;

    const deImage& sourceImage = getSourceImage();

    int s = sourceImage.getChannelIndex(i);

    if ((isChannelNeutral(i)) || (!isChannelEnabled(i)))
    {
        mainLayerImage.disableChannel(i, s);
        return true;
    }
    else
    {
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

                actionResult = processAction(i, *sourceChannel, *channel, channelManager.getChannelSize());

                sourceChannel->unlockRead();
                channel->unlockWrite();
            }
        }
    }

    return actionResult;

}

void deActionLayer::saveBlend(xmlNodePtr root)
{
//    saveChild(root, "enabled", str(isEnabled()));
//    saveChild(root, "blend_mode", getBlendModeName(blendMode));
//    saveChild(root, "opacity", str(opacity));

    int n = getColorSpaceSize(colorSpace);
    int i;
    for (i = 0; i < n; i++)
    {
        bool c = isChannelEnabled(i);
        saveChild(root, "channel", str(c));
    }
}

void deActionLayer::loadBlend(xmlNodePtr root)
{
    xmlNodePtr child = root->xmlChildrenNode;

    int channelIndex = 0;
    //channels.clear();

    while (child)
    {

        if ((!xmlStrcmp(child->name, BAD_CAST("channel")))) 
        {
            bool c = getBool(getContent(child));
            if (c)
            {
                //channels.insert(channelIndex);
            }
            channelIndex++;
        }

        if ((!xmlStrcmp(child->name, BAD_CAST("enabled")))) 
        {
            setEnabled(getBool(getContent(child)));
        }

        if ((!xmlStrcmp(child->name, BAD_CAST("blend_mode")))) 
        {
            //blendMode = blendModeFromString(getContent(child));
        }

        if ((!xmlStrcmp(child->name, BAD_CAST("opacity"))))
        {
            //opacity = getValue(getContent(child));
        }

        child = child->next;

    }        

}

void deActionLayer::setHistogramChannel(int channel)
{
    viewManager.setHistogramChannel(channel);
}


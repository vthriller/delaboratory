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

#include "apply_image_layer.h"
#include "layer_stack.h"
#include "channel_manager.h"
#include "xml.h"
#include "str.h"
#include "frame_factory.h"
#include "color_space_utils.h"

deApplyImageLayer::deApplyImageLayer(deColorSpace _colorSpace, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager, int _layerIndex)
:deActionLayer(_colorSpace, _sourceLayer, _layerStack, _channelManager, _viewManager),
appliedLayer("applied_layer"),
applySingleChannel("apply_single_channel")
{
    appliedChannel = 0;
    applySingleChannel.set(false);

    std::vector<std::string>& choices = appliedLayer. getChoices();
    int i;
    for (i = 0; i < _layerIndex; i++)
    {
        choices.push_back(str(i));
    }

    appliedLayer.setIndex(_layerIndex - 1);

}

deApplyImageLayer::~deApplyImageLayer()
{
}

bool deApplyImageLayer::updateMainImageSingleChannel(int i)
{

    if (!isChannelEnabled(i))
    {
        mainLayerImage.disableChannel(i, getSourceImage().getChannelIndex(i));
        return true;
    }

    int a = getInt(appliedLayer.get());

    const deImage& appliedImage = getOtherLayerImage(a);

    int n = getColorSpaceSize(appliedImage.getColorSpace());

    int c = -1 ;

    if (applySingleChannel.get())
    {
        c = appliedImage.getChannelIndex(appliedChannel);
    }
    else
    {
        if (i < n)
        {
            c = appliedImage.getChannelIndex(i);
        }
        else
        {
            c = appliedImage.getChannelIndex(0);
        }
    }        

    mainLayerImage.disableChannel(i, c);

    return true;
}

deColorSpace deApplyImageLayer::getAppliedColorSpace()
{
    const deImage& appliedImage = getOtherLayerImage(getInt(appliedLayer.get()));
    return appliedImage.getColorSpace();
}

bool deApplyImageLayer::isChannelNeutral(int index)
{
    return false;
}    

void deApplyImageLayer::setAppliedChannel(int c)
{
    appliedChannel = c;
}

void deApplyImageLayer::save(xmlNodePtr root)
{
    saveCommon(root);
    saveBlend(root);

    appliedLayer.save(root);
    applySingleChannel.save(root);

    saveChild(root, "applied_channel", str(appliedChannel));
}

void deApplyImageLayer::load(xmlNodePtr root)
{
    loadBlend(root);

    xmlNodePtr child = root->xmlChildrenNode;

    while (child)
    {

        appliedLayer.load(child);
        applySingleChannel.load(child);

        if ((!xmlStrcmp(child->name, BAD_CAST("applied_channel")))) 
        {
            appliedChannel = getValue(getContent(child));
        }

        child = child->next;

    }        
}

bool deApplyImageLayer::isSingleChannel() const
{
    return applySingleChannel.get();
}

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

deApplyImageLayer::deApplyImageLayer(deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deLayerProcessor& _layerProcessor, deChannelManager& _channelManager, deViewManager& _viewManager, const std::string& _name)
:deActionLayer(_name, _colorSpace, _index, _sourceLayer, _layerStack, _layerProcessor, _channelManager, _viewManager),
appliedLayer("applied_layer"),
applySingleChannel("apply_single_channel")
{
    appliedChannel = 0;
    applySingleChannel.set(false);

    std::vector<std::string>& choices = appliedLayer. getChoices();
    int i;
    for (i = 0; i < index; i++)
    {
        choices.push_back(str(i));
    }

    appliedLayer.setIndex(0);

}

deApplyImageLayer::~deApplyImageLayer()
{
}

void deApplyImageLayer::processAction(int i)
{
    int a = getInt(appliedLayer.get());
    deLayer* applied = layerStack.getLayer(a);
    if (!applied)
    {
        return;
    }
    const deImage& appliedImage = applied->getImage();
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

    imageActionPass.disableChannel(i, c);
}

deColorSpace deApplyImageLayer::getAppliedColorSpace()
{
    int a = getInt(appliedLayer.get());
    deLayer* applied = layerStack.getLayer(a);
    if (!applied)
    {
        return deColorSpaceInvalid;
    }
    const deImage& appliedImage = applied->getImage();
    return appliedImage.getColorSpace();
}

bool deApplyImageLayer::isChannelNeutral(int index)
{
    return false;
}    

void deApplyImageLayer::setAppliedChannel(int c)
{
    appliedChannel = c;
    updateImage();
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

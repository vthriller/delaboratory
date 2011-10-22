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
#include "apply_image_frame.h"
#include "xml.h"
#include "str.h"

deApplyImageLayer::deApplyImageLayer(deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager, const std::string& _name)
:deActionLayer(_name, _colorSpace, _index, _sourceLayer, _layerStack, _channelManager, _viewManager) 
{
    appliedLayer = 0;
    appliedChannel = 0;
    singleChannel = false;

}

deApplyImageLayer::~deApplyImageLayer()
{
}

void deApplyImageLayer::processAction(int i)
{
    deLayer* applied = layerStack.getLayer(appliedLayer);
    const deImage& appliedImage = applied->getImage();
    int n = getColorSpaceSize(appliedImage.getColorSpace());

    int c = -1 ;

    if (singleChannel)
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
    deLayer* applied = layerStack.getLayer(appliedLayer);
    const deImage& appliedImage = applied->getImage();
    return appliedImage.getColorSpace();
}

void deApplyImageLayer::createActionFrame(wxWindow* parent)
{
    if (!actionFrame)
    {
        actionFrame = new deApplyImageFrame(parent, *this);
        actionFrame->Show(true);
    }        
}


bool deApplyImageLayer::isChannelNeutral(int index)
{
    return false;
}    

void deApplyImageLayer::enableSingleChannel()
{
    singleChannel = true;
    updateImage();
}

void deApplyImageLayer::disableSingleChannel()
{
    singleChannel = false;
    updateImage();
}

void deApplyImageLayer::setAppliedLayer(int l)
{
    appliedLayer = l;
    updateImage();
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

    saveChild(root, "applied_layer", str(appliedLayer));
    saveChild(root, "applied_channel", str(appliedChannel));
    saveChild(root, "single_channel", str(singleChannel));
}

void deApplyImageLayer::load(xmlNodePtr root)
{
    loadBlend(root);

    xmlNodePtr child = root->xmlChildrenNode;

    while (child)
    {

        if ((!xmlStrcmp(child->name, BAD_CAST("applied_layer")))) 
        {
            appliedLayer = getInt(getContent(child));
        }

        if ((!xmlStrcmp(child->name, BAD_CAST("applied_channel")))) 
        {
            appliedChannel = getValue(getContent(child));
        }

        if ((!xmlStrcmp(child->name, BAD_CAST("single_channel")))) 
        {
            singleChannel = getBool(getContent(child));
        }

        child = child->next;

    }        
}


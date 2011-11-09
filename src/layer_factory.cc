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

#include "layer_factory.h"
#include "layer_stack.h"
#include "curves_layer.h"
#include "blur_layer.h"
#include "apply_image_layer.h"
#include "mixer_layer.h"
#include "conversion_layer.h"
#include "conversion_bw_layer.h"
#include "usm_layer.h"
#include "source_image_layer.h"


deLayer* createLayer(const std::string& type, int source, deColorSpace colorSpace, deLayerStack& _layerStack, deLayerProcessor& _processor, deChannelManager& _channelManager, deViewManager& _viewManager, const std::string& name, deChannelManager& _sourceChannelManager)
{
    int index = _layerStack.getSize();

    if (type == "curves")
    {
        return new deCurvesLayer(colorSpace, index, source, _layerStack, _processor, _channelManager, _viewManager, name);
    }

    if (type == "blur")
    {
        return new deBlurLayer(colorSpace, index, source, _layerStack, _processor, _channelManager, _viewManager, name);
    }

    if (type == "usm")
    {
        return new deUSMLayer(colorSpace, index, source, _layerStack, _processor, _channelManager, _viewManager, name);
    }

    if (type == "mixer")
    {
        return new deMixerLayer(colorSpace, index, source, _layerStack, _processor, _channelManager, _viewManager, name);
    }

    if (type == "apply_image")
    {
        return new deApplyImageLayer(colorSpace, index, source, _layerStack, _processor, _channelManager, _viewManager, name);
    }
    
    bool cbw = false;

    if (type == "conversion")
    {
        if (colorSpace == deColorSpaceBW)
        {
            cbw = true;
        }
        else
        {
            return new deConversionLayer(colorSpace, index, source, _layerStack, _processor, _channelManager);
        }
    }

    if (type == "conversion_bw")
    {
        cbw = true;
    }

    if (cbw)
    {
        deLayer* sourceLayer = _layerStack.getLayer(source);
        deColorSpace sourceColorSpace = sourceLayer->getColorSpace();
        int n = getColorSpaceSize(sourceColorSpace);
        return new deConversionBWLayer(index, source, _layerStack, _processor, _channelManager, _viewManager, n);
    }

    if (type == "source_image")
    {
        return new deSourceImageLayer(index,  _channelManager, _viewManager, _sourceChannelManager);
    }

    return NULL;
}

void getSupportedActions(std::vector<std::string>& actions)
{
    actions.push_back("curves");
    actions.push_back("mixer");
    actions.push_back("apply_image");
    actions.push_back("blur");
    actions.push_back("usm");
//    actions.push_back("nd");
}


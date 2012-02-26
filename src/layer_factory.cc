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
#include "vignette_layer.h"
#include "apply_image_layer.h"
#include "mixer_layer.h"
#include "conversion_layer.h"
#include "conversion_bw_layer.h"
#include "conversion_bw2hue_layer.h"
#include "usm_layer.h"
#include "source_image_layer.h"
#include "dodge_burn_layer.h"
#include "high_pass_layer.h"
#include "shadows_highlights_layer.h"
#include "basic_layer.h"
#include <algorithm>

deLayer* createLayer(const std::string& type, int source, deColorSpace colorSpace, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager, const std::string& name, deStaticImage& sourceImage)
{
    int index = _layerStack.getSize();

    if (type == "curves")
    {
        return new deCurvesLayer(colorSpace, index, source, _layerStack, _channelManager, _viewManager, name);
    }

    if (type == "blur")
    {
        return new deBlurLayer(colorSpace, index, source, _layerStack, _channelManager, _viewManager, name);
    }

    if (type == "vignette")
    {
        return new deVignetteLayer(colorSpace, index, source, _layerStack, _channelManager, _viewManager, name);
    }

    if (type == "usm")
    {
        return new deUSMLayer(colorSpace, index, source, _layerStack, _channelManager, _viewManager, name);
    }

    if (type == "basic")
    {
        return new deBasicLayer(colorSpace, index, source, _layerStack, _channelManager, _viewManager, name);
    }

    if (type == "dodge_burn")
    {
        return new deDodgeBurnLayer(colorSpace, index, source, _layerStack, _channelManager, _viewManager, name);
    }

    if (type == "high_pass")
    {
        return new deHighPassLayer(colorSpace, index, source, _layerStack, _channelManager, _viewManager, name);
    }

    if (type == "shadows_highlights")
    {
        return new deShadowsHighlightsLayer(colorSpace, index, source, _layerStack, _channelManager, _viewManager, name);
    }

    if (type == "mixer")
    {
        return new deMixerLayer(colorSpace, index, source, _layerStack, _channelManager, _viewManager, name);
    }

    if (type == "apply_image")
    {
        return new deApplyImageLayer(colorSpace, index, source, _layerStack, _channelManager, _viewManager, name);
    }
    
    bool cbw = false;
    bool cbwhue = false;

    if (type == "conversion")
    {
        if (colorSpace == deColorSpaceBW)
        {
            cbw = true;
        }
        else
        {
            deLayer* sourceLayer = _layerStack.getLayer(source);
            deColorSpace sourceColorSpace = sourceLayer->getColorSpace();
            if (sourceColorSpace == deColorSpaceBW)
            {
                switch (colorSpace)
                {
                    case deColorSpaceHSL:
                    case deColorSpaceHSV:
                    case deColorSpaceLCH:
                    {
                        cbwhue = true;
                        break;
                    }
                    default:
                        break;
                }
            }
        }

        if ((!cbw) && (!cbwhue))
        {
            return new deConversionLayer(colorSpace, index, source, _layerStack, _channelManager);
        }
    }

    if (type == "conversion_bw")
    {
        cbw = true;
    }

    if (type == "conversion_bw2hue")
    {
        cbwhue = true;
    }

    if (cbw)
    {
        deLayer* sourceLayer = _layerStack.getLayer(source);
        deColorSpace sourceColorSpace = sourceLayer->getColorSpace();
        int n = getColorSpaceSize(sourceColorSpace);
        return new deConversionBWLayer(index, source, _layerStack, _channelManager, n);
    }

    if (cbwhue)
    {
        return new deConversionBW2HueLayer(index, source, _layerStack, _channelManager, colorSpace);
    }

    if (type == "source_image")
    {
        return new deSourceImageLayer(index,  _channelManager, _viewManager, sourceImage, colorSpace);
    }

    return NULL;
}

void getSupportedActions(std::vector<std::string>& actions)
{
    actions.push_back("basic");
    actions.push_back("curves");
    actions.push_back("mixer");
    actions.push_back("apply_image");
    actions.push_back("dodge_burn");
    actions.push_back("shadows_highlights");
    actions.push_back("vignette");
    actions.push_back("blur");
    actions.push_back("high_pass");
    actions.push_back("usm");
}

std::string getActionGroup(const std::string n)
{
    if ((n == "vignette") || (n == "dodge_burn") || (n == "shadows_highlights"))
    {
        return "light";
    }

    if ((n == "blur") || (n == "high_pass") || (n == "usm"))
    {
        return "sharp";
    }

    return "gen";
}

void getSupportedActionGroups(std::vector<std::string>& names)
{
    std::vector<std::string> actions;
    getSupportedActions(actions);
    std::vector<std::string>::const_iterator i;
    for (i = actions.begin(); i != actions.end(); i++)
    {
        std::string n = getActionGroup(*i);
        std::vector<std::string>::iterator j = find(names.begin(), names.end(), n);
        if (j == names.end())
        {
            names.push_back(n);
        }
    }
}


std::string getActionDescription(const std::string& a)
{
    if (a == "shadows_highlights")
    {
        return "sh / hi";
    }

    if (a == "apply_image")
    {
        return "apply image";
    }

    if (a == "high_pass")
    {
        return "high pass";
    }

    if (a == "dodge_burn")
    {
        return "d / b";
    }

    return a;
}


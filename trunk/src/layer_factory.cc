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
#include "fill_layer.h"
#include "vignette_layer.h"
#include "apply_image_layer.h"
#include "mixer_layer.h"
#include "apply_luminance_layer.h"
#include "equalizer_layer.h"
#include "conversion_layer.h"
#include "source_image_layer.h"
#include "dodge_burn_layer.h"
#include "high_pass_layer.h"
#include "shadows_highlights_layer.h"
#include "basic_layer.h"
#include <algorithm>
#include "color_space_utils.h"
#include "levels_layer.h"
#include "local_contrast_layer.h"
#include "sharpen_layer.h"

deBaseLayer* createLayer(const std::string& type, int source, deColorSpace colorSpace, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager, deStaticImage& sourceImage)
{
    int index = _layerStack.getSize();

    if (type == "curves")
    {
        return new deCurvesLayer(colorSpace, source, _layerStack, _channelManager, _viewManager);
    }

    if (type == "blur")
    {
        return new deBlurLayer(colorSpace, source, _layerStack, _channelManager, _viewManager);
    }

    if (type == "basic")
    {
        return new deBasicLayer(colorSpace, source, _layerStack, _channelManager, _viewManager);
    }

    if (type == "dodge_burn")
    {
        return new deDodgeBurnLayer(colorSpace, source, _layerStack, _channelManager, _viewManager);
    }

    if (type == "high_pass")
    {
        return new deHighPassLayer(colorSpace, source, _layerStack, _channelManager, _viewManager);
    }

    if (type == "shadows_highlights")
    {
        return new deShadowsHighlightsLayer(colorSpace, source, _layerStack, _channelManager, _viewManager);
    }

    if (type == "mixer")
    {
        return new deMixerLayer(colorSpace, source, _layerStack, _channelManager, _viewManager);
    }

    if (type == "apply_luminance")
    {
        return new deApplyLuminanceLayer(colorSpace, source, _layerStack, _channelManager, _viewManager);
    }

    if (type == "equalizer8")
    {
        return new deEqualizerLayer8(colorSpace, source, _layerStack, _channelManager, _viewManager);
    }

    if (type == "equalizer16")
    {
        return new deEqualizerLayer16(colorSpace, source, _layerStack, _channelManager, _viewManager);
    }

    if (type == "apply_image")
    {
        return new deApplyImageLayer(colorSpace, source, _layerStack, _channelManager, _viewManager, index);
    }

    if (type == "levels")
    {
        return new deLevelsLayer(colorSpace, _channelManager, source, _layerStack);
    }        

    if (type == "fill")
    {
        return new deFillLayer(colorSpace, _channelManager, source, _layerStack);
    }        

    if (type == "local_contrast")
    {
        return new deLocalContrastLayer(colorSpace, _channelManager, source, _layerStack, _viewManager);
    }        

    if (type == "sharpen")
    {
        return new deSharpenLayer(colorSpace, _channelManager, source, _layerStack, _viewManager);
    }        

    if (type == "vignette")
    {
        return new deVignetteLayer(colorSpace, _channelManager, source, _layerStack, _viewManager);
    }        
    
    if (type == "conversion")
    {
        return new deConversionLayer(colorSpace, _channelManager, source, _layerStack);
    }

    if (type == "original")
    {
        return new deSourceImageLayer(_channelManager, _viewManager, sourceImage, colorSpace);
    }

    return NULL;
}

void getSupportedActions(std::vector<std::string>& actions)
{
    actions.push_back("levels");
    actions.push_back("curves");
    actions.push_back("fill");
    actions.push_back("local_contrast");
    actions.push_back("sharpen");
    actions.push_back("vignette");
    actions.push_back("basic");
    actions.push_back("mixer");
    actions.push_back("equalizer8");
    actions.push_back("equalizer16");
    actions.push_back("apply_image");
    actions.push_back("apply_luminance");

    actions.push_back("dodge_burn");
    actions.push_back("shadows_highlights");

    actions.push_back("high_pass");
    actions.push_back("blur");
}

std::string getActionGroup(const std::string n)
{
    if ((n == "levels") || (n == "local_contrast") || (n == "sharpen") || (n == "vignette") || (n == "apply_luminance"))
    {
        return "basic";
    }

    if ((n == "vignette1") || (n == "vignette2") || (n == "dodge_burn") || (n == "shadows_highlights"))
    {
        return "light";
    }

    if ((n == "blur") || (n == "high_pass"))
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

    if (a == "apply_luminance")
    {
        return "luminance";
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

    if (a == "local_contrast")
    {
        return "local contrast";
    }

    return a;
}


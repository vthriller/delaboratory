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
#include "equalizer_layer.h"
#include "fill_layer.h"
#include "tone_layer.h"
#include "saturation_layer.h"
#include "white_balance_layer.h"
#include "exposure_layer.h"
#include "vignette_layer.h"
#include "gradient_layer.h"
#include "gaussian_blur_layer.h"
#include "gaussian_blur_single_layer.h"
#include "recover_highlights_layer.h"
#include "recover_shadows_layer.h"
#include "mixer_layer.h"
#include "copy_layer.h"
#include "conversion_layer.h"
#include "source_image_layer.h"
#include "high_pass_layer.h"
#include "c2g_layer.h"
#include <algorithm>
#include "color_space_utils.h"
#include "levels_layer.h"
#include "local_contrast_layer.h"
#include "sharpen_layer.h"
#include "auto_dodge_layer.h"
#include "auto_burn_layer.h"

deBaseLayer* createLayer(const std::string& type, int source, deColorSpace colorSpace, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager, deStaticImage& sourceImage)
{
    if (type == "curves")
    {
        return new deCurvesLayer(colorSpace, source, _layerStack, _channelManager);
    }

    if (type == "equalizer")
    {
        return new deEqualizerLayer(colorSpace, source, _layerStack, _channelManager, _viewManager);
    }

    if (type == "mixer")
    {
        return new deMixerLayer(colorSpace, source, _layerStack, _channelManager, _viewManager);
    }

    if (type == "copy")
    {
        return new deCopyLayer(colorSpace, _channelManager, source, _layerStack);
    }

    if (type == "levels")
    {
        return new deLevelsLayer(colorSpace, _channelManager, source, _layerStack);
    }        

    if (type == "fill")
    {
        return new deFillLayer(colorSpace, _channelManager, source, _layerStack);
    }        

    if (type == "tone")
    {
        return new deToneLayer(colorSpace, _channelManager, source, _layerStack);
    }        

    if (type == "exposure")
    {
        return new deExposureLayer(colorSpace, _channelManager, source, _layerStack);
    }        

    if (type == "saturation")
    {
        return new deSaturationLayer(colorSpace, _channelManager, source, _layerStack);
    }        

    if (type == "white_balance")
    {
        return new deWhiteBalanceLayer(colorSpace, _channelManager, source, _layerStack);
    }        

    if (type == "local_contrast")
    {
        return new deLocalContrastLayer(colorSpace, _channelManager, source, _layerStack, _viewManager);
    }        

    if (type == "auto_dodge")
    {
        return new deAutoDodgeLayer(colorSpace, _channelManager, source, _layerStack, _viewManager);
    }        

    if (type == "auto_burn")
    {
        return new deAutoBurnLayer(colorSpace, _channelManager, source, _layerStack, _viewManager);
    }        

    if (type == "sharpen")
    {
        return new deSharpenLayer(colorSpace, _channelManager, source, _layerStack, _viewManager);
    }        

    if (type == "high_pass")
    {
        return new deHighPassLayer(colorSpace, _channelManager, source, _layerStack, _viewManager);
    }        

    if (type == "c2g")
    {
        return new deC2GLayer(colorSpace, _channelManager, source, _layerStack, _viewManager);
    }        

    if (type == "vignette")
    {
        return new deVignetteLayer(colorSpace, _channelManager, source, _layerStack, _viewManager);
    }        

    if (type == "gradient")
    {
        return new deGradientLayer(colorSpace, _channelManager, source, _layerStack, _viewManager);
    }        

    if (type == "gaussian_blur")
    {
        return new deGaussianBlurLayer(colorSpace, _channelManager, source, _layerStack, _viewManager);
    }        

    if (type == "gaussian_blur_single")
    {
        return new deGaussianBlurSingleLayer(colorSpace, _channelManager, source, _layerStack, _viewManager);
    }        

    if (type == "recover_highlights")
    {
        return new deRecoverHighlightsLayer(colorSpace, _channelManager, source, _layerStack, _viewManager);
    }        

    if (type == "recover_shadows")
    {
        return new deRecoverShadowsLayer(colorSpace, _channelManager, source, _layerStack, _viewManager);
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
    actions.push_back("equalizer");
    actions.push_back("exposure");
    actions.push_back("saturation");
    actions.push_back("white_balance");
    actions.push_back("fill");
    actions.push_back("tone");
    actions.push_back("local_contrast");
    actions.push_back("auto_dodge");
    actions.push_back("auto_burn");
    actions.push_back("sharpen");
    actions.push_back("vignette");
    actions.push_back("gradient");
    actions.push_back("gaussian_blur");
    actions.push_back("recover_highlights");
    actions.push_back("recover_shadows");
    actions.push_back("mixer");
    actions.push_back("high_pass");
    actions.push_back("c2g");
    actions.push_back("copy");
}

std::string getActionDescription(const std::string& a)
{
    if (a == "shadows_highlights")
    {
        return "sh / hi";
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
        return "loc contrast";
    }

    if (a == "gaussian_blur")
    {
        return "g. blur";
    }

    if (a == "gaussian_blur_single")
    {
        return "g. blur s";
    }

    if (a == "white_balance")
    {
        return "wb";
    }

    if (a == "recover_highlights")
    {
        return "highlights";
    }

    if (a == "recover_shadows")
    {
        return "shadows";
    }

    return a;
}


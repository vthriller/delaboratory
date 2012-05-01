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

#include "frame_factory.h"

#include "curves_editor.h"
#include "mixer_editor.h"
#include "apply_image_frame.h"
#include "blur_frame.h"
#include "equalizer_frame.h"
#include "basic_frame.h"
#include "dodge_burn_frame.h"
#include "high_pass_frame.h"
#include "shadows_highlights_frame.h"

#include "logger.h"

#include "generic_layer_frame.h"

#include "window_wx.h"

deFrameOld* createFrame(wxWindow *parent, deBaseLayer& layer, deLayerProcessor& layerProcessor, deLayerFrameManager& frameManager, int layerIndex, deChannelManager& channelManager)
{
    logMessage("creating frame...");
    const std::string type = layer.getType();
    logMessage("creating frame of type " + type);

    if (type == "curves")
    {
        deLayer& al = dynamic_cast<deLayer&>(layer);
        return new deCurvesEditor(parent, al, layerProcessor, frameManager, layerIndex, channelManager);
    }        

    if (type == "mixer")
    {
        deLayer& al = dynamic_cast<deLayer&>(layer);
        return new deMixerEditor(parent, al, layerProcessor, frameManager, layerIndex);
    }        

    if (type == "apply_image")
    {
        deLayer& al = dynamic_cast<deLayer&>(layer);
        return new deApplyImageFrame(parent, al, layerProcessor, frameManager, layerIndex);
    }        

    if (type == "blur")
    {
        deLayer& al = dynamic_cast<deLayer&>(layer);
        return new deBlurFrame(parent, al, layerProcessor, frameManager, layerIndex);
    }        

    if ((type == "equalizer8") || (type == "equalizer16"))
    {
        deLayer& al = dynamic_cast<deLayer&>(layer);
        return new deEqualizerFrame(parent, al, layerProcessor, frameManager, layerIndex);
    }        

    if (type == "basic")
    {
        deLayer& al = dynamic_cast<deLayer&>(layer);
        return new deBasicFrame(parent, al, layerProcessor, frameManager, layerIndex, channelManager);
    }        

    if (type == "dodge_burn")
    {
        deLayer& al = dynamic_cast<deLayer&>(layer);
        return new deDodgeBurnFrame(parent, al, layerProcessor, frameManager, layerIndex);
    }        

/*
    if ((type == "vignette1") || (type == "vignette2"))
    {
        deLayer& al = dynamic_cast<deLayer&>(layer);
        return new deVignetteFrame(parent, al, layerProcessor, frameManager, layerIndex);
    }        
    */

    if (type == "high_pass")
    {
        deLayer& al = dynamic_cast<deLayer&>(layer);
        return new deHighPassFrame(parent, al, layerProcessor, frameManager, layerIndex);
    }        

    if (type == "shadows_highlights")
    {
        deLayer& al = dynamic_cast<deLayer&>(layer);
        return new deShadowsHighlightsFrame(parent, al, layerProcessor, frameManager, layerIndex);
    }        

/*
    if (type == "conversion_bw")
    {
        deConversionBWLayer& bwl = dynamic_cast<deConversionBWLayer&>(layer);
        return new deMixerBWEditor(parent, bwl, layerProcessor, frameManager, layerIndex);
    }        

    if (type == "conversion_bw2hue")
    {
        deConversionBW2HueLayer& bwl = dynamic_cast<deConversionBW2HueLayer&>(layer);
        return new deHueSaturationFrame(parent, bwl, layerProcessor, frameManager, layerIndex);
    }        
    */

    return NULL;
}        

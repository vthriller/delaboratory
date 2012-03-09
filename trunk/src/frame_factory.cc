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
#include "usm_frame.h"
#include "basic_frame.h"
#include "mixer_bw_editor.h"
#include "conversion_bw_layer.h"
#include "dodge_burn_frame.h"
#include "vignette_frame.h"
#include "high_pass_frame.h"
#include "shadows_highlights_frame.h"
#include "hue_saturation_frame.h"

#include "action_layer.h"
#include "conversion_bw2hue_layer.h"

deFrame* createFrame(wxWindow *parent, deLayer& layer, deLayerProcessor& layerProcessor, deLayerFrameManager& frameManager)
{
    const std::string type = layer.getType();

    if (type == "curves")
    {
        deActionLayer& al = dynamic_cast<deActionLayer&>(layer);
        return new deCurvesEditor(parent, al, layerProcessor, frameManager);
    }        

    if (type == "mixer")
    {
        deActionLayer& al = dynamic_cast<deActionLayer&>(layer);
        return new deMixerEditor(parent, al, layerProcessor, frameManager);
    }        

    if (type == "apply_image")
    {
        deActionLayer& al = dynamic_cast<deActionLayer&>(layer);
        return new deApplyImageFrame(parent, al, layerProcessor, frameManager);
    }        

    if (type == "blur")
    {
        deActionLayer& al = dynamic_cast<deActionLayer&>(layer);
        return new deBlurFrame(parent, al, layerProcessor, frameManager);
    }        

    if (type == "equalizer")
    {
        deActionLayer& al = dynamic_cast<deActionLayer&>(layer);
        return new deEqualizerFrame(parent, al, layerProcessor, frameManager);
    }        

    if (type == "usm")
    {
        deActionLayer& al = dynamic_cast<deActionLayer&>(layer);
        return new deUSMFrame(parent, al, layerProcessor, frameManager);
    }        

    if (type == "basic")
    {
        deActionLayer& al = dynamic_cast<deActionLayer&>(layer);
        return new deBasicFrame(parent, al, layerProcessor, frameManager);
    }        

    if (type == "dodge_burn")
    {
        deActionLayer& al = dynamic_cast<deActionLayer&>(layer);
        return new deDodgeBurnFrame(parent, al, layerProcessor, frameManager);
    }        

    if (type == "vignette")
    {
        deActionLayer& al = dynamic_cast<deActionLayer&>(layer);
        return new deVignetteFrame(parent, al, layerProcessor, frameManager);
    }        

    if (type == "high_pass")
    {
        deActionLayer& al = dynamic_cast<deActionLayer&>(layer);
        return new deHighPassFrame(parent, al, layerProcessor, frameManager);
    }        

    if (type == "shadows_highlights")
    {
        deActionLayer& al = dynamic_cast<deActionLayer&>(layer);
        return new deShadowsHighlightsFrame(parent, al, layerProcessor, frameManager);
    }        

    if (type == "conversion_bw")
    {
        deConversionBWLayer& bwl = dynamic_cast<deConversionBWLayer&>(layer);
        return new deMixerBWEditor(parent, bwl, layerProcessor, frameManager);
    }        

    if (type == "conversion_bw2hue")
    {
        deConversionBW2HueLayer& bwl = dynamic_cast<deConversionBW2HueLayer&>(layer);
        return new deHueSaturationFrame(parent, bwl, layerProcessor, frameManager);
    }        


    return NULL;
}        

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

#include "equalizer_frame.h"
#include "basic_frame.h"
#include "dodge_burn_frame.h"
#include "shadows_highlights_frame.h"

#include "logger.h"

#include "generic_layer_frame.h"

#include "window_wx.h"

#include "curves_layer.h"

deFrameOld* createFrame(wxWindow *parent, deBaseLayer& layer, deLayerProcessor& layerProcessor, deLayerFrameManager& frameManager, int layerIndex, deChannelManager& channelManager)
{
    const std::string type = layer.getType();

/*
    if (type == "curves")
    {
        deCurvesLayer& al = dynamic_cast<deCurvesLayer&>(layer);
        return new deCurvesEditor(parent, al, layerProcessor, frameManager, layerIndex, channelManager);
    }        
    */

    if ((type == "equalizer8") || (type == "equalizer16"))
    {
        deLayerOld& al = dynamic_cast<deLayerOld&>(layer);
        return new deEqualizerFrame(parent, al, layerProcessor, frameManager, layerIndex);
    }        

    if (type == "basic")
    {
        deLayerOld& al = dynamic_cast<deLayerOld&>(layer);
        return new deBasicFrame(parent, al, layerProcessor, frameManager, layerIndex, channelManager);
    }        

    if (type == "dodge_burn")
    {
        deLayerOld& al = dynamic_cast<deLayerOld&>(layer);
        return new deDodgeBurnFrame(parent, al, layerProcessor, frameManager, layerIndex);
    }        

    if (type == "shadows_highlights")
    {
        deLayerOld& al = dynamic_cast<deLayerOld&>(layer);
        return new deShadowsHighlightsFrame(parent, al, layerProcessor, frameManager, layerIndex);
    }        

    return NULL;
}        

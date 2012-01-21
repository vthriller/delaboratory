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

#include "shadows_highlights_layer.h"
#include "project.h"
#include <iostream>
#include "blur.h"
#include "str.h"
#include "xml.h"
#include "frame_factory.h"
#include "copy_channel.h"
#include "blend_channel.h"
#include "process_linear.h"
#include "layer_processor.h"

deShadowsHighlightsLayer::deShadowsHighlightsLayer(deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager, const std::string& _name)
:deActionLayer(_name, _colorSpace, _index, _sourceLayer, _layerStack, _channelManager, _viewManager),
 blurRadius("blur_radius"),
 shadowsHighlightsAmount("shadows_highlights_amount"),
 darkenAmount("darken_amount"),
 lightenAmount("lighten_amount")
{
    blurRadius.setLabel("radius");
    shadowsHighlightsAmount.setLabel("sh/hi");
    darkenAmount.setLabel("darken");
    lightenAmount.setLabel("lighten");
    reset();
}

void deShadowsHighlightsLayer::reset()
{
    blurRadius.set(0.3);
    shadowsHighlightsAmount.set(0.3);
    darkenAmount.set(0.0);
    lightenAmount.set(0.0);
}

deShadowsHighlightsLayer::~deShadowsHighlightsLayer()
{
}

void deShadowsHighlightsLayer::processAction(int i, const deChannel& sourceChannel, deChannel& channel, deSize size)
{
    logMessage("shadows/highlights start");

    const deValue* source = sourceChannel.getPixels();
    deValue* destination = channel.getPixels();

    deValue* blurMap = new deValue [size.getN()];
    deValue* firstStage = new deValue [size.getN()];
    deValue* secondStage = new deValue [size.getN()];

    deValue r = viewManager.getRealScale() * blurRadius.get() * 200;
    deBlurType type = deGaussianBlur;
    blurChannel(source, blurMap, size, r, type, 0.0);

    deValue sha = shadowsHighlightsAmount.get();
    blendChannel(source, blurMap, firstStage, NULL, deBlendOverlayInvert, sha, size.getN());

    deValue da = darkenAmount.get();
    blendChannel(firstStage, source, secondStage, NULL, deBlendDarken, da, size.getN());

    deValue la = lightenAmount.get();
    blendChannel(secondStage, source, destination, NULL, deBlendLighten, la, size.getN());

    delete [] blurMap;
    delete [] firstStage;
    delete [] secondStage;
    
    logMessage("shadows/highlights end");
}


bool deShadowsHighlightsLayer::isChannelNeutral(int index)
{
    return false;
}    

void deShadowsHighlightsLayer::save(xmlNodePtr root)
{
    saveCommon(root);
    saveBlend(root);
    blurRadius.save(root);
}

void deShadowsHighlightsLayer::load(xmlNodePtr root)
{
    loadBlend(root);

    xmlNodePtr child = root->xmlChildrenNode;

    while (child)
    {
        blurRadius.load(child);

        child = child->next;
    }        
}


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

#include "dodge_burn_layer.h"
#include "project.h"
#include <iostream>
#include "blur.h"
#include "str.h"
#include "xml.h"
#include "frame_factory.h"
#include "copy_channel.h"
#include "blend_channel.h"
#include "process_linear.h"

deDodgeBurnLayer::deDodgeBurnLayer(deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deLayerProcessor& _layerProcessor, deChannelManager& _channelManager, deViewManager& _viewManager, const std::string& _name)
:deActionLayer(_name, _colorSpace, _index, _sourceLayer, _layerStack, _layerProcessor, _channelManager, _viewManager),
 blurRadius("blur_radius"),
 dodgeAmount("dodge_amount"),
 dodgeMin("dodge_min"),
 dodgeMax("dodge_max"),
 burnAmount("burn_amount"),
 burnMin("burn_min"),
 burnMax("burn_max")
{
    blurRadius.setLabel("radius");
    blurRadius.set(0.1);
}

deDodgeBurnLayer::~deDodgeBurnLayer()
{
}

void deDodgeBurnLayer::processAction(int i, const deChannel& sourceChannel, deChannel& channel, deSize size)
{
    const deValue* source = sourceChannel.getPixels();
    deValue* destination = channel.getPixels();

    deValue* blurMap = new deValue [size.getN()];
    deValue* dodgeMap = new deValue [size.getN()];
    deValue* burnMap = new deValue [size.getN()];
    deValue* firstStage = new deValue [size.getN()];

    deValue r = viewManager.getRealScale() * blurRadius.get() * 200;
    deBlurType type = deGaussianBlur;
    blurChannel(source, blurMap, size, r, type, 0.0);

    deValue dmin = 0.0;
    deValue dmax = 1.0;

    processLinear(blurMap, dodgeMap, size.getN(), dmin, dmax, false);

    deValue da = 1.0;   // dodge amount
    blendChannel(source, source, firstStage, dodgeMap, deBlendOverlay, da, size.getN());

    deValue bmin = 0.0;
    deValue bmax = 1.0;
    processLinear(blurMap, burnMap, size.getN(), bmin, bmax, true);

    deValue ba = 1.0;   // burn amount
    blendChannel(firstStage, source, destination, burnMap, deBlendMultiply, ba, size.getN());

    delete [] blurMap;
    delete [] dodgeMap;
    delete [] burnMap;
    delete [] firstStage;
}


bool deDodgeBurnLayer::isChannelNeutral(int index)
{
    return (blurRadius.get() == 0);
}    

void deDodgeBurnLayer::save(xmlNodePtr root)
{
    saveCommon(root);
    saveBlend(root);
    blurRadius.save(root);
}

void deDodgeBurnLayer::load(xmlNodePtr root)
{
    loadBlend(root);

    xmlNodePtr child = root->xmlChildrenNode;

    while (child)
    {
        blurRadius.load(child);

        child = child->next;
    }        
}


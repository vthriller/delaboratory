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
#include "layer_processor.h"

deDodgeBurnLayer::deDodgeBurnLayer(deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager, const std::string& _name)
:deActionLayer(_name, _colorSpace, _index, _sourceLayer, _layerStack, _channelManager, _viewManager),
 blurRadius("blur_radius"),
 alternate("alternate"),
 dodgeAmount("dodge_amount"),
 dodgeMin("dodge_min"),
 dodgeMax("dodge_max"),
 burnAmount("burn_amount"),
 burnMin("burn_min"),
 burnMax("burn_max")
{
    blurRadius.setLabel("radius");

    dodgeAmount.setLabel("amount");
    dodgeMin.setLabel("min");
    dodgeMax.setLabel("max");

    burnAmount.setLabel("amount");
    burnMin.setLabel("min");
    burnMax.setLabel("max");

    alternate.setLabel("use screen / multiply instead dodge / burn");

    reset();
}

void deDodgeBurnLayer::reset()
{
    blurRadius.set(0.1);
    dodgeAmount.set(0.4);
    dodgeMin.set(0.6);
    dodgeMax.set(0.95);
    burnAmount.set(0.4);
    burnMin.set(0.05);
    burnMax.set(0.4);
    alternate.set(false);
}

void deDodgeBurnLayer::setDodge(deValue v)
{
    dodgeAmount.set(v);
}

void deDodgeBurnLayer::setBurn(deValue v)
{
    burnAmount.set(v);
}

void deDodgeBurnLayer::setDodgeRange(deValue min, deValue max)
{
    dodgeMin.set(min);
    dodgeMax.set(max);
}

void deDodgeBurnLayer::setBurnRange(deValue min, deValue max)
{
    burnMin.set(min);
    burnMax.set(max);
}

deDodgeBurnLayer::~deDodgeBurnLayer()
{
}

void deDodgeBurnLayer::processAction(int i, const deChannel& sourceChannel, deChannel& channel, deSize size)
{
    logMessage("dodge/burn start");

    deBlendMode mode1 = deBlendDodge;
    deBlendMode mode2 = deBlendBurn;

    if (alternate.get())
    {
        mode1 = deBlendScreen;
        mode2 = deBlendMultiply;
    }

    const deValue* source = sourceChannel.getPixels();
    deValue* destination = channel.getPixels();

    deValue* blurMap = new deValue [size.getN()];
    deValue* dodgeMap = new deValue [size.getN()];
    deValue* burnMap = new deValue [size.getN()];
    deValue* firstStage = new deValue [size.getN()];

    deValue r = viewManager.getRealScale() * blurRadius.get() * 200;
    deBlurType type = deGaussianBlur;
    blurChannel(source, blurMap, size, r, type, 0.0);

    deValue dmin = dodgeMin.get();
    deValue dmax = dodgeMax.get();

    processLinear(blurMap, dodgeMap, size.getN(), dmin, dmax, false);

    deValue da = dodgeAmount.get(); 
    blendChannel(source, source, firstStage, dodgeMap, mode1, da, size.getN());

    deValue bmin = burnMin.get();
    deValue bmax = burnMax.get();
    processLinear(blurMap, burnMap, size.getN(), bmin, bmax, true);

    deValue ba = burnAmount.get(); 
    blendChannel(firstStage, firstStage, destination, burnMap, mode2, ba, size.getN());

    delete [] blurMap;
    delete [] dodgeMap;
    delete [] burnMap;
    delete [] firstStage;

    logMessage("dodge/burn end");
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
    alternate.save(root);
    dodgeAmount.save(root);
    dodgeMin.save(root);
    dodgeMax.save(root);
    burnAmount.save(root);
    burnMin.save(root);
    burnMax.save(root);
}

void deDodgeBurnLayer::load(xmlNodePtr root)
{
    loadBlend(root);

    xmlNodePtr child = root->xmlChildrenNode;

    while (child)
    {
        blurRadius.load(child);
        alternate.load(child);
        dodgeAmount.load(child);
        dodgeMin.load(child);
        dodgeMax.load(child);
        burnAmount.load(child);
        burnMin.load(child);
        burnMax.load(child);

        child = child->next;
    }        
}


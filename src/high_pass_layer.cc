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

#include "high_pass_layer.h"
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

deHighPassLayer::deHighPassLayer(deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deLayerProcessor& _layerProcessor, deChannelManager& _channelManager, deViewManager& _viewManager, const std::string& _name)
:deActionLayer(_name, _colorSpace, _index, _sourceLayer, _layerStack, _layerProcessor, _channelManager, _viewManager),
 blurRadius("blur_radius")
{
    blurRadius.setLabel("radius");
    reset();
}

void deHighPassLayer::reset()
{
    blurRadius.set(0.1);
    int index = getIndex();
    layerProcessor.markUpdateAllChannels(index);
}

deHighPassLayer::~deHighPassLayer()
{
}

void deHighPassLayer::processAction(int i, const deChannel& sourceChannel, deChannel& channel, deSize size)
{
    const deValue* source = sourceChannel.getPixels();
    deValue* destination = channel.getPixels();

    deValue* blurMap = new deValue [size.getN()];

    deValue r = viewManager.getRealScale() * blurRadius.get() * 200;
    deBlurType type = deGaussianBlur;
    blurChannel(source, blurMap, size, r, type, 0.0);

    blendChannel(source, blurMap, destination, NULL, deBlendAddInvert, 1.0, size.getN());

    delete [] blurMap;
}


bool deHighPassLayer::isChannelNeutral(int index)
{
    return (blurRadius.get() == 0);
}    

void deHighPassLayer::save(xmlNodePtr root)
{
    saveCommon(root);
    saveBlend(root);
    blurRadius.save(root);
}

void deHighPassLayer::load(xmlNodePtr root)
{
    loadBlend(root);

    xmlNodePtr child = root->xmlChildrenNode;

    while (child)
    {
        blurRadius.load(child);

        child = child->next;
    }        
}


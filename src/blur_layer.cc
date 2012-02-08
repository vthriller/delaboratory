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

#include "blur_layer.h"
#include "project.h"
#include <iostream>
#include "str.h"
#include "xml.h"
#include "frame_factory.h"
#include "layer_processor.h"

deBlurLayer::deBlurLayer(deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager, const std::string& _name)
:deActionLayer(_name, _colorSpace, _index, _sourceLayer, _layerStack, _channelManager, _viewManager),
 blurRadius("blur_radius"),
 threshold("threshold"),
 blurType("blur_type")
{
    blurRadius.setLabel("radius");
    blurRadius.setMin(0);
    blurRadius.setMax(50);
    getSupportedBlurTypes(blurType.getChoices());
    blurType.setLabel("blur type");
    reset();
}

deBlurLayer::~deBlurLayer()
{
}

void deBlurLayer::reset()
{
    blurRadius.set(5);
    blurType.set("gaussian");
}

bool deBlurLayer::processAction(int i, const deChannel& sourceChannel, deChannel& channel, deSize size)
{
    const deValue* source = sourceChannel.getPixels();
    deValue* destination = channel.getPixels();

    deValue r = viewManager.getRealScale() * blurRadius.get();

    deBlurType type = blurTypeFromString(blurType.get());
    
    logMessage("blur r=" + str(r));

    bool result = blurChannel(source, destination, size, r, r, type, threshold.get());

    logMessage("blur done");

    return result;
}

bool deBlurLayer::isChannelNeutral(int index)
{
    return (blurRadius.get() == 0);
}    

void deBlurLayer::save(xmlNodePtr root)
{
    saveCommon(root);
    saveBlend(root);

    blurRadius.save(root);
    blurType.save(root);
    threshold.save(root);
}

void deBlurLayer::load(xmlNodePtr root)
{
    loadBlend(root);

    xmlNodePtr child = root->xmlChildrenNode;

    while (child)
    {

        blurRadius.load(child);
        blurType.load(child);
        threshold.load(child);

        child = child->next;

    }        

}

bool deBlurLayer::randomize()
{
    deValue r = (deValue) rand() / RAND_MAX;

    r *= 0.5;

    blurRadius.set(r);

    return true;
}

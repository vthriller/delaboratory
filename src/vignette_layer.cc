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

#include "vignette_layer.h"
#include "vignette.h"
#include "project.h"
#include <iostream>
#include "str.h"
#include "xml.h"
#include "frame_factory.h"

deVignetteLayer::deVignetteLayer(deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deLayerProcessor& _layerProcessor, deChannelManager& _channelManager, deViewManager& _viewManager, const std::string& _name)
:deActionLayer(_name, _colorSpace, _index, _sourceLayer, _layerStack, _layerProcessor, _channelManager, _viewManager),
 radiusX("radius_x"),
 radiusY("radius_y"),
 centerX("center_x"),
 centerY("center_y")
{
    setBlendMode(deBlendOverlay);
    setOpacity(0.4);
}

deVignetteLayer::~deVignetteLayer()
{
}

void deVignetteLayer::processAction(int i, const deChannel& sourceChannel, deChannel& channel, deSize size)
{
    deValue* destination = channel.getPixels();

    vignetteChannel(destination, size, centerX.get(), centerY.get(), radiusX.get(), radiusY.get());
}

bool deVignetteLayer::isChannelNeutral(int index)
{
    return false;
}    

void deVignetteLayer::save(xmlNodePtr root)
{
/*
    saveCommon(root);
    saveBlend(root);

    blurRadius.save(root);
    blurType.save(root);
    threshold.save(root);
    */
}

void deVignetteLayer::load(xmlNodePtr root)
{
/*
    loadBlend(root);

    xmlNodePtr child = root->xmlChildrenNode;

    while (child)
    {

        blurRadius.load(child);
        blurType.load(child);
        threshold.load(child);

        child = child->next;

    }        
    */

}

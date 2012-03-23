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
#include "layer_processor.h"

deVignetteLayer::deVignetteLayer(deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager, const std::string& _name)
:deActionLayer(_name, _colorSpace, _index, _sourceLayer, _layerStack, _channelManager, _viewManager)
{
    lightIndex = registerPropertyValue("light");
    darknessIndex = registerPropertyValue("darkness");
    spotIndex = registerPropertyValue("spot");
    centerXIndex = registerPropertyValue("center_x");
    centerYIndex = registerPropertyValue("center_y");
    radiusXIndex = registerPropertyValue("radius_x");
    radiusYIndex = registerPropertyValue("radius_y");

    dePropertyValue* radiusX = valueProperties[radiusXIndex];
    dePropertyValue* radiusY = valueProperties[radiusYIndex];
    dePropertyValue* centerX = valueProperties[centerXIndex];
    dePropertyValue* centerY = valueProperties[centerYIndex];
    dePropertyValue* light = valueProperties[lightIndex];
    dePropertyValue* darkness = valueProperties[darknessIndex];
    dePropertyValue* spot = valueProperties[spotIndex];

    radiusX->setMin(0.01);
    radiusX->setMax(0.7);
    radiusY->setMin(0.01);
    radiusY->setMax(0.7);
    centerX->setMin(-1);
    centerY->setMin(-1);
    light->setMin(0.0);
    light->setMax(1.0);
    spot->setMin(0.0);
    spot->setMax(1.0);
    darkness->setMin(0.0);
    darkness->setMax(1.0);
    reset();

    disableNotLuminance();
}

deVignetteLayer::~deVignetteLayer()
{
}

bool deVignetteLayer::processAction(int i, const deChannel& sourceChannel, deChannel& channel, deSize size)
{
    logMessage("deVignetteLayer::processAction i=" + str(i));

    deValue* destination = channel.getPixels();

    if (!destination)
    {
        logMessage("ERROR channel pixels are NULL");
        return false;
    }

    deValue x1;
    deValue y1;
    deValue x2;
    deValue y2;

    viewManager.getZoom(x1, y1, x2, y2);

    deValue w = x2 - x1;
    deValue h = y2 - y1;

    dePropertyValue* radiusX = valueProperties[radiusXIndex];
    dePropertyValue* radiusY = valueProperties[radiusYIndex];
    dePropertyValue* centerX = valueProperties[centerXIndex];
    dePropertyValue* centerY = valueProperties[centerYIndex];
    dePropertyValue* light = valueProperties[lightIndex];
    dePropertyValue* darkness = valueProperties[darknessIndex];
    dePropertyValue* spot = valueProperties[spotIndex];

    deValue rx = radiusX->get() / w;
    deValue ry = radiusY->get() / h;

    // 0..1
    deValue ccx = (centerX->get() + 1.0) / 2.0;
    deValue ccy = (centerY->get() + 1.0) / 2.0;

    deValue cccx = (ccx - x1) / w;
    deValue cccy = (ccy - y1) / h;

    // -1..1
    deValue cx = cccx * 2.0 - 1.0;
    deValue cy = cccy * 2.0 - 1.0;

    vignetteChannel(destination, size, cx, cy, rx, ry, light->get(), darkness->get(), spot->get());

    logMessage("deVignetteLayer::processAction i=" + str(i) + " done");

    return true;
}

bool deVignetteLayer::isChannelNeutral(int index)
{
    return false;
}    

void deVignetteLayer::reset()
{
    dePropertyValue* radiusX = valueProperties[radiusXIndex];
    dePropertyValue* radiusY = valueProperties[radiusYIndex];
    dePropertyValue* centerX = valueProperties[centerXIndex];
    dePropertyValue* centerY = valueProperties[centerYIndex];
    dePropertyValue* light = valueProperties[lightIndex];
    dePropertyValue* darkness = valueProperties[darknessIndex];
    dePropertyValue* spot = valueProperties[spotIndex];

    setBlendMode(deBlendOverlay);
    setOpacity(1.0);
    radiusX->set(0.3);
    radiusY->set(0.3);
    centerX->set(0.0);
    centerY->set(0.0);
    light->set(0.5);
    spot->set(0.3);
    darkness->set(0.2);
}

void deVignetteLayer::save(xmlNodePtr root)
{
    saveCommon(root);
    saveBlend(root);
    saveValueProperties(root);
}

void deVignetteLayer::load(xmlNodePtr root)
{
    loadBlend(root);

    xmlNodePtr child = root->xmlChildrenNode;

    while (child)
    {
        loadValueProperties(child);
        child = child->next;
    }        

}

bool deVignetteLayer::randomize()
{
    dePropertyValue* radiusX = valueProperties[radiusXIndex];
    dePropertyValue* radiusY = valueProperties[radiusYIndex];
    dePropertyValue* centerX = valueProperties[centerXIndex];
    dePropertyValue* centerY = valueProperties[centerYIndex];
    dePropertyValue* light = valueProperties[lightIndex];
    dePropertyValue* darkness = valueProperties[darknessIndex];

    deValue r = (deValue) rand() / RAND_MAX;
    r *= 0.5;
    radiusX->set(r + 0.8);

    deValue r2 = (deValue) rand() / RAND_MAX;
    r2 *= 0.5;
    radiusY->set(r2 + 0.8);

    deValue r3 = (deValue) rand() / RAND_MAX;
    r3 *= 2.0;
    centerX->set(r3 - 1.0);

    deValue r4 = (deValue) rand() / RAND_MAX;
    r4 *= 2.0;
    centerY->set(r4 - 1.0);

    return true;
}

bool deVignetteLayer::setCenter(deValue x, deValue y)
{
    dePropertyValue* centerX = valueProperties[centerXIndex];
    dePropertyValue* centerY = valueProperties[centerYIndex];
    centerX->set(2 * x - 1);
    centerY->set(2 * y - 1);
}

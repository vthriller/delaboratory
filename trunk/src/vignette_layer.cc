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

deVignetteLayer1::deVignetteLayer1(deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager)
:deVignetteLayer(_colorSpace, _index, _sourceLayer, _layerStack, _channelManager, _viewManager, 1)
{
}


deVignetteLayer1::~deVignetteLayer1()
{
}

deVignetteLayer2::deVignetteLayer2(deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager)
:deVignetteLayer(_colorSpace, _index, _sourceLayer, _layerStack, _channelManager, _viewManager, 2)
{
}


deVignetteLayer2::~deVignetteLayer2()
{
}

deVignetteLayer::deVignetteLayer(deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager, int _vignettes)
:deActionLayer(_colorSpace, _index, _sourceLayer, _layerStack, _channelManager, _viewManager),
 vignettes(_vignettes)
{
    lightIndex = registerPropertyValue("light");
    darknessIndex = registerPropertyValue("darkness");
    spotIndex = registerPropertyValue("spot");

    center1XIndex = registerPropertyValue("center1_x");
    center1YIndex = registerPropertyValue("center1_y");
    radius1XIndex = registerPropertyValue("radius1_x");
    radius1YIndex = registerPropertyValue("radius1_y");

    if (vignettes == 2)
    {
        center2XIndex = registerPropertyValue("center2_x");
        center2YIndex = registerPropertyValue("center2_y");
        radius2XIndex = registerPropertyValue("radius2_x");
        radius2YIndex = registerPropertyValue("radius2_y");
    }

    dePropertyValue* radius1X = getPropertyValue(radius1XIndex);
    dePropertyValue* radius1Y = getPropertyValue(radius1YIndex);
    dePropertyValue* center1X = getPropertyValue(center1XIndex);
    dePropertyValue* center1Y = getPropertyValue(center1YIndex);

    dePropertyValue* light = getPropertyValue(lightIndex);
    dePropertyValue* darkness = getPropertyValue(darknessIndex);
    dePropertyValue* spot = getPropertyValue(spotIndex);

    radius1X->setMin(0.01);
    radius1X->setMax(1.0);
    radius1Y->setMin(0.01);
    radius1Y->setMax(1.0);
    center1X->setMin(-1);
    center1Y->setMin(-1);

    light->setMin(0.0);
    light->setMax(1.0);
    spot->setMin(0.0);
    spot->setMax(2.0);
    darkness->setMin(0.0);
    darkness->setMax(1.0);

    if (vignettes == 2)
    {
        dePropertyValue* radius2X = getPropertyValue(radius2XIndex);
        dePropertyValue* radius2Y = getPropertyValue(radius2YIndex);
        dePropertyValue* center2X = getPropertyValue(center2XIndex);
        dePropertyValue* center2Y = getPropertyValue(center2YIndex);
        radius2X->setMin(0.01);
        radius2X->setMax(1.0);
        radius2Y->setMin(0.01);
        radius2Y->setMax(1.0);
        center2X->setMin(-1);
        center2Y->setMin(-1);
    }

    reset();

    disableNotLuminance();
}

deVignetteLayer::~deVignetteLayer()
{
}

deEllipse calcEllipse(deValue radX, deValue radY, deValue cenX, deValue cenY, deValue x1, deValue y1, deValue x2, deValue y2)
{
    deValue w = x2 - x1;
    deValue h = y2 - y1;

    deValue rx = radX / w;
    deValue ry = radY / h;

    // 0..1
    deValue ccx = (cenX + 1.0) / 2.0;
    deValue ccy = (cenY + 1.0) / 2.0;

    deValue cccx = (ccx - x1) / w;
    deValue cccy = (ccy - y1) / h;

    // -1..1
    deValue cx = cccx * 2.0 - 1.0;
    deValue cy = cccy * 2.0 - 1.0;

    return deEllipse(cx, cy, rx, ry);
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

    dePropertyValue* radius1X = getPropertyValue(radius1XIndex);
    dePropertyValue* radius1Y = getPropertyValue(radius1YIndex);
    dePropertyValue* center1X = getPropertyValue(center1XIndex);
    dePropertyValue* center1Y = getPropertyValue(center1YIndex);

    dePropertyValue* light = getPropertyValue(lightIndex);
    dePropertyValue* darkness = getPropertyValue(darknessIndex);
    dePropertyValue* spot = getPropertyValue(spotIndex);

    deEllipse ellipse1 = calcEllipse(radius1X->get(), radius1Y->get(), center1X->get(), center1Y->get(), x1, y1, x2, y2);

    if (vignettes == 2)
    {
        dePropertyValue* radius2X = getPropertyValue(radius2XIndex);
        dePropertyValue* radius2Y = getPropertyValue(radius2YIndex);
        dePropertyValue* center2X = getPropertyValue(center2XIndex);
        dePropertyValue* center2Y = getPropertyValue(center2YIndex);

        deEllipse ellipse2 = calcEllipse(radius2X->get(), radius2Y->get(), center2X->get(), center2Y->get(), x1, y1, x2, y2);
        vignetteChannel(destination, size, ellipse1, ellipse2, light->get(), darkness->get(), spot->get());
    }
    else
    {
        vignetteChannel(destination, size, ellipse1, light->get(), darkness->get(), spot->get());
    }

    logMessage("deVignetteLayer::processAction i=" + str(i) + " done");

    return true;
}

bool deVignetteLayer::isChannelNeutral(int index)
{
    return false;
}    

void deVignetteLayer::reset()
{
    dePropertyValue* radius1X = getPropertyValue(radius1XIndex);
    dePropertyValue* radius1Y = getPropertyValue(radius1YIndex);
    dePropertyValue* center1X = getPropertyValue(center1XIndex);
    dePropertyValue* center1Y = getPropertyValue(center1YIndex);

    dePropertyValue* light = getPropertyValue(lightIndex);
    dePropertyValue* darkness = getPropertyValue(darknessIndex);
    dePropertyValue* spot = getPropertyValue(spotIndex);

    if (colorSpace == deColorSpaceCMYK)
    {
        setBlendMode(deBlendScreen);
    }
    else
    {
        setBlendMode(deBlendMultiply);
    }
    setOpacity(1.0);

    radius1X->set(0.5);
    radius1Y->set(0.5);
    center1X->set(0.0);
    center1Y->set(0.0);

    if (vignettes == 2)
    {
        dePropertyValue* radius2X = getPropertyValue(radius2XIndex);
        dePropertyValue* radius2Y = getPropertyValue(radius2YIndex);
        dePropertyValue* center2X = getPropertyValue(center2XIndex);
        dePropertyValue* center2Y = getPropertyValue(center2YIndex);

        radius2X->set(0.5);
        radius2Y->set(0.5);
        center2X->set(0.0);
        center2Y->set(0.0);
    }

    if (colorSpace == deColorSpaceCMYK)
    {
        light->set(0.2);
        darkness->set(1.0);
    }
    else
    {
        light->set(1.0);
        darkness->set(0.2);
    }
    spot->set(1.0);
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

bool deVignetteLayer::setCenter(deValue x, deValue y)
{
    dePropertyValue* center1X = getPropertyValue(center1XIndex);
    dePropertyValue* center1Y = getPropertyValue(center1YIndex);
    center1X->set(2 * x - 1);
    center1Y->set(2 * y - 1);

    return true;
}

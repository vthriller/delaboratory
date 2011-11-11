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

#include "usm_layer.h"
#include "project.h"
#include <iostream>
#include "blur.h"
#include "str.h"
#include "xml.h"
#include "frame_factory.h"

deUSMLayer::deUSMLayer(deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deLayerProcessor& _layerProcessor, deChannelManager& _channelManager, deViewManager& _viewManager, const std::string& _name)
:deActionLayer(_name, _colorSpace, _index, _sourceLayer, _layerStack, _layerProcessor, _channelManager, _viewManager),
 blurRadius(*this,"blur_radius"),
 amount(*this, "amount"),
 threshold(*this, "threshold")
{
    reset();
}

deUSMLayer::~deUSMLayer()
{
}

void deUSMLayer::processAction(int i, const deChannel& sourceChannel, deChannel& channel, deSize size)
{
    int n = size.getN();
    deValue* unsharpMask = new deValue[n];

    const deValue* source = sourceChannel.getPixels();
    deValue* destination = channel.getPixels();
    deBlurType type = deGaussianBlur;
    deValue t = 0.0;

    deValue r = viewManager.getRealScale() * blurRadius.get() * 1000;

    blurChannel(source, unsharpMask, size, r, type, t);

    for (i = 0; i < n; i ++)
    {
        deValue src = source[i];
        deValue u = src - unsharpMask[i];

        if (fabs(u) >= threshold.get())
        {
            deValue d = src + amount.get() * u;

            if (d < 0)
            {
                destination[i] = 0;
            }
            else if (d > 1)
            {
                destination[i] = 1;
            }
            else
            {    
                destination[i] = d;
            }               
        }
        else
        {
            destination[i] = src;
        }
    }

    delete unsharpMask;
}


bool deUSMLayer::isChannelNeutral(int index)
{
    return (blurRadius.get() == 0);
}    

void deUSMLayer::setBlurRadius(deValue r)
{
    blurRadius.set(r);
    updateImage();
    updateOtherLayers();
    repaint();
}

deValue deUSMLayer::getBlurRadius() const
{
    return blurRadius.get();
}

void deUSMLayer::setAmount(deValue r)
{
    amount.set(r);
    updateImage();
    updateOtherLayers();
    repaint();
}

deValue deUSMLayer::getAmount() const
{
    return amount.get();
}

void deUSMLayer::setThreshold(deValue r)
{
    threshold.set(r);
    updateImage();
    updateOtherLayers();
    repaint();
}

deValue deUSMLayer::getThreshold() const
{
    return threshold.get();
}

void deUSMLayer::reset()
{
    blurRadius.set(0.002);
    amount.set(2.0);
    threshold.set(0.0);
    updateImage();
    updateOtherLayers();
    repaint();
}

void deUSMLayer::sharp()
{
    blurRadius.set(0.001);
    amount.set(4.0);
    threshold.set(0.0);
    updateImage();
    updateOtherLayers();
    repaint();
}

void deUSMLayer::hiraloam1()
{
    blurRadius.set(0.8);
    amount.set(0.1);
    threshold.set(0.0);
    updateImage();
    updateOtherLayers();
    repaint();
}

void deUSMLayer::hiraloam2()
{
    blurRadius.set(0.8);
    amount.set(0.2);
    threshold.set(0.0);
    updateImage();
    updateOtherLayers();
    repaint();
}

void deUSMLayer::save(xmlNodePtr root)
{
    saveCommon(root);
    saveBlend(root);
    blurRadius.save(root);
    amount.save(root);
    threshold.save(root);
}

void deUSMLayer::load(xmlNodePtr root)
{
    loadBlend(root);

    xmlNodePtr child = root->xmlChildrenNode;

    while (child)
    {
        blurRadius.load(child);
        amount.load(child);
        threshold.load(child);

        child = child->next;
    }        
}


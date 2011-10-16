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
#include "usm_frame.h"
#include "project.h"
#include <iostream>
#include "blur.h"
#include "str.h"
#include "xml.h"

deUSMLayer::deUSMLayer(deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager, const std::string& _name)
:deActionLayer(_name, _colorSpace, _index, _sourceLayer, _layerStack, _channelManager, _viewManager) 
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

    blurChannel(source, unsharpMask, size, blurRadius, type, t);

    for (i = 0; i < n; i ++)
    {
        deValue src = source[i];
        deValue u = src - unsharpMask[i];

        if (fabs(u) >= threshold)
        {
            deValue d = src + amount * u;

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


void deUSMLayer::createActionFrame(wxWindow* parent)
{
    if (!actionFrame)
    {
        actionFrame = new deUSMFrame(parent, *this);
        actionFrame->Show(true);
    }        
}

bool deUSMLayer::isChannelNeutral(int index)
{
    return (blurRadius == 0);
}    

void deUSMLayer::setBlurRadius(deValue r)
{
    blurRadius = r;
    updateImage();
    updateOtherLayers();
    repaint();
}

deValue deUSMLayer::getBlurRadius() const
{
    return blurRadius;
}

void deUSMLayer::setAmount(deValue r)
{
    amount = r;
    updateImage();
    updateOtherLayers();
    repaint();
}

deValue deUSMLayer::getAmount() const
{
    return amount;
}

void deUSMLayer::setThreshold(deValue r)
{
    threshold = r;
    updateImage();
    updateOtherLayers();
    repaint();
}

deValue deUSMLayer::getThreshold() const
{
    return threshold;
}

void deUSMLayer::reset()
{
    blurRadius = 0.002;
    amount = 2.0;
    threshold = 0.0;
    updateImage();
    updateOtherLayers();
    repaint();
}

void deUSMLayer::sharp()
{
    blurRadius = 0.001;
    amount = 4.0;
    threshold = 0.0;
    updateImage();
    updateOtherLayers();
    repaint();
}

void deUSMLayer::hiraloam1()
{
    blurRadius = 0.1;
    amount = 0.1;
    threshold = 0.0;
    updateImage();
    updateOtherLayers();
    repaint();
}

void deUSMLayer::hiraloam2()
{
    blurRadius = 0.1;
    amount = 0.2;
    threshold = 0.0;
    updateImage();
    updateOtherLayers();
    repaint();
}

void deUSMLayer::save(xmlNodePtr root)
{
    saveCommon(root);
    saveChild(root, "radius", str(blurRadius));
    saveChild(root, "amount", str(amount));
    saveChild(root, "threshold", str(threshold));
};

void deUSMLayer::load(xmlNodePtr root)
{
    xmlNodePtr child = root->xmlChildrenNode;

    while (child)
    {

        if ((!xmlStrcmp(child->name, BAD_CAST("radius")))) 
        {
            blurRadius = getValue(getContent(child));
        }

        if ((!xmlStrcmp(child->name, BAD_CAST("amount")))) 
        {
            amount = getValue(getContent(child));
        }

        if ((!xmlStrcmp(child->name, BAD_CAST("threshold")))) 
        {
            threshold = getValue(getContent(child));
        }

        child = child->next;

    }        
}


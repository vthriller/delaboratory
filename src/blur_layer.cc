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
#include "blur_frame.h"
#include "str.h"
#include "xml.h"

deBlurLayer::deBlurLayer(deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager, const std::string& _name)
:deActionLayer(_name, _colorSpace, _index, _sourceLayer, _layerStack, _channelManager, _viewManager) 
{
    radius = 0.03;
    threshold = 0.0;
    type = deGaussianBlur;
}

deBlurLayer::~deBlurLayer()
{
}

void deBlurLayer::processAction(int i, const deChannel& sourceChannel, deChannel& channel, deSize size)
{
    const deValue* source = sourceChannel.getPixels();
    deValue* destination = channel.getPixels();

    blurChannel(source, destination, size, radius, type, threshold);
}


void deBlurLayer::createActionFrame(wxWindow* parent)
{
    if (!actionFrame)
    {
        actionFrame = new deBlurFrame(parent, *this);
        actionFrame->Show(true);
    }        
}

bool deBlurLayer::isChannelNeutral(int index)
{
    return (radius == 0);
}    

void deBlurLayer::setBlurRadius(deValue r)
{
    radius = r;
    updateImage();
    updateOtherLayers();
    repaint();
}

deValue deBlurLayer::getBlurRadius() const
{
    return radius;
}

void deBlurLayer::setBlurType(deBlurType t)
{
    type = t;
    updateImage();
    updateOtherLayers();
    repaint();
}

deBlurType deBlurLayer::getBlurType() const
{
    return type;
}


void deBlurLayer::setBlurThreshold(deValue r)
{
    threshold = r;
    updateImage();
    updateOtherLayers();
    repaint();
}

deValue deBlurLayer::getBlurThreshold() const
{
    return threshold;
}

void deBlurLayer::save(xmlNodePtr root)
{
    saveCommon(root);

    saveChild(root, "radius", str(radius));
};

void deBlurLayer::load(xmlNodePtr root)
{
    xmlNodePtr child = root->xmlChildrenNode;

    while (child)
    {

        if ((!xmlStrcmp(child->name, BAD_CAST("radius")))) 
        {
            radius = getValue(getContent(child));
        }

        child = child->next;

    }        

}

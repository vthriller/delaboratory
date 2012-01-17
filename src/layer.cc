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

#include "layer.h"
#include <sstream>
#include "xml.h"
#include "str.h"
#include "layer_processor.h"

deLayer::deLayer(const std::string& _name, deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerProcessor& _processor)
:name(_name), colorSpace(_colorSpace), index(_index), sourceLayer(_sourceLayer),
mutex(wxMUTEX_RECURSIVE),
layerProcessor(_processor)
{
    layerProcessor.log("creating layer " + str(index) + " " + name);
}

deLayer::~deLayer()
{
    layerProcessor.log("destroying layer " + str(index) + " " + name);
}

deColorSpace deLayer::getColorSpace() const
{
    return colorSpace;
}

std::string deLayer::getName() const
{
    return name;
}

bool deLayer::hasAction() const
{
    return false;
}

bool deLayer::hasBlending() const
{
    return false;
}

bool deLayer::canDisable() const
{
    return false;
}

bool deLayer::isEnabled() const
{
    return true;
}

void deLayer::setEnabled(bool e)
{
}

void deLayer::saveCommon(xmlNodePtr node)
{
    saveChild(node, "type", getType());
    saveChild(node, "name", name);
    saveChild(node, "index", str(index));
    saveChild(node, "source", str(sourceLayer));
    saveChild(node, "color_space", getColorSpaceName(colorSpace));
}

void deLayer::onUpdateProperties()
{
/*
    if (actionFrame)
    {
        actionFrame->onUpdateProperties();
    }
    if (blendFrame)
    {
        blendFrame->onUpdateProperties();
    }
    */
}

void deLayer::updateImageThreadCall()
{
    updateImage();
}

void deLayer::lock()
{
    //log("layer " + str(index) + " lock");
    mutex.Lock();
}

void deLayer::unlock()
{
    //log("layer " + str(index) + " unlock");
    mutex.Unlock();
}

void deLayer::process(deLayerProcessType type, int channel)
{
    switch (type)
    {
        case deLayerProcessFull:
        {
            processFull();
            break;
        }
        case deLayerProcessSingleChannel:
        {
            processChannel(channel);
            break;
        }
        case deLayerProcessBlend:
        {
            processBlend();
            break;
        }
    }
}

void deLayer::processFull()
{
    updateImageThreadCall();
}

void deLayer::processBlend()
{
}

void deLayer::processChannel(int channel)
{
}

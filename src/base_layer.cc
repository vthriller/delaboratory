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

#include "base_layer.h"
#include "xml.h"
#include "color_space_utils.h"
#include "logger.h"

deBaseLayer::deBaseLayer(deColorSpace _colorSpace)
:colorSpace(_colorSpace) 
{
}

deBaseLayer::~deBaseLayer()
{
}

deColorSpace deBaseLayer::getColorSpace() const
{
    return colorSpace;
}

void deBaseLayer::saveCommon(xmlNodePtr node)
{
    saveChild(node, "type", getType());
    //saveChild(node, "index", str(index));
    //saveChild(node, "source", str(sourceLayer));
    saveChild(node, "color_space", getColorSpaceName(colorSpace));
}

void deBaseLayer::lockLayer()
{
    lockWithLog(mutex, "layer mutex");
}

void deBaseLayer::unlockLayer()
{
    mutex.unlock();
}

void deBaseLayer::process(deLayerProcessType type, int channel)
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
        default:
            break;
    }
}

bool deBaseLayer::processFull()
{
    return updateImage();
}


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

#include "conversion_bw2hue_layer.h"
#include "layer_stack.h"
#include "layer_processor.h"
#include "convert_image.h"
#include "channel_manager.h"
#include "view_manager.h"
#include "frame_factory.h"
#include "blend_mode.h"
#include "layer_processor.h"
#include "logger.h"

deConversionBW2HueLayer::deConversionBW2HueLayer(int _index, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deColorSpace _colorSpace)
:deConversionLayer(_colorSpace, _index, _sourceLayer, _layerStack, _channelManager),
hue("hue"), saturation("saturation")
{
    reset();
}

deConversionBW2HueLayer::~deConversionBW2HueLayer()
{
}

void deConversionBW2HueLayer::reset()
{
    hue.set(0.0);
    saturation.set(0.00);
}

void deConversionBW2HueLayer::preset(deValue h)
{
    hue.set(h);
    saturation.set(0.04);
}

void fill(deValue* pixels, int n, deValue v)
{
    int i;
    for (i = 0; i < n; i++)
    {
        pixels[i] = v;
    }
}

bool deConversionBW2HueLayer::updateImage()
{
    logMessage("conversion start");

    deLayer* source = layerStack.getLayer(sourceLayer);
    const deImage& sourceImage = source->getImage();

    image.enableAllChannels();
    convertImage(sourceImage, image, channelManager);

    deChannel* dc0 = channelManager.getChannel(image.getChannelIndex(0));
    deChannel* dc1 = channelManager.getChannel(image.getChannelIndex(1));
    deChannel* dc2 = channelManager.getChannel(image.getChannelIndex(2));

    deValue* p0 = dc0->getPixels();
    deValue* p1 = dc1->getPixels();
    deValue* p2 = dc2->getPixels();

    int n = channelManager.getChannelSize().getN();

    switch (colorSpace)
    {
        case deColorSpaceHSL:
        {
            fill(p0, n, hue.get());
            fill(p1, n, saturation.get());
            break;
        }
        case deColorSpaceHSV:
        {
            fill(p0, n, hue.get());
            fill(p1, n, saturation.get());
            break;
        }
        case deColorSpaceLCH:
        {
            fill(p2, n, hue.get());
            fill(p1, n, saturation.get());
            break;
        }
        default:
            break;
    }

    logMessage("conversion end");

    return true;
}

void deConversionBW2HueLayer::save(xmlNodePtr root)
{
    saveCommon(root);
    hue.save(root);
    saturation.save(root);
}

void deConversionBW2HueLayer::load(xmlNodePtr root)
{
    xmlNodePtr child = root->xmlChildrenNode;

    while (child)
    {
        hue.load(child);
        saturation.load(child);

        child = child->next;
    }        
}

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

#include "conversion_bw_layer.h"
#include "layer_stack.h"
#include "layer_processor.h"
#include "convert_image.h"
#include "channel_manager.h"
#include "view_manager.h"
#include "frame_factory.h"
#include "blend_mode.h"
#include "layer_processor.h"
#include "logger.h"

deConversionBWLayer::deConversionBWLayer(int _index, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager, int n)
:deConversionLayer(deColorSpaceBW, _index, _sourceLayer, _layerStack, _channelManager),
 add0("add_0"),
 add1("add_1"),
 add2("add_2"),
 add3("add_3"),
 overlay0("overlay_0"),
 overlay1("overlay_1"),
 overlay2("overlay_2"),
 overlay3("overlay_3"),
 viewManager(_viewManager)
{
    resetM();

    add0.setMin(-1.0);
    add0.setMax(3.0);
    add1.setMin(-1.0);
    add1.setMax(3.0);
    add2.setMin(-1.0);
    add2.setMax(3.0);

    deLayer* source = layerStack.getLayer(sourceLayer);
    deColorSpace sourceColorSpace = source->getColorSpace();

    add0.setLabel(getChannelName(sourceColorSpace, 0));
    add1.setLabel(getChannelName(sourceColorSpace, 1));
    add2.setLabel(getChannelName(sourceColorSpace, 2));

    overlay0.setLabel(getChannelName(sourceColorSpace, 0));
    overlay1.setLabel(getChannelName(sourceColorSpace, 1));
    overlay2.setLabel(getChannelName(sourceColorSpace, 2));
}

deConversionBWLayer::~deConversionBWLayer()
{
}

void deConversionBWLayer::resetM()
{
    add0.set(0.3);
    add1.set(0.6);
    add2.set(0.1);
}

void deConversionBWLayer::presetM(int c)
{
    add0.set(0);
    add1.set(0);
    add2.set(0);
    if (c == 0)
    {
        add0.set(1.0);
    }
    if (c == 1)
    {
        add1.set(1.0);
    }
    if (c == 2)
    {
        add2.set(1.0);
    }
}

void deConversionBWLayer::updateImage()
{
    logMessage("conversion BW start");

    deLayer* source = layerStack.getLayer(sourceLayer);
    const deImage& sourceImage = source->getImage();

    image.enableAllChannels();

    int n = channelManager.getChannelSize().getN();

    deChannel* sc0 = NULL;
    deChannel* sc1 = NULL;
    deChannel* sc2 = NULL;

    sc0 = channelManager.getChannel(sourceImage.getChannelIndex(0));
    sc1 = channelManager.getChannel(sourceImage.getChannelIndex(1));
    sc2 = channelManager.getChannel(sourceImage.getChannelIndex(2));

    if (!sc0)
    {
        return;
    }
    if (!sc1)
    {
        return;
    }
    if (!sc2)
    {
        return;
    }

    deChannel* dc = channelManager.getChannel(image.getChannelIndex(0));

    if (!dc)
    {
        return;
    }

    dc->lockWrite();

    deValue* d = dc->getPixels();

    sc0->lockRead();
    sc1->lockRead();
    sc2->lockRead();

    deValue* v0 = NULL;
    deValue* v1 = NULL;
    deValue* v2 = NULL;

    if (sc0)
    {
        v0 = sc0->getPixels();
    }
    if (sc1)
    {
        v1 = sc1->getPixels();
    }
    if (sc2)
    {
        v2 = sc2->getPixels();
    }

    deValue a0 = add0.get();
    deValue a1 = add1.get();
    deValue a2 = add2.get();
    deValue o0 = overlay0.get();
    deValue o1 = overlay1.get();
    deValue o2 = overlay2.get();

    int i;
    for (i = 0; i < n; i++)
    {
        deValue result = 0.0;

        result += v0[i] * a0;
        result += v1[i] * a1;
        result += v2[i] * a2;

        if (result < 0)
        {
            result = 0;
        }

        if (result > 1)
        {
            result = 1;
        }

        deValue blend0 = calcBlendResult(result, v0[i], deBlendOverlay);
        deValue blend1 = calcBlendResult(result, v1[i], deBlendOverlay);
        deValue blend2 = calcBlendResult(result, v2[i], deBlendOverlay);

        result = result * (1 - o0) + blend0 * o0;
        result = result * (1 - o1) + blend1 * o1;
        result = result * (1 - o2) + blend2 * o2;

        if (result < 0)
        {
            result = 0;
        }

        if (result > 1)
        {
            result = 1;
        }

        d[i] = result;
    }

    sc0->unlockRead();
    sc1->unlockRead();
    sc2->unlockRead();
    dc->unlockWrite();

    logMessage("conversion BW end");
}

deColorSpace deConversionBWLayer::getSourceColorSpace() const
{
    deLayer* source = layerStack.getLayer(sourceLayer);
    return source->getColorSpace();
}


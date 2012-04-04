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

deConversionBWLayer::deConversionBWLayer(int _index, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, int n)
:deConversionLayer(deColorSpaceBW, _index, _sourceLayer, _layerStack, _channelManager),
 add0("add_0"),
 add1("add_1"),
 add2("add_2"),
 add3("add_3"),
 overlay0("overlay_0"),
 overlay1("overlay_1"),
 overlay2("overlay_2"),
 overlay3("overlay_3")
{
    add0.setMin(-1.0);
    add0.setMax(3.0);
    add1.setMin(-1.0);
    add1.setMax(3.0);
    add2.setMin(-1.0);
    add2.setMax(3.0);

    deLayer* source = layerStack.getLayer(sourceLayer);
    deColorSpace sourceColorSpace = source->getColorSpace();
    int cs = getColorSpaceSize(sourceColorSpace);

    resetM();

    add0.setLabel(getChannelName(sourceColorSpace, 0));
    add1.setLabel(getChannelName(sourceColorSpace, 1));
    add2.setLabel(getChannelName(sourceColorSpace, 2));

    overlay0.setLabel(getChannelName(sourceColorSpace, 0));
    overlay1.setLabel(getChannelName(sourceColorSpace, 1));
    overlay2.setLabel(getChannelName(sourceColorSpace, 2));

    if (cs == 4)
    {
        add3.setMin(-1.0);
        add3.setMax(3.0);
        add3.setLabel(getChannelName(sourceColorSpace, 3));
        overlay3.setLabel(getChannelName(sourceColorSpace, 3));
    }        

    if (sourceColorSpace == deColorSpaceRGB)
    {
        // http://www.markushartel.com/blog/learn-from-markus/channel-mixer-settings
        // http://www.darktable.org/usermanual/ch03s04s36.html.php
        // http://www.pbase.com/digitalrebel/bw_film_types
        // http://www.flickr.com/groups/blackwhite/discuss/72157594354861228/
        // http://oxle.com/topic.asp?archive=true&tid=5243
        // http://keith_towers.webs.com/channelmixersettings.htm
        films.push_back(deFilm("AGFA 200X", 18, 41, 41));
        films.push_back(deFilm("Agfapan 25", 25, 39, 36));
        films.push_back(deFilm("Agfapan 100", 21, 40, 39));
        films.push_back(deFilm("Agfapan 400", 20, 41, 39));
        films.push_back(deFilm("Ilford Delta 100", 21, 42, 37));
        films.push_back(deFilm("Ilford Delta 400", 22, 42, 36));
        films.push_back(deFilm("Ilford Delta 3200", 31, 36, 33));
        films.push_back(deFilm("Ilford FP4", 28, 41, 31));
        films.push_back(deFilm("Ilford HP5", 23, 37, 40));
        films.push_back(deFilm("Ilford Pan F", 33, 36, 31));
        films.push_back(deFilm("Ilford SFX", 36, 31, 33));
        films.push_back(deFilm("Ilford XP2 Super", 21, 42, 37));
        films.push_back(deFilm("Kodak T-Max 100", 24, 37, 39));
        films.push_back(deFilm("Kodak T-Max 400", 27, 36, 37));
        films.push_back(deFilm("Kodak Tri-X 400", 25, 35, 40));
    }
}

deConversionBWLayer::~deConversionBWLayer()
{
}

void deConversionBWLayer::resetM()
{
    deLayer* source = layerStack.getLayer(sourceLayer);
    deColorSpace sourceColorSpace = source->getColorSpace();
    switch (sourceColorSpace)
    {
        case deColorSpaceRGB:
        case deColorSpaceProPhoto:
        {
            add0.set(0.3);
            add1.set(0.6);
            add2.set(0.1);
            add3.set(0.0);
            break;
        }            
        case deColorSpaceCMYK:
        {
            add0.set(0.0);
            add1.set(0.0);
            add2.set(0.0);
            add3.set(1.0);
            break;
        }            
        case deColorSpaceLAB:
        case deColorSpaceLCH:
        {
            add0.set(1.0);
            add1.set(0.0);
            add2.set(0.0);
            add3.set(0.0);
            break;
        }
        case deColorSpaceHSV:
        case deColorSpaceHSL:
        {
            add0.set(0.0);
            add1.set(0.0);
            add2.set(1.0);
            add3.set(0.0);
            break;
        }
        default:
        {
            add0.set(0.3);
            add1.set(0.3);
            add2.set(0.3);
            add3.set(0.0);
            break;
        }
    }      
    overlay0.set(0);
    overlay1.set(0);
    overlay2.set(0);
    overlay3.set(0);
}

void deConversionBWLayer::presetM(int c)
{
    add0.set(0);
    add1.set(0);
    add2.set(0);
    add3.set(0);
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
    if (c == 3)
    {
        add3.set(1.0);
    }
}

bool deConversionBWLayer::updateImage()
{
    logMessage("conversion BW start");

    deLayer* source = layerStack.getLayer(sourceLayer);
    const deImage& sourceImage = source->getImage();

    deColorSpace sourceColorSpace = source->getColorSpace();
    int cs = getColorSpaceSize(sourceColorSpace);

    image.enableAllChannels();

    int n = channelManager.getChannelSize().getN();

    deChannel* sc0 = NULL;
    deChannel* sc1 = NULL;
    deChannel* sc2 = NULL;
    deChannel* sc3 = NULL;

    sc0 = channelManager.getChannel(sourceImage.getChannelIndex(0));
    sc1 = channelManager.getChannel(sourceImage.getChannelIndex(1));
    sc2 = channelManager.getChannel(sourceImage.getChannelIndex(2));

    if (!sc0)
    {
        return false;
    }
    if (!sc1)
    {
        return false;
    }
    if (!sc2)
    {
        return false;
    }

    if (cs == 4)
    {
        sc3 = channelManager.getChannel(sourceImage.getChannelIndex(3));
        if (!sc3)
        {
            return false;
        }
    }        

    deChannel* dc = channelManager.getChannel(image.getChannelIndex(0));

    if (!dc)
    {
        return false;
    }

    dc->lockWrite();

    deValue* d = dc->getPixels();

    sc0->lockRead();
    sc1->lockRead();
    sc2->lockRead();
    if (sc3)
    {
        sc3->lockRead();
    }

    deValue* v0 = NULL;
    deValue* v1 = NULL;
    deValue* v2 = NULL;
    deValue* v3 = NULL;

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
    if (sc3)
    {
        v3 = sc3->getPixels();
    }

    deValue a0 = add0.get();
    deValue a1 = add1.get();
    deValue a2 = add2.get();
    deValue a3 = add3.get();
    deValue o0 = overlay0.get();
    deValue o1 = overlay1.get();
    deValue o2 = overlay2.get();
    deValue o3 = overlay3.get();

    int i;
    for (i = 0; i < n; i++)
    {
        deValue result = 0.0;

        result += v0[i] * a0;
        result += v1[i] * a1;
        result += v2[i] * a2;
        if (v3)
        {
            result += v3[i] * a3;
        }

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
        deValue blend3 = 0;
        if (v3)
        {
            blend3 = calcBlendResult(result, v3[i], deBlendOverlay);
        }

        result = result * (1 - o0) + blend0 * o0;
        result = result * (1 - o1) + blend1 * o1;
        result = result * (1 - o2) + blend2 * o2;
        if (v3)
        {
            result = result * (1 - o3) + blend3 * o3;
        }

        if (sourceColorSpace == deColorSpaceCMYK)
        {
            result = 1 - result;
        }

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
    if (sc3)
    {
        sc3->unlockRead();
    }
    dc->unlockWrite();

    logMessage("conversion BW end");

    return true;
}

deColorSpace deConversionBWLayer::getSourceColorSpace() const
{
    deLayer* source = layerStack.getLayer(sourceLayer);
    return source->getColorSpace();
}

void deConversionBWLayer::save(xmlNodePtr root)
{
    saveCommon(root);
    add0.save(root);
    add1.save(root);
    add2.save(root);
    add3.save(root);
    overlay0.save(root);
    overlay1.save(root);
    overlay2.save(root);
    overlay3.save(root);
}

void deConversionBWLayer::load(xmlNodePtr root)
{
    xmlNodePtr child = root->xmlChildrenNode;

    while (child)
    {
        add0.load(child);
        add1.load(child);
        add2.load(child);
        add3.load(child);
        overlay0.load(child);
        overlay1.load(child);
        overlay2.load(child);
        overlay3.load(child);

        child = child->next;
    }        
}

void deConversionBWLayer::applyFilm(int f)
{
    if (f < 0)
    {
        return;
    }
    if ((unsigned int)f >= films.size())
    {
        return;
    }
    deFilm& film = films[f];

    add0.set(film.getR());
    add1.set(film.getG());
    add2.set(film.getB());

    add3.set(0.0);
    overlay0.set(0);
    overlay1.set(0);
    overlay2.set(0);
    overlay3.set(0);

}

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
#include "layer_processor.h"

deUSMLayer::deUSMLayer(deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager, const std::string& _name)
:deActionLayer(_name, _colorSpace, _index, _sourceLayer, _layerStack, _channelManager, _viewManager),
 blurRadius("blur_radius"),
 amount("amount"),
 threshold("threshold")
{
    blurRadius.setMin(1);
    blurRadius.setMax(200);
    reset();
    amount.setMax(5);
    blurRadius.setLabel("radius");
}

deUSMLayer::~deUSMLayer()
{
}

bool deUSMLayer::processAction(int i, const deChannel& sourceChannel, deChannel& channel, deSize size)
{
    logMessage("usm start");

    int n = size.getN();

    deValue* unsharpMask = NULL;
    try
    {
        unsharpMask = new deValue [size.getN()];
    }
    catch (std::bad_alloc)
    {
        logMessage("ERROR allocating memory in high pass");
        if (unsharpMask)
        {
            delete [] unsharpMask;
        }
        return false;
    }

    const deValue* source = sourceChannel.getPixels();
    deValue* destination = channel.getPixels();
    deBlurType type = deGaussianBlur;
    deValue t = 0.0;

    deValue r = viewManager.getRealScale() * blurRadius.get();

    bool result = blurChannel(source, unsharpMask, size, r, r, type, t);

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

    delete [] unsharpMask;

    logMessage("usm end");

    return result;
}


bool deUSMLayer::isChannelNeutral(int index)
{
    return (blurRadius.get() == 0);
}    

void deUSMLayer::reset()
{
    blurRadius.set(5);
    amount.set(0.1);
    threshold.set(0.0);
}

void deUSMLayer::sharp()
{
    blurRadius.set(3);
    amount.set(3.0);
    threshold.set(0.0);
}

void deUSMLayer::hiraloam1()
{
    blurRadius.set(100);
    amount.set(0.1);
    threshold.set(0.0);
}

void deUSMLayer::hiraloam2()
{
    blurRadius.set(150);
    amount.set(0.2);
    threshold.set(0.0);
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

bool deUSMLayer::randomize()
{
    deValue r = (deValue) rand() / RAND_MAX;
    r *= 0.5;
    blurRadius.set(r + 0.001);

    deValue r2 = (deValue) rand() / RAND_MAX;
    r2 *= 5;
    amount.set(r2);

    deValue r3 = (deValue) rand() / RAND_MAX;
    r3 *= 0.2;
    threshold.set(r3);

    return true;
}

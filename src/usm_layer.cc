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
#include "preset.h"

deUSMLayer::deUSMLayer(deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager, const std::string& _name)
:deActionLayer(_name, _colorSpace, _index, _sourceLayer, _layerStack, _channelManager, _viewManager)
{
    blurRadiusPropertyIndex = registerPropertyValue("blur_radius");
    amountPropertyIndex = registerPropertyValue("amount");
    thresholdPropertyIndex = registerPropertyValue("threshold");

    {
        dePresetLayer* p = new dePresetLayer(*this);
        p->addPresetValue(new dePresetValue("blur_radius", 5.0));
        p->addPresetValue(new dePresetValue("amount", 0.1));
        p->addPresetValue(new dePresetValue("threshold", 0.0));
        presets["reset"] = p;
    }        
    {
        dePresetLayer* p = new dePresetLayer(*this);
        p->addPresetValue(new dePresetValue("blur_radius", 3.0));
        p->addPresetValue(new dePresetValue("amount", 3.0));
        p->addPresetValue(new dePresetValue("threshold", 0.0));
        presets["sharp"] = p;
    }        
    {
        dePresetLayer* p = new dePresetLayer(*this);
        p->addPresetValue(new dePresetValue("blur_radius", 100));
        p->addPresetValue(new dePresetValue("amount", 0.1));
        p->addPresetValue(new dePresetValue("threshold", 0.0));
        presets["hiraloam"] = p;
    }        
    {
        dePresetLayer* p = new dePresetLayer(*this);
        p->addPresetValue(new dePresetValue("blur_radius", 150));
        p->addPresetValue(new dePresetValue("amount", 0.2));
        p->addPresetValue(new dePresetValue("threshold", 0.0));
        presets["hiraloam2"] = p;
    }        

    dePropertyValue* blurRadius = valueProperties[blurRadiusPropertyIndex];
    dePropertyValue* amount = valueProperties[amountPropertyIndex];

    blurRadius->setMin(1);
    blurRadius->setMax(200);
    applyPreset("reset");
    amount->setMax(5);
    blurRadius->setLabel("radius");
}

deUSMLayer::~deUSMLayer()
{
    while (presets.size() > 0)
    {
        std::map<std::string, dePresetLayer*>::iterator i = presets.begin();
        delete i->second;
        presets.erase(i);
    }
}

bool deUSMLayer::processAction(int index, const deChannel& sourceChannel, deChannel& channel, deSize size)
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
        logMessage("ERROR allocating memory in USM");
        if (unsharpMask)
        {
            delete [] unsharpMask;
        }
        return false;
    }

    dePropertyValue* blurRadius = valueProperties[blurRadiusPropertyIndex];
    dePropertyValue* amount = valueProperties[amountPropertyIndex];
    dePropertyValue* threshold = valueProperties[thresholdPropertyIndex];

    const deValue* source = sourceChannel.getPixels();
    deValue* destination = channel.getPixels();
    deBlurType type = deGaussianBlur;
    deValue b_t = 0.0;

    deValue r = viewManager.getRealScale() * blurRadius->get();

    bool result = blurChannel(source, unsharpMask, size, r, r, type, b_t);

    int i;

    deValue t = threshold->get();
    deValue a = amount->get();

    if (t > 0)
    {
        for (i = 0; i < n; i++)
        {
            deValue src = source[i];
            deValue u = src - unsharpMask[i];

            if (fabs(u) >= t)
            {
                deValue d = src + a * u;

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
    }
    else
    {
        for (i = 0; i < n; i++)
        {
            deValue src = source[i];
            deValue u = src - unsharpMask[i];

            deValue d = src + a * u;

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
    }


    delete [] unsharpMask;

    logMessage("usm end");

    return result;
}


bool deUSMLayer::isChannelNeutral(int index)
{
    dePropertyValue* blurRadius = valueProperties[blurRadiusPropertyIndex];
    return (blurRadius->get() == 0);
}    

void deUSMLayer::save(xmlNodePtr root)
{
    saveCommon(root);
    saveBlend(root);
    saveValueProperties(root);
}

void deUSMLayer::load(xmlNodePtr root)
{
    loadBlend(root);

    xmlNodePtr child = root->xmlChildrenNode;

    while (child)
    {
        loadValueProperties(child);

        child = child->next;
    }        
}

bool deUSMLayer::randomize()
{
    dePropertyValue* blurRadius = valueProperties[blurRadiusPropertyIndex];
    dePropertyValue* amount = valueProperties[amountPropertyIndex];
    dePropertyValue* threshold = valueProperties[thresholdPropertyIndex];

    deValue r = (deValue) rand() / RAND_MAX;
    r *= 0.5;
    blurRadius->set(r + 0.001);

    deValue r2 = (deValue) rand() / RAND_MAX;
    r2 *= 5;
    amount->set(r2);

    deValue r3 = (deValue) rand() / RAND_MAX;
    r3 *= 0.2;
    threshold->set(r3);

    return true;
}

bool deUSMLayer::applyPreset(const std::string& _name)
{
    std::map<std::string, dePresetLayer*>::iterator i = presets.find(_name);

    if (i == presets.end())
    {
        return false;
    }

    dePresetLayer* preset = i->second;

    preset->apply();
            
}

void deUSMLayer::getPresets(std::vector<std::string>& result)
{
    std::map<std::string, dePresetLayer*>::iterator i;
    for (i = presets.begin(); i != presets.end(); i++)
    {
        std::string n = i->first;
        result.push_back(n);
    }
}


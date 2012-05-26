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
#include "color_space_utils.h"
#include "logger.h"
#include "semaphore.h"
#include "str.h"
#include "property_numeric.h"
#include "property_boolean.h"
#include "property_choice.h"
#include "preset.h"

class deUpdateActionThread:public wxThread
{
    private:
        virtual void *Entry()
        {
            bool result = layer.updateMainImageSingleChannel(channel);
            if (!result)
            {
                logError("update action failed");
                layer.setErrorOnUpdateFromThread();
            }
            semaphore.post();
            return NULL;
        }
        deBaseLayer& layer;
        int channel;
        deSemaphore& semaphore;
    public:    
        deUpdateActionThread(deBaseLayer& _layer, int _channel, deSemaphore& _semaphore)
        :layer(_layer),
         channel(_channel),
         semaphore(_semaphore)
        {
        }
        virtual ~deUpdateActionThread()
        {
        }
};


deBaseLayer::deBaseLayer(deColorSpace _colorSpace, deChannelManager& _channelManager)
:colorSpace(_colorSpace),
 channelManager(_channelManager),
 mainLayerImage(_colorSpace, _channelManager)
{
    warning = "OK";
}

deBaseLayer::~deBaseLayer()
{
    std::vector<deProperty*>::iterator i;
    while (properties.size() > 0)
    {
        i = properties.begin();
        delete *i;
        properties.erase(i);
    }
    std::vector<dePreset*>::iterator j;
    while (presets.size() > 0)
    {
        j = presets.begin();
        delete *j;
        presets.erase(j);
    }
}

deColorSpace deBaseLayer::getColorSpace() const
{
    return colorSpace;
}

void deBaseLayer::lockLayer()
{
    mutex.lock();
}

void deBaseLayer::unlockLayer()
{
    mutex.unlock();
}

void deBaseLayer::processLayer(deLayerProcessType type, int channel)
{
    logInfo("processLayer channel: " + str(channel));
    switch (type)
    {
        case deLayerProcessFull:
        {
            processFull();
            break;
        }
        case deLayerProcessSingleChannel:
        {
            processSingleChannel(channel);
            break;
        }
        case deLayerProcessBlend:
        {
            updateBlendAllChannels();
            break;
        }
        default:
            break;
    }
    logInfo("processLayer DONE");
}

bool deBaseLayer::processFull()
{
    return updateImage();
}

const deImage& deBaseLayer::getLayerImage() const
{
    return mainLayerImage;
}

void deBaseLayer::updateChannelUsage(std::map<int, int>& channelUsage, int layerIndex) const
{
    mainLayerImage.updateChannelUsage(channelUsage, layerIndex);
}

void deBaseLayer::setErrorOnUpdateFromThread()
{
    errorOnUpdate = true;
}

bool deBaseLayer::updateMainImageAllChannels()
{
    logInfo("update action all channels start");

    int n = getColorSpaceSize(colorSpace);
    int i;

    deSemaphore semaphore(0, n);

    errorOnUpdate = false;

    for (i = 0; i < n; i++)
    {
        logInfo("creating update action thread for channel " + str(i));
        deUpdateActionThread* thread = new deUpdateActionThread(*this, i, semaphore);

        if ( thread->Create() != wxTHREAD_NO_ERROR )
        {
        }

        if ( thread->Run() != wxTHREAD_NO_ERROR )
        {
        }
    }

    for (i = 0; i < n; i++)
    {
        logInfo("waiting for update action thread for channel " + str(i));
        semaphore.wait();
    }

    logInfo("update action all channels DONE");

    if (errorOnUpdate)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void deBaseLayer::processSingleChannel(int channel)
{
    logInfo("base layer process single channel");
    updateMainImageSingleChannel(channel);
    logInfo("base layer process single channel DONE");
}    

bool deBaseLayer::updateImage()
{
    bool result = updateMainImageNotThreadedWay();
    if (!result)
    {
        result = updateMainImageAllChannels();
    }

    return result;
}

void deBaseLayer::createPropertyNumeric(const std::string& _name, deValue _min, deValue _max)
{
    std::vector<deProperty*>::iterator i;
    for (i = properties.begin(); i != properties.end(); i++)
    {
        deProperty* property = *i;
        if (property->getName() == _name)
        {
            return;
        }
    }
    properties.push_back(new dePropertyNumeric(_name, _min, _max));
}

void deBaseLayer::createPropertyChoice(const std::string& _name, const std::vector<std::string>& _choices)
{
    std::vector<deProperty*>::iterator i;
    for (i = properties.begin(); i != properties.end(); i++)
    {
        deProperty* property = *i;
        if (property->getName() == _name)
        {
            return;
        }
    }
    properties.push_back(new dePropertyChoice(_name, _choices));
}

void deBaseLayer::createPropertyBoolean(const std::string& _name)
{
    std::vector<deProperty*>::iterator i;
    for (i = properties.begin(); i != properties.end(); i++)
    {
        deProperty* property = *i;
        if (property->getName() == _name)
        {
            return;
        }
    }
    properties.push_back(new dePropertyBoolean(_name));
}

deProperty* deBaseLayer::getProperty(const std::string& _name)
{
    std::vector<deProperty*>::iterator i;
    for (i = properties.begin(); i != properties.end(); i++)
    {
        deProperty* property = *i;
        if (property->getName() == _name)
        {
            return property;
        }
    }
    logError("property " + _name + " not found");

    return NULL;
}

const deProperty* deBaseLayer::getProperty(const std::string& _name) const
{
    std::vector<deProperty*>::const_iterator i;
    for (i = properties.begin(); i != properties.end(); i++)
    {
        deProperty* property = *i;
        if (property->getName() == _name)
        {
            return property;
        }
    }

    return NULL;
}

void deBaseLayer::getProperties(std::vector<std::string>& names)
{
    std::vector<deProperty*>::iterator i;
    for (i = properties.begin(); i != properties.end(); i++)
    {
        deProperty* property = *i;
        names.push_back(property->getName());
    }

}

dePropertyNumeric* deBaseLayer::getPropertyNumeric(const std::string& _name)
{
    return dynamic_cast<dePropertyNumeric*>(getProperty(_name));
}

const dePropertyNumeric* deBaseLayer::getPropertyNumeric(const std::string& _name) const
{
    return dynamic_cast<const dePropertyNumeric*>(getProperty(_name));
}

dePropertyChoice* deBaseLayer::getPropertyChoice(const std::string& _name)
{
    return dynamic_cast<dePropertyChoice*>(getProperty(_name));
}

const dePropertyChoice* deBaseLayer::getPropertyChoice(const std::string& _name) const
{
    return dynamic_cast<const dePropertyChoice*>(getProperty(_name));
}

dePropertyBoolean* deBaseLayer::getPropertyBoolean(const std::string& _name)
{
    return dynamic_cast<dePropertyBoolean*>(getProperty(_name));
}

const dePropertyBoolean* deBaseLayer::getPropertyBoolean(const std::string& _name) const
{
    return dynamic_cast<const dePropertyBoolean*>(getProperty(_name));
}

deValue deBaseLayer::getNumericValue(const std::string& name) const
{
    const dePropertyNumeric* p = getPropertyNumeric(name);
    if (p)
    {
        return p->get();
    }
    return 0;
}

void deBaseLayer::applyPreset(const std::string& name)
{
    std::vector<dePreset*>::iterator i;
    for (i = presets.begin(); i != presets.end(); i++)
    {
        dePreset* preset = *i;
        if (preset->getName() == name)
        {
            preset->apply(*this);
        }
    }
}

void deBaseLayer::getPresets(std::vector<std::string>& names)
{
    std::vector<dePreset*>::iterator i;
    for (i = presets.begin(); i != presets.end(); i++)
    {
        dePreset* preset = *i;
        names.push_back(preset->getName());
    }
}

dePreset* deBaseLayer::createPreset(const std::string& name)
{
    dePreset* preset = new dePreset(name);
    presets.push_back(preset);
    return preset;
}

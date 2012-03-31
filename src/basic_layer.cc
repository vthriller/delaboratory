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

#include "basic_layer.h"
#include "process_linear.h"

deBasicLayer::deBasicLayer(deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager, const std::string& _name)
:deActionLayer(_name, _colorSpace, _index, _sourceLayer, _layerStack, _channelManager, _viewManager)
{
    getBasicSettings(colorSpace, settings1, settings2);

    std::copy(settings1.begin(), settings1.end(), std::back_inserter(settings));
    std::copy(settings2.begin(), settings2.end(), std::back_inserter(settings));

    int n = settings.size();
    int i;
    for (i = 0; i < n; i++)
    {
        deBasicSetting& setting = settings[i];
        std::string n = setting.getName();
        dePropertyValue* p = new dePropertyValue(n);
        p->setMin(-1.0);
        p->setMax(1.0);
        int c = setting.getChannel();
        if (c >= 0)
        {
            p->setChannel(c);
        }
        basicProperties.push_back(p);
    }

    int nn = getColorSpaceSize(colorSpace);
    curves = new deCurve[nn];

    shiftIndex = -1;
    shiftValue = 0.0;

    int j;
    for (j = 0; j < n; j++)
    {
        deBasicSetting& s = settings[j];
        if (s.isShift())
        {
            for (i = 0; i < nn; i++)
            {
                if (s.affects(i))
                {
                    shiftIndex = i;
                }
            }        
        }
    }
}

deBasicLayer::~deBasicLayer()
{
    int n = settings.size();
    int i;
    for (i = 0; i < n; i++)
    {
        dePropertyValue* p = basicProperties[i];
        delete p;
    }

    delete [] curves;
}

bool deBasicLayer::isChannelNeutral(int index)
{
    return false;
}    

void deBasicLayer::reset()
{
    int nn = settings.size();
    int j;
    for (j = 0; j < nn; j++)
    {
        basicProperties[j]->set(0.0);
    }
}

void deBasicLayer::save(xmlNodePtr root)
{
    saveCommon(root);
    saveBlend(root);
}

void deBasicLayer::load(xmlNodePtr root)
{
    loadBlend(root);

    xmlNodePtr child = root->xmlChildrenNode;

    while (child)
    {
        child = child->next;
    }        
}

void deBasicLayer::updateCurve(int i)
{
    deValue brightness = 0.0;
    deValue contrast = 0.0;

    int n = settings.size();
    int j;
    for (j = 0; j < n; j++)
    {
        deBasicSetting& s = settings[j];
        if (s.affects(i))
        {
            deValue scale = s.getScale();
            dePropertyValue* p = basicProperties[j];
            if (s.isContrast())
            {
                contrast += scale * p->get();
            }
            if (s.isBrightness())
            {
                brightness += scale * p->get();
            }
        }
    }

    curves[i].setContrastBrightness(contrast, brightness);
}

bool deBasicLayer::processAction(int i, const deChannel& sourceChannel, deChannel& channel, deSize size)
{
    if (i == shiftIndex)
    {
        shiftChannel(sourceChannel.getPixels(), channel.getPixels(), shiftValue, size.getN());    
    }
    else
    {
        curves[i].process(sourceChannel, channel, size.getN());
    }

    return true;
}



bool deBasicLayer::randomize()
{
    return true;
}

int deBasicLayer::getNumberOfSettings()
{
    int n = settings.size();
    return n;
}

int deBasicLayer::getSeparator()
{
    int n = settings1.size();
    return n;
}

dePropertyValue* deBasicLayer::getBasicProperty(int n)
{
    dePropertyValue* p = basicProperties[n];
    return p;
}

void deBasicLayer::onUpdateProperties()
{
    int n = getColorSpaceSize(colorSpace);
    int i;
    for (i = 0; i < n; i++)
    {
        updateCurve(i);
    }        

    int nn = settings.size();
    int j;
    for (j = 0; j < nn; j++)
    {
        deBasicSetting& s = settings[j];
        if (s.isShift())
        {
            shiftValue = basicProperties[j]->get();
        }
    }
}

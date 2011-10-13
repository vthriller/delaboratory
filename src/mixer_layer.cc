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

#include "mixer_layer.h"
#include "project.h"
#include "mixer_editor.h"
#include <iostream>

deMixerLayer::deMixerLayer(deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager, const std::string& _name)
:deActionLayer(_name, _colorSpace, _index, _sourceLayer, _layerStack, _channelManager, _viewManager) 
{
    int n = getColorSpaceSize(colorSpace);
    int i;
    for (i = 0; i < n; i++)
    {
        mixers.push_back( new deMixer(n));
        mixers[i]->reset(i);
    }

}

deMixerLayer::~deMixerLayer()
{
    int n = getColorSpaceSize(colorSpace);
    int i;
    for (i = 0; i < n; i++)
    {
        delete mixers[i];
    }
}

void deMixerLayer::processAction4(int i, const deChannel* s1, const deChannel* s2, const deChannel* s3, const deChannel* s4, deChannel& channel, int channelSize)
{
    mixers[i]->process(s1, s2, s3, s4, channel, channelSize);
}


void deMixerLayer::createActionFrame(wxWindow* parent)
{
    if (!actionFrame)
    {
        actionFrame = new deMixerEditor(parent, *this);
        actionFrame->Show(true);
    }        
}

deMixer* deMixerLayer::getMixer(int index)
{
    int n = getColorSpaceSize(colorSpace);
    if ((index < 0) || (index >= n))
    {
        return NULL;
    }
    return mixers[index];
}

bool deMixerLayer::isChannelNeutral(int index)
{
    return mixers[index]->isNeutral(index);
}    

void deMixerLayer::setValue(int s, int d, deValue value)
{
    mixers[d]->setValue(s, value);
}

deValue deMixerLayer::getValue(int s, int d)
{
    return mixers[d]->getValue(s);
}

void deMixerLayer::save(xmlNodePtr root)
{
    saveCommon(root);
};

void deMixerLayer::load(xmlNodePtr root)
{
}

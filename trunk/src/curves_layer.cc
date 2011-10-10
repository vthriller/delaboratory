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

#include "curves_layer.h"
#include "project.h"
#include "curves_editor.h"
#include <iostream>

deCurvesLayer::deCurvesLayer(deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager, const std::string& _name)
:deActionLayer(_name, _colorSpace, _index, _sourceLayer, _layerStack, _channelManager, _viewManager) 
{

    int n = getColorSpaceSize(colorSpace);
    curves = new deCurve[n];

}

deCurvesLayer::~deCurvesLayer()
{
    delete [] curves;
}

void deCurvesLayer::processAction(int i, const deChannel& sourceChannel, deChannel& channel, deSize size)
{
    curves[i].process(sourceChannel, channel, size.getN());
}


void deCurvesLayer::createActionFrame(wxWindow* parent)
{
    if (!actionFrame)
    {
        actionFrame = new deCurvesEditor(parent, *this);
        actionFrame->Show(true);
    }        
}

deCurve* deCurvesLayer::getCurve(int index)
{
    int n = getColorSpaceSize(colorSpace);
    if ((index < 0) || (index >= n))
    {
        return NULL;
    }
    return &(curves[index]);
}

bool deCurvesLayer::isChannelNeutral(int index)
{
    return curves[index].isNeutral();
}    

void deCurvesLayer::onKey(int key)
{
    if (actionFrame)
    {
        deCurvesEditor* editor = dynamic_cast<deCurvesEditor*>(actionFrame);
        editor->onKey(key);
    }
}

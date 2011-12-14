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
#include <iostream>
#include <cassert>

#include "frame_factory.h"
#include "curves_editor.h"

deCurvesLayer::deCurvesLayer(deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deLayerProcessor& _processor, deChannelManager& _channelManager, deViewManager& _viewManager, const std::string& _name)
:deActionLayer(_name, _colorSpace, _index, _sourceLayer, _layerStack, _processor, _channelManager, _viewManager) 
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

void deCurvesLayer::save(xmlNodePtr root)
{
    saveCommon(root);
    saveBlend(root);

    int n = getColorSpaceSize(colorSpace);
    int i;
    for (i = 0; i < n; i++)
    {
        deCurve& curve = curves[i];
        xmlNodePtr child = xmlNewChild(root, NULL, BAD_CAST("curve"), NULL);
        curve.save(child);
    }
}

void deCurvesLayer::load(xmlNodePtr root)
{
    loadBlend(root);

    xmlNodePtr child = root->xmlChildrenNode;

    int i = 0;
    while (child)
    {
        if ((!xmlStrcmp(child->name, BAD_CAST("curve")))) 
        {
            assert(i < n);
            deCurve& curve = curves[i];
            curve.load(child);
            i++;
        }

        child = child->next;
    }
}

void deCurvesLayer::setHistogramChannel(int channel)
{
    viewManager.setHistogramChannel(channel);
}

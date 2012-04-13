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

#include "apply_luminance_layer.h"
#include "apply_luminance_color.h"
#include "copy_channel.h"

deApplyLuminanceLayer::deApplyLuminanceLayer(deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager)
:deActionLayer(_colorSpace, _index, _sourceLayer, _layerStack, _channelManager, _viewManager) 
{
    channelIndex = 1;

}

deApplyLuminanceLayer::~deApplyLuminanceLayer()
{
}

bool deApplyLuminanceLayer::processActionFull(deValue** sp, deValue** dp, int channelSize)
{
    if (colorSpace == deColorSpaceRGB)
    {
        int i;
        for (i = 0; i < channelSize; i++)
        {
            applyLuminance(sp[0][i], sp[1][i], sp[2][i], sp[channelIndex][i], sp[channelIndex][i], sp[channelIndex][i], dp[0][i], dp[1][i], dp[2][i]);
        }
        return true;
    }

    int n = getColorSpaceSize(colorSpace);
    int i;
    for (i = 0; i < n; i++)
    {
        copyChannel(sp[i], dp[i], channelSize);
    }

    return true;
}

void deApplyLuminanceLayer::save(xmlNodePtr root)
{
    saveCommon(root);
    saveBlend(root);
}

void deApplyLuminanceLayer::load(xmlNodePtr root)
{
    loadBlend(root);

    xmlNodePtr child = root->xmlChildrenNode;

    while (child)
    {

        child = child->next;
    }
}


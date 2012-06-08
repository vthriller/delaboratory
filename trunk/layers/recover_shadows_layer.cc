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

#include "recover_shadows_layer.h"
#include "usm.h"
#include "preset.h"
#include "view_manager.h"
#include "color_space_utils.h"
#include "copy_channel.h"
#include "logger.h"

deRecoverShadowsLayer::deRecoverShadowsLayer(deColorSpace _colorSpace, deChannelManager& _channelManager, int _sourceLayer, deLayerStack& _layerStack, deViewManager& _viewManager)
:deLayerWithBlending(_colorSpace, _channelManager, _sourceLayer, _layerStack), viewManager(_viewManager)
{
    dePreset* reset = createPreset("reset");
    createPropertyNumeric("radius", 2, 600);
    reset->addNumericValue("radius", 200);

    createPropertyChoice("channel", getChannelNames(colorSpace));

    applyPreset("reset");
    setOpacity(0.5);
}

deRecoverShadowsLayer::~deRecoverShadowsLayer()
{
}

bool deRecoverShadowsLayer::updateMainImageNotThreadedWay()
{
    deValue r = getNumericValue("radius") * viewManager.getRealScale();

    int channel = getPropertyChoice("channel")->getIndex();

    bool result = shadowsHighlights(r, channel, getSourceImage(), mainLayerImage, true);

    return result;
}    


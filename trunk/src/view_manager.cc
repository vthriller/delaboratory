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

#include "view_manager.h"
#include "project.h"
#include "layer.h"
#include "layer_processor.h"
#include "str.h"
#include "channel_manager.h"
#include "layer_stack.h"
#include "zoom_manager.h"

deViewManager::deViewManager(deProject& _project, deLayerProcessor& _processor, deZoomManager& _zoomManager)
:project(_project), layerProcessor(_processor), zoomManager(_zoomManager)
{
    view = 0;
    single = false;
}

deViewManager::~deViewManager()
{
}

int deViewManager::getView() const
{
    return view;
}

void deViewManager::setView(int v)
{
    logInfo("view manager set view " + str(v));
    int old = view;
    view = v;
    project.onChangeView(old);
}

void deViewManager::setSingleChannel(int _channel)
{
    single = true;
    channel = _channel;
    project.onChangeViewMode();
    layerProcessor.onChangeViewMode();

}

void deViewManager::setNormal()
{
    single = false;
    project.onChangeViewMode();
    layerProcessor.onChangeViewMode();
}

deColorSpace deViewManager::getColorSpace() const
{
    deLayerStack& layerStack = project.getLayerStack();
    deBaseLayer* layer = layerStack.getLayer(view);
    if (!layer)
    {
        return deColorSpaceInvalid;
    }

    return layer->getColorSpace();
}

void deViewManager::setHistogramChannel(int channel)
{
    project.setHistogramChannel(channel);
}

deValue deViewManager::getRealScale() const
{
    deSize ps = project.getPreviewChannelManager().getChannelSizeFromChannelManager();

    deSize ss = project.getSourceImageSize();

    deValue sx = (deValue) ps.getW() / ss.getW();
    deValue sy = (deValue) ps.getH() / ss.getH();

    deValue s = sx;
    if (sy < s)
    {
        s = sy;
    }

    return s;
}

void deViewManager::getZoom(deValue& _x1, deValue& _y1, deValue& _x2, deValue& _y2)
{
    return zoomManager.getZoom(_x1, _y1, _x2, _y2);
}

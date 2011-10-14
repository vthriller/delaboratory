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

deViewManager::deViewManager(deProject& _project)
:project(_project)
{
    view = 0;
    showBlendMask = false;
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
    hideMask();
    int old = view;
    view = v;
    project.onChangeView(old, v);
}

void deViewManager::repaint()
{
    project.repaintImage();
}    

void deViewManager::setSingleChannel(int _channel)
{
    single = true;
    channel = _channel;
    project.onChangeViewMode();
    project.repaintImage();
}

void deViewManager::setNormal()
{
    single = false;
    project.onChangeViewMode();
    project.repaintImage();
}

void deViewManager::hideMask()
{
    showBlendMask = false;
    project.onChangeViewMode();
}

void deViewManager::showMask(int maskChannel)
{
    showBlendMask = true;
    blendMaskChannel = maskChannel;
    project.onChangeViewMode();
}

void deViewManager::hideThisMask(int maskChannel)
{
    if (blendMaskChannel != maskChannel)
    {
        return;
    }
    showBlendMask = false;
    project.onChangeViewMode();
}

bool deViewManager::maskVisible() const
{
    return showBlendMask;
}

int deViewManager::getMaskChannel() const
{
    return blendMaskChannel;
}

deColorSpace deViewManager::getColorSpace() const
{
    deLayerStack& layerStack = project.getLayerStack();
    deLayer* layer = layerStack.getLayer(view);
    if (!layer)
    {
        return deColorSpaceInvalid;
    }

    return layer->getColorSpace();
}

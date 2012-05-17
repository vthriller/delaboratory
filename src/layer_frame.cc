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

#include "layer_frame.h"
#include "layer_frame_manager.h"
#include "logger.h"
#include "str.h"

deLayerFrameOld::deLayerFrameOld(wxWindow *parent, deBaseLayer& _layer, const std::string& name, deLayerFrameManager& _frameManager, int _layerIndex)
:deFrameOld(parent, name),
layer(_layer),
frameManager(_frameManager),
layerIndex(_layerIndex)
{
}

deLayerFrameOld::~deLayerFrameOld()
{
}

bool deLayerFrameOld::checkIndex(int index)
{
    if (layerIndex == index)
    {
        return true;
    }

    return false;
}

deLayerFrame::deLayerFrame(deWindow& parent, const std::string& name, deBaseLayer& _layer, deLayerFrameManager& _frameManager, int _index)
:deFrame(parent, name), layer(_layer), frameManager(_frameManager), index(_index)
{
    frameManager.addLayerFrame(index, this);
}

deLayerFrame::~deLayerFrame()
{
    logInfo(" ~deLayerFrame");
    frameManager.removeLayerFrame(index);
    logInfo(" ~deLayerFrame DONE");
}

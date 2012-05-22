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

#ifndef _DE_LAYER_FRAME_H
#define _DE_LAYER_FRAME_H

#include "frame.h"
#include "value.h"
class deActionLayer;
class deBaseLayer;
class deLayerFrameManager;

class deLayerFrame:public deFrame
{
    protected:
        deBaseLayer& layer;
        deLayerFrameManager& frameManager;
        int index;
    public:
        deLayerFrame(deWindow& parent, const std::string& name, deBaseLayer& _layer, deLayerFrameManager& _frameManager, int _index);
        virtual ~deLayerFrame();
        
        virtual void setUIFromLayer() = 0;
        virtual bool onImageClick(deValue x, deValue y) {return false;};
};

#endif

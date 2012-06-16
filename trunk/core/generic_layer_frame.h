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

#ifndef _DE_GENERIC_LAYER_FRAME_H
#define _DE_GENERIC_LAYER_FRAME_H

#include "layer_frame.h"
class deLayerProcessor;
#include <vector>
class dePropertyNumericUI;
class dePropertyChoiceUI;
class dePropertyBooleanUI;
class dePropertyLevelsUI;
class dePropertyCurvesUI;
class dePropertyMixerUI;

class deGenericLayerFrame:public deLayerFrame
{
    protected:
        std::vector<dePropertyNumericUI*> numerics;
        std::vector<dePropertyChoiceUI*> choices;
        std::vector<dePropertyBooleanUI*> booleans;
        std::vector<dePropertyLevelsUI*> levels;
        std::vector<dePropertyCurvesUI*> curves;
        std::vector<dePropertyMixerUI*> mixers;

        deLayerProcessor& layerProcessor;
    public:
        deGenericLayerFrame(deWindow& parent, const std::string& name, deBaseLayer& _layer, deLayerProcessor& _layerProcessor, deLayerFrameManager& _frameManager, int _index);
        virtual ~deGenericLayerFrame();

        virtual void setUIFromLayer();

        virtual bool onImageClick(deValue x, deValue y);
        virtual bool onKey(int key);
};

#endif

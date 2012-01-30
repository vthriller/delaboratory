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

#ifndef _DE_HUE_SATURATION_FRAME_H
#define _DE_HUE_SATURATION_FRAME_H

#include "layer_frame.h"
#include "slider.h"

class dePropertyValueSlider;
class deLayerProcessor;
class deConversionBW2HueLayer;

class deHueSaturationFrame:public deLayerFrame
{
    private:
        dePropertyValueSlider* hue;
        dePropertyValueSlider* saturation;
        deLayerProcessor& layerProcessor;
        deConversionBW2HueLayer& layer;

        wxButton* reset;
        wxButton* preset1;
        wxButton* preset2;
        wxButton* preset3;

        void click(wxCommandEvent &event);

    public:
        deHueSaturationFrame(wxWindow *parent, deConversionBW2HueLayer& _layer, deLayerProcessor& _layerProcessor, deLayerFrameManager& _frameManager);
        virtual ~deHueSaturationFrame();


};


#endif

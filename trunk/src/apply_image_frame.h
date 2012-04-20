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

#ifndef _DE_APPLY_IMAGE_FRAME_H
#define _DE_APPLY_IMAGE_FRAME_H

#include "action_frame.h"
#include "color_space.h"
class dePropertyChoiceUI;
class dePropertyBooleanUI;
class deLayerProcessor;

class deApplyImageFrame:public deActionFrame
{
    private:
        dePropertyChoiceUI* appliedLayer;
        dePropertyBooleanUI* applySingleChannel;
        wxChoice* layerChoice;
        wxRadioButton* channels[MAX_COLOR_SPACE_SIZE];
        deLayerProcessor& layerProcessor;

        void select(wxCommandEvent &event);

        void setChannels();

    public:
        deApplyImageFrame(wxWindow *parent, deLayer& _layer, deLayerProcessor& _layerProcessor, deLayerFrameManager& _frameManager, int _layerIndex);
        virtual ~deApplyImageFrame();

        virtual void onUpdateProperties();

};


#endif

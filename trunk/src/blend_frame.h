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

#ifndef _DE_BLEND_FRAME_H
#define _DE_BLEND_FRAME_H

#include "layer_frame.h"
#include "blend_mode.h"
#include "slider.h"
class deLayerProcessor;

class deBlendFrame:public deLayerFrame
{
    private:
        bool applyAllowed;

        void choose(wxCommandEvent &event);
        void check(wxCommandEvent &event);
        void select(wxCommandEvent &event);

        void setChannels();

        wxChoice* choice;
        deSlider* alphaSlider;
        deSlider* blurRadiusSlider;
        deSlider* blendMaskMinSlider;
        deSlider* blendMaskMaxSlider;
        wxRadioButton *b1;
        wxRadioButton *b2;
        wxRadioButton *b3;
        wxChoice* maskLayerChoice;
        std::vector<deBlendMode> blendModes;
        std::vector<wxCheckBox*> channels;
        wxRadioButton* maskChannels[4];
        deLayerProcessor& layerProcessor;

        wxCheckBox* maskEnable;
        wxCheckBox* maskShow;

        int getBlendChannel();

        void updateMask();

        void showHide();

    public:
        deBlendFrame(wxWindow *parent, deActionLayer& _layer, deLayerProcessor& _layerProcessor, deLayerFrameManager& _frameManager);
        virtual ~deBlendFrame();

        void beforeClose();

};


#endif

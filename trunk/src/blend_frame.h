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

#include "action_frame.h"
#include <vector>
#include "blend_mode.h"
class deBlendLayer;
class dePreviewStack;
class deSlider;

class deBlendFrame:public deActionFrame
{
    private:
        deBlendLayer& layer;
        dePreviewStack& stack;
        deSlider* alphaSlider;
        wxCheckBox* singleOverlayChannelCheckBox;
        wxCheckBox* singleDestinationChannelCheckBox;
        wxChoice* overlayChannelChoice;
        wxChoice* destinationChannelChoice;
        int layerNumber;

        wxChoice* blendModeChoice;
        std::vector<deBlendMode> blendModes;

        void choose(wxCommandEvent &event);
        void check(wxCommandEvent &event);

        void update();
        void updateSelections();

    public:
        deBlendFrame(wxWindow* parent, deBlendLayer& _layer, dePreviewStack& _stack, int layerNumber);
        virtual ~deBlendFrame();
};

#endif

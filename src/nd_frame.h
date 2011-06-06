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

#ifndef _DE_ND_FRAME_H
#define _DE_ND_FRAME_H

#include "action_frame.h"
#include <vector>
#include "nd_type.h"
class deNDLayer;
class dePreviewStack;
class deSlider;

class deNDFrame:public deActionFrame
{
    private:
        deNDLayer& layer;
        dePreviewStack& stack;
        int layerNumber;
        deSlider* xCenterSlider;
        deSlider* yCenterSlider;
        deSlider* powerSlider;

        wxChoice* ndTypeChoice;
        std::vector<deNDType> ndTypes;

        void choose(wxCommandEvent &event);

    public:
        deNDFrame(wxWindow* parent, deNDLayer& _layer, dePreviewStack& _stack, int layerNumber);
        virtual ~deNDFrame();
};

#endif

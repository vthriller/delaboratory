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

#ifndef _DE_ADD_LAYER_FRAME_H
#define _DE_ADD_LAYER_FRAME_H

#include <wx/wx.h>
class deLayerListPanel;
class deProject;
#include <map>
#include "color_space.h"

class deAddLayerFrame:public wxFrame
{
    private:
        deLayerListPanel* panel;
        deProject* project;

        wxButton* curvesButton;
        wxButton* mixerButton;
        wxButton* convertButton;
        wxButton* blendButton;
        wxButton* blurButton;
        wxButton* ndButton;
        wxButton* grainButton;

        void click(wxCommandEvent &event);
    public:
        deAddLayerFrame(deLayerListPanel* _panel, deProject* _project);
        virtual ~deAddLayerFrame();

};

#endif

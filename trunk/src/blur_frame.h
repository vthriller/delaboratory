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

#ifndef _DE_BLUR_FRAME_H
#define _DE_BLUR_FRAME_H

#include "action_frame.h"
#include "slider.h"

/*
class deCurvesPanel;
class deGradientPanel;
*/

class deBlurFrame:public deActionFrame
{
    private:
        deSlider* radius;
    /*
        deCurvesPanel* curvesPanel;

        void choose(wxCommandEvent &event);
        void click(wxCommandEvent &event);

        deGradientPanel* leftBar;
        deGradientPanel* bottomBar;

        wxChoice* channelChoice;
        wxButton* reset;
        wxButton* invert;
        wxButton* const0;
        wxButton* const05;
        wxButton* const1;
        wxButton* angle1;
        wxButton* angle2;
        wxButton* angle3;
        wxButton* angle4;
        wxButton* angle5;
        */

    public:
        deBlurFrame(wxWindow *parent, deActionLayer& _layer);
        virtual ~deBlurFrame();

        virtual void onImageClick(deValue x, deValue y) {};

};


#endif

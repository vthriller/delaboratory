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

#ifndef _DE_CURVES_FRAME_H
#define _DE_CURVES_FRAME_H

#include <vector>
#include "action_frame.h"
class deCurvesLayer;
class dePreviewStack;
class deCurvesPanel;

class deCurvesFrame:public deActionFrame
{
    private:
        deCurvesLayer& curvesLayer;
        wxChoice* channelChoice;
        deCurvesPanel* curvesPanel;

        void choose(wxCommandEvent &event);

    public:
        deCurvesFrame(wxWindow *parent, deCurvesLayer& _curvesLayer, dePreviewStack& _stack, int layerNumber);
        virtual ~deCurvesFrame();
        
        virtual void traceSampler(deSampler* sampler);

};


#endif

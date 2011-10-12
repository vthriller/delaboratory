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

#ifndef _DE_SAMPLER_MANAGER_FRAME_H
#define _DE_SAMPLER_MANAGER_FRAME_H

#include "frame.h"
#include "color_space.h"
#include <vector>
class deProject;
class deControlPanel;

class deSamplerManagerFrame:public deFrame
{
    class deSamplerRow
    {
        public:
            deSamplerRow()
            {
            };

            wxStaticText* x;
            wxStaticText* y;
            wxChoice* colorSpace;
            wxRadioButton* selected;
            wxStaticText* v1;
            wxStaticText* v2;
            wxStaticText* v3;
            wxStaticText* v4;
    };

    private:
        std::vector<deSamplerRow> samplerRows;
        std::vector<deColorSpace> colorSpaces;

        void select(wxCommandEvent &event);
        void choose(wxCommandEvent &event);
    protected:
        wxFlexGridSizer* gridSizer;
        deProject& project;
        deControlPanel* controlPanel;

        void buildRows();
    public:
        deSamplerManagerFrame(deControlPanel *parent, deProject& _project);
        virtual ~deSamplerManagerFrame();

        void update();

        void dumpColor();
};

#endif

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

#ifndef _DE_SAMPLER_MANAGER_H
#define _DE_SAMPLER_MANAGER_H

class deProject;
class deLayerProcessor;
#include "sampler.h"
#include <vector>

class deSamplerManager
{
    private:
        deLayerProcessor& layerProcessor;
        std::vector<deSampler> samplers;

        int selected;
        int size;

        bool moving;

        bool select(deValue x, deValue y);

        
    public:
        deSamplerManager(deLayerProcessor& _processor);
        virtual ~deSamplerManager();

        int getNumberOfSamplers() const {return samplers.size();};
        deSampler* getSampler(int index);

        //bool onImageClick(deValue x, deValue y);
        bool onClick(deValue x, deValue y);
        bool onMove(deValue x, deValue y);
        bool onRelease();

        void setSelected(int s);
        int getSelected() const {return selected;};

        void setMoving(bool m);
};

#endif

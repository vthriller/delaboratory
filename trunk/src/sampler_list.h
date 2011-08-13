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

#ifndef _DE_SAMPLER_LIST_H
#define _DE_SAMPLER_LIST_H

#include <list>
#include <vector>
class deSampler;
class deProject;

typedef std::vector<deSampler*> deSamplers;

class deSamplerList
{
    private:
        deSamplers samplers;
        deProject* project;

        int selected;

        void select(int sampler);
        void deselect();
        void moveSampler(float x, float y);

        void addSampler(float x, float y);
        void removeSampler();

    public:        
        deSamplerList();
        virtual ~deSamplerList();

        void setProject(deProject* _project);

        void click(float x, float y);
        void release(float x, float y);
        void move(float x, float y);

        deSamplers& getSamplers();

        int getSelected() const;

        void removeAllSamplers();

};

#endif

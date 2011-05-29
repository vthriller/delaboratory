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

#ifndef _DE_SAMPLER_H
#define _DE_SAMPLER_H

class deProject;
#include "color_space.h"

class deSampler
{
    private:
        float x;
        float y;
        bool enabled;

        deProject* project;
//        deColorSpace mode;
        deSampler(const deSampler& sampler);

    public:
        deSampler(deProject* _project);
        virtual ~deSampler();

//        void setColorSpace(deColorSpace colorSpace);
//        deColorSpace getColorSpace() const {return mode;};

        void setPosition(float _x, float _y);

        float getX() const;
        float getY() const;

        bool getPixel(float &v0, float &v1, float &v2, float &v3, deColorSpace mode) const;

        bool isEnabled() const;
        void enable();
        void disable();

};

#endif

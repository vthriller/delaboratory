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

#include "color_space.h"
#include "value.h"

class deSampler
{
    private:
        deValue x;
        deValue y;
        deColorSpace colorSpace;
        bool enabled;
        bool selected;

    public:
        deSampler();
        virtual ~deSampler();

        deValue getX() const {return x;};
        deValue getY() const {return y;};

        void setPosition(deValue _x, deValue _y);

        deColorSpace getColorSpace() const {return colorSpace;};
        void setColorSpace(deColorSpace c);

        bool isEnabled() const;
        void enable();
        void disable();

        bool isSelected() const;
        void setSelected(bool s);

};

#endif

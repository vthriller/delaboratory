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

#ifndef _DE_PALETTE_H
#define _DE_PALETTE_H

#include <vector>
#include "value.h"
#include "color_space.h"

class deColor3
{
    private:
        deValue v1;
        deValue v2;
        deValue v3;
    public:
        deColor3(deValue _v1, deValue _v2, deValue _v3)
        :v1(_v1),
         v2(_v2),
         v3(_v3)
        {
        };
        virtual ~deColor3()
        {
        };
};

class dePalette3
{
    private:
        deColorSpace colorSpace;
        std::vector<deColor3> colors;

    public:
        dePalette3(deColorSpace _colorSpace);
        virtual ~dePalette3();

        void addColor(const deColor3& color);
    
};

#endif

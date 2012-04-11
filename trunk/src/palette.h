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
#include <cmath>

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

        deValue getV1() {return v1;};
        deValue getV2() {return v2;};
        deValue getV3() {return v3;};

        deValue calcDistance(const deColor3& c) const
        {
            deValue d1 = c.v1 - v1;
            deValue d2 = c.v2 - v2;
            deValue d3 = c.v3 - v3;
            deValue d = d1 * d1 + d2 * d2 + d3 * d3;
            return sqrt(d);
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

        void optimize(const dePalette3& source, int n, deValue margin);

        deColor3 getColor(int index);

        int getSize() const {return colors.size();};

        void getMinMax(int index, deValue& min, deValue& max);
        bool find23(deValue minA, deValue maxA, deValue minB, deValue maxB, deValue& L);
    
};

#endif

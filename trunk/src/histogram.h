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

#ifndef _DE_HISTOGRAM_H
#define _DE_HISTOGRAM_H

#include <vector>
#include <list>
#include "value.h"
//#include "curve_point.h"

class deChannel;

class deHistogram
{
    private:
        std::vector<int> bars;
/*        float min;
        float max;*/
        int size;
    public:
        deHistogram(/*float _min, float _max, */int _size);
        virtual ~deHistogram();

        void put(deValue value);
        void calc(const deChannel* channel, int n);
        int get(int bar) const;
        void clear();

//        virtual void fill(std::list<deCurvePoint>& points, int m) const;

        int getMax() const;
        int getSize() const;

        bool render(unsigned char* data, int sizeW, int sizeH, unsigned char g1, unsigned char g2);

};

#endif

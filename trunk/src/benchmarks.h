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

#ifndef _DE_BENCHMARKS_H
#define _DE_BENCHMARKS_H

#include "blur.h"
#include "color_space.h"

class deBenchmark
{
    private:
    public:
        deBenchmark();
        virtual ~deBenchmark();

        virtual int perform() = 0;
};

class deBenchmarkBlur:public deBenchmark
{
    private:
        deSize size;
        deValue radius;
        deBlurType type;
    public:
        deBenchmarkBlur(deSize _size, deValue _radius, deBlurType _type);
        virtual ~deBenchmarkBlur();

        virtual int perform();
};

class deBenchmarkColor:public deBenchmark
{
    private:
        int size;
        deColorSpace src;
        deColorSpace dst;
    public:
        deBenchmarkColor(int _size, deColorSpace _src, deColorSpace _dst);
        virtual ~deBenchmarkColor();

        virtual int perform();
};

#endif

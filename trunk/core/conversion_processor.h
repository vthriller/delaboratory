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

#ifndef _DE_CONVERSION_PROCESSOR_H
#define _DE_CONVERSION_PROCESSOR_H

#include "conversion_cpu.h"


class deConversionProcessor
{
    private:

    public:
        deConversionProcessor();
        virtual ~deConversionProcessor();

        void convertImage(const deImage& sourceImage, deImage& image, deConversionCPU& cpu);

        int convertImageNew(const deImage& sourceImage, deImage& image);
        bool renderImageToRGBNew(const deImage& image, unsigned char* data);
        bool convert(deColorSpace sourceColorSpace, deValue v1, deValue v2, deValue v3, deValue v4, deColorSpace targetColorSpace, deValue &r1, deValue& r2, deValue& r3, deValue& r4);

};



#endif

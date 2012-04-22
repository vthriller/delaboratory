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

#include "value.h"
class deImage;
class deChannelManager;
#include "color_space.h"

#define CPU_REGISTER_OVERFLOW 0
#define CPU_REGISTER_CMYK_KEY_SUB 1
#define CPU_REGISTER_CMYK_KEY_MAX 2
#define CPU_REGISTER_CMYK_MIN_SUM 3

#define CPU_REGISTERS 4

class deConversionCPU
{
    private:
    public:
        deValue* input;
        deValue* output;
        deValue* registers;

        deConversionCPU(int size);
        virtual ~deConversionCPU();

        void switchIO();

};


class deConversionProcessor
{
    private:

    public:
        deConversionProcessor();
        virtual ~deConversionProcessor();

        void convertImage(const deImage& sourceImage, deImage& image, deChannelManager& channelManager);
        bool renderImageToRGB(const deImage& image, unsigned char* data, deChannelManager& channelManager);
        bool convertPixel(const deImage& image, int p, deColorSpace colorSpace, deValue &v1, deValue& v2, deValue& v3, deValue& v4);
        bool convertToRGB(deValue v1, deValue v2, deValue v3, deValue v4, deColorSpace colorSpace, deValue &rr, deValue& gg, deValue& bb);
        bool convertToLAB(deValue v1, deValue v2, deValue v3, deValue v4, deColorSpace colorSpace, deValue &rr, deValue& gg, deValue& bb);

};


#endif

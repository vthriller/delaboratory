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
#define CPU_REGISTER_BW_MIXER_R 4
#define CPU_REGISTER_BW_MIXER_G 5
#define CPU_REGISTER_BW_MIXER_B 6
#define CPU_REGISTER_CONTRAST 7
#define CPU_REGISTER_SATURATION 8

#define CPU_REGISTERS 9


class deConversionCPU
{
    private:
    public:
        typedef void (*deFunction)(deConversionCPU& cpu);

        deValue* input;
        deValue* output;
        deValue* registers;

        deConversionCPU(int size);
        virtual ~deConversionCPU();

        void switchIO();
        void incOverflow();

        void convertImage3x3(const deImage& sourceImage, deImage& image, deFunction f1, deFunction f2);
        void convertImage3x4(const deImage& sourceImage, deImage& image, deFunction f1, deFunction f2);
        void convertImage4x3(const deImage& sourceImage, deImage& image, deFunction f1, deFunction f2);
        void convertImage3x1(const deImage& sourceImage, deImage& image, deFunction f1, deFunction f2);
        void convertImage1x3(const deImage& sourceImage, deImage& image, deFunction f1, deFunction f2);

        bool renderImage1(const deImage& image, deConversionCPU::deFunction f, unsigned char* data);
        bool renderImage3(const deImage& image, deConversionCPU::deFunction f, unsigned char* data);
        bool renderImage4(const deImage& image, deConversionCPU::deFunction f, unsigned char* data);

};


class deConversionProcessor
{
    private:

    public:
        deConversionProcessor();
        virtual ~deConversionProcessor();

        void convertImage(const deImage& sourceImage, deImage& image, deConversionCPU& cpu);

        int convertImageNew(const deImage& sourceImage, deImage& image);
        bool renderImageToRGBNew(const deImage& image, unsigned char* data);
        bool convertPixel(const deImage& image, int p, deColorSpace colorSpace, deValue &v1, deValue& v2, deValue& v3, deValue& v4);
        bool convertToRGB(deValue v1, deValue v2, deValue v3, deValue v4, deColorSpace colorSpace, deValue &rr, deValue& gg, deValue& bb);
        bool convertToLAB(deValue v1, deValue v2, deValue v3, deValue v4, deColorSpace colorSpace, deValue &rr, deValue& gg, deValue& bb);

};



void rgb2cmy(deConversionCPU& cpu);
void cmy2rgb(deConversionCPU& cpu);
void cmy2cmyk(deConversionCPU& cpu);
void cmyk2cmy(deConversionCPU& cpu);
void rgb2prophoto(deConversionCPU& cpu);
void prophoto2rgb(deConversionCPU& cpu);

#endif

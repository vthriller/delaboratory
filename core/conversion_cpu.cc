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

#include "conversion_processor.h"

#include "image.h"
#include "channel_manager.h"
#include "color_space_utils.h"

#include "logger.h"

#include "power.h"

#include "str.h"

#include <cmath>

static const deValue Xn  = 0.951;
static const deValue Yn  = 1.0;
static const deValue Zn  = 1.089;
static const deValue c6_29 = 6.0 / 29.0;
static const deValue c29_6 = 29.0 / 6.0;
static const deValue c6_29_2 = c6_29 * c6_29;
static const deValue c6_29_3 = c6_29 * c6_29 * c6_29;
static const deValue c29_6_2 = c29_6 * c29_6;
static const deValue c4_29 = 4.0 / 29.0;

deConversionCPU::deConversionCPU(int size)
{
    input = new deValue [size];
    output = new deValue [size];
    registers = new deValue [CPU_REGISTERS];

    registers[CPU_REGISTER_OVERFLOW] = 0;
    registers[CPU_REGISTER_CMYK_KEY_SUB] = 0.25;
    registers[CPU_REGISTER_CMYK_KEY_MAX] = 0.8;
    registers[CPU_REGISTER_CMYK_MIN_SUM] = 1.0;
    registers[CPU_REGISTER_BW_MIXER_R] = 0.3;
    registers[CPU_REGISTER_BW_MIXER_G] = 0.6;
    registers[CPU_REGISTER_BW_MIXER_B] = 0.1;
    registers[CPU_REGISTER_CONTRAST] = 1.0;
    registers[CPU_REGISTER_PSEUDOGREY] = 0.0;
}

deConversionCPU::~deConversionCPU()
{
    delete [] input;
    delete [] output;
    delete [] registers;
}

void deConversionCPU::switchIO()
{
    deValue* a = input;
    input = output;
    output = a;
}

void deConversionCPU::incOverflow()
{
    registers[CPU_REGISTER_OVERFLOW] ++;
}

bool deConversionCPU::renderImage1(const deImage& image, deConversionCPU::deFunction f, unsigned char* data)
{
    int n = image.getChannelSize().getN();

    const deValue* s0 = image.startRead(0);
    if (!s0)
    {
        logError("NULL in renderImage1");
        image.finishRead(0);
        return false;
    }

    int i;
    int pos = 0;

    for (i = 0; i < n; i++)
    {
        input[0] = s0[i];

        f(*this);

        unsigned char r = 255 * output[0];
        data[pos] = r;
        pos++;
        unsigned char g = 255 * output[1];
        data[pos] = g;
        pos++;
        unsigned char b = 255 * output[2];
        data[pos] = b;
        pos++;
    }

    image.finishRead(0);

    return true;
}    

bool deConversionCPU::renderImage3(const deImage& image, deConversionCPU::deFunction f, unsigned char* data)
{
    int n = image.getChannelSize().getN();

    const deValue* s0 = image.startRead(0);
    const deValue* s1 = image.startRead(1);
    const deValue* s2 = image.startRead(2);
    if ((!s0) || (!s1) || (!s2))
    {
        logError("NULL in renderImage3");
        image.finishRead(0);
        image.finishRead(1);
        image.finishRead(2);
        return false;
    }

    int i;
    int pos = 0;

    for (i = 0; i < n; i++)
    {
        input[0] = s0[i];
        input[1] = s1[i];
        input[2] = s2[i];

        f(*this);

        unsigned char r = 255 * output[0];
        data[pos] = r;
        pos++;
        unsigned char g = 255 * output[1];
        data[pos] = g;
        pos++;
        unsigned char b = 255 * output[2];
        data[pos] = b;
        pos++;
    }

    image.finishRead(0);
    image.finishRead(1);
    image.finishRead(2);

    return true;
}    

bool deConversionCPU::renderImage4(const deImage& image, deConversionCPU::deFunction f, unsigned char* data)
{
    int n = image.getChannelSize().getN();

    const deValue* s0 = image.startRead(0);
    const deValue* s1 = image.startRead(1);
    const deValue* s2 = image.startRead(2);
    const deValue* s3 = image.startRead(3);

    if ((!s0) || (!s1) || (!s2) || (!s3))
    {
        logError("NULL in renderImage4");
        image.finishRead(0);
        image.finishRead(1);
        image.finishRead(2);
        image.finishRead(3);
        return false;
    }

    int i;
    int pos = 0;

    for (i = 0; i < n; i++)
    {
        input[0] = s0[i];
        input[1] = s1[i];
        input[2] = s2[i];
        input[3] = s3[i];

        f(*this);

        unsigned char r = 255 * output[0];
        data[pos] = r;
        pos++;
        unsigned char g = 255 * output[1];
        data[pos] = g;
        pos++;
        unsigned char b = 255 * output[2];
        data[pos] = b;
        pos++;
    }

    image.finishRead(0);
    image.finishRead(1);
    image.finishRead(2);
    image.finishRead(3);

    return true;
}    

void deConversionCPU::convertImage3x3(const deImage& sourceImage, deImage& image, deConversionCPU::deFunction f1, deConversionCPU::deFunction f2)
{
    int n = sourceImage.getChannelSize().getN();

    const deValue* s0 = sourceImage.startRead(0);
    const deValue* s1 = sourceImage.startRead(1);
    const deValue* s2 = sourceImage.startRead(2);

    deValue* d0 = image.startWrite(0);
    deValue* d1 = image.startWrite(1);
    deValue* d2 = image.startWrite(2);

    bool ok;
    if (s0 && s1 && s2 && d0 && d1 && d2)
    {
        ok = true;
    }
    else
    {
        logError("NULL channel in convertImage 3x3");
        ok = false;
    }
    if (ok)
    {
        int i;

        for (i = 0; i < n; i++)
        {
            input[0] = s0[i];
            input[1] = s1[i];
            input[2] = s2[i];

            f1(*this);
            f2(*this);

            d0[i] = output[0];
            d1[i] = output[1];
            d2[i] = output[2];
        }
    }        

    sourceImage.finishRead(0);
    sourceImage.finishRead(1);
    sourceImage.finishRead(2);
    image.finishWrite(0);
    image.finishWrite(1);
    image.finishWrite(2);
}

void deConversionCPU::convertImage3x4(const deImage& sourceImage, deImage& image, deConversionCPU::deFunction f1, deConversionCPU::deFunction f2)
{
    int n = sourceImage.getChannelSize().getN();

    const deValue* s0 = sourceImage.startRead(0);
    const deValue* s1 = sourceImage.startRead(1);
    const deValue* s2 = sourceImage.startRead(2);

    deValue* d0 = image.startWrite(0);
    deValue* d1 = image.startWrite(1);
    deValue* d2 = image.startWrite(2);
    deValue* d3 = image.startWrite(3);

    bool ok;
    if (s0 && s1 && s2 && d0 && d1 && d2 && d3)
    {
        ok = true;
    }
    else
    {
        logError("NULL channel in convertImage 3x4");
        ok = false;
    }
    if (ok)
    {
        int i;
        for (i = 0; i < n; i++)
        {
            input[0] = s0[i];
            input[1] = s1[i];
            input[2] = s2[i];

            f1(*this);
            f2(*this);

            d0[i] = output[0];
            d1[i] = output[1];
            d2[i] = output[2];
            d3[i] = output[3];
        }
    }

    sourceImage.finishRead(0);
    sourceImage.finishRead(1);
    sourceImage.finishRead(2);
    image.finishWrite(0);
    image.finishWrite(1);
    image.finishWrite(2);
    image.finishWrite(3);
}

void deConversionCPU::convertImage3x1(const deImage& sourceImage, deImage& image, deConversionCPU::deFunction f1, deConversionCPU::deFunction f2)
{
    int n = sourceImage.getChannelSize().getN();

    const deValue* s0 = sourceImage.startRead(0);
    const deValue* s1 = sourceImage.startRead(1);
    const deValue* s2 = sourceImage.startRead(2);

    deValue* d0 = image.startWrite(0);

    bool ok;
    if (s0 && s1 && s2 && d0)
    {
        ok = true;
    }
    else
    {
        logError("NULL channel in convertImage 3x1");
        ok = false;
    }
    if (ok)
    {
        int i;

        for (i = 0; i < n; i++)
        {
            input[0] = s0[i];
            input[1] = s1[i];
            input[2] = s2[i];

            f1(*this);
            f2(*this);

            d0[i] = output[0];
        }
    }

    sourceImage.finishRead(0);
    sourceImage.finishRead(1);
    sourceImage.finishRead(2);
    image.finishWrite(0);
}

void deConversionCPU::convertImage4x1(const deImage& sourceImage, deImage& image, deConversionCPU::deFunction f1, deConversionCPU::deFunction f2)
{
    int n = sourceImage.getChannelSize().getN();

    const deValue* s0 = sourceImage.startRead(0);
    const deValue* s1 = sourceImage.startRead(1);
    const deValue* s2 = sourceImage.startRead(2);
    const deValue* s3 = sourceImage.startRead(3);

    deValue* d0 = image.startWrite(0);

    bool ok;
    if (s0 && s1 && s2 && s3 && d0)
    {
        ok = true;
    }
    else
    {
        logError("NULL channel in convertImage 4x1");
        ok = false;
    }

    if (ok)
    {
        int i;

        for (i = 0; i < n; i++)
        {
            input[0] = s0[i];
            input[1] = s1[i];
            input[2] = s2[i];
            input[3] = s3[i];

            f1(*this);
            f2(*this);

            d0[i] = output[0];
        }
    }        

    sourceImage.finishRead(0);
    sourceImage.finishRead(1);
    sourceImage.finishRead(2);
    sourceImage.finishRead(3);
    image.finishWrite(0);
}

void deConversionCPU::convertImage4x3(const deImage& sourceImage, deImage& image, deConversionCPU::deFunction f1, deConversionCPU::deFunction f2)
{
    int n = sourceImage.getChannelSize().getN();

    const deValue* s0 = sourceImage.startRead(0);
    const deValue* s1 = sourceImage.startRead(1);
    const deValue* s2 = sourceImage.startRead(2);
    const deValue* s3 = sourceImage.startRead(3);

    deValue* d0 = image.startWrite(0);
    deValue* d1 = image.startWrite(1);
    deValue* d2 = image.startWrite(2);

    bool ok;
    if (s0 && s1 && s2 && s3 && d0 && d1 && d2)
    {
        ok = true;
    }
    else
    {
        logError("NULL channel in convertImage 4x3");
        ok = false;
    }
    if (ok)
    {
        int i;

        for (i = 0; i < n; i++)
        {
            input[0] = s0[i];
            input[1] = s1[i];
            input[2] = s2[i];
            input[3] = s3[i];

            f1(*this);
            f2(*this);

            d0[i] = output[0];
            d1[i] = output[1];
            d2[i] = output[2];
        }
    }

    sourceImage.finishRead(0);
    sourceImage.finishRead(1);
    sourceImage.finishRead(2);
    sourceImage.finishRead(3);
    image.finishWrite(0);
    image.finishWrite(1);
    image.finishWrite(2);
}

void deConversionCPU::convertImage1x3(const deImage& sourceImage, deImage& image, deConversionCPU::deFunction f1, deConversionCPU::deFunction f2)
{
    int n = sourceImage.getChannelSize().getN();

    const deValue* s0 = sourceImage.startRead(0);

    deValue* d0 = image.startWrite(0);
    deValue* d1 = image.startWrite(1);
    deValue* d2 = image.startWrite(2);

    bool ok;
    if (s0 && d0 && d1 && d2)
    {
        ok = true;
    }
    else
    {
        logError("NULL channel in convertImage 1x3");
        ok = false;
    }
    if (ok)
    {
        int i;

        for (i = 0; i < n; i++)
        {
            input[0] = s0[i];

            f1(*this);
            f2(*this);

            d0[i] = output[0];
            d1[i] = output[1];
            d2[i] = output[2];
        }
    }

    sourceImage.finishRead(0);
    image.finishWrite(0);
    image.finishWrite(1);
    image.finishWrite(2);
}

void deConversionCPU::convertImage1x4(const deImage& sourceImage, deImage& image, deConversionCPU::deFunction f1, deConversionCPU::deFunction f2)
{
    int n = sourceImage.getChannelSize().getN();

    const deValue* s0 = sourceImage.startRead(0);

    deValue* d0 = image.startWrite(0);
    deValue* d1 = image.startWrite(1);
    deValue* d2 = image.startWrite(2);
    deValue* d3 = image.startWrite(3);

    bool ok;
    if (s0 && d0 && d1 && d2 && d3)
    {
        ok = true;
    }
    else
    {
        logError("NULL channel in convertImage 1x4");
        ok = false;
    }
    if (ok)
    {
        int i;

        for (i = 0; i < n; i++)
        {
            input[0] = s0[i];

            f1(*this);
            f2(*this);

            d0[i] = output[0];
            d1[i] = output[1];
            d2[i] = output[2];
            d3[i] = output[3];
        }
    }

    sourceImage.finishRead(0);
    image.finishWrite(0);
    image.finishWrite(1);
    image.finishWrite(2);
    image.finishWrite(3);
}

void empty(deConversionCPU& cpu)
{
}

void rgb2cmy(deConversionCPU& cpu)
{
    cpu.output[0] = 1.0 - cpu.input[0];
    cpu.output[1] = 1.0 - cpu.input[1];
    cpu.output[2] = 1.0 - cpu.input[2];
}

void bw2rgb(deConversionCPU& cpu)
{
    deValue pg = cpu.registers[CPU_REGISTER_PSEUDOGREY];

    deValue bw = cpu.input[0];

    if (pg == 0)
    {
        cpu.output[0] = bw;
        cpu.output[1] = bw;
        cpu.output[2] = bw;
    }
    else
    {
        int bbw = (int) (255 * bw * 16);
        int m = bbw % 16;

        deValue r = 0;
        deValue g = 0;
        deValue b = 0;

        if ((m >= 2) && (m <= 4))
        {
            b = 1;
        }
        if ((m >= 5) && (m <= 6))
        {
            r = 1;
        }
        if ((m >= 7) && (m <= 8))
        {
            r = 1;
            b = 1;
        }
        if ((m >= 9) && (m <= 10))
        {
            g = 1;
        }
        if ((m >= 11) && (m <= 13))
        {
            g = 1;
            b = 1;
        }
        if ((m >= 14) && (m <= 15))
        {
            g = 1;
            r = 1;
        }

        r = bw + pg * r;
        if (r > 1)
        {
            r = 1;
        }
        g = bw + pg * g;
        if (g > 1)
        {
            g = 1;
        }
        b = bw + pg * b;
        if (b > 1)
        {
            b = 1;
        }
        cpu.output[0] = r;
        cpu.output[1] = g;
        cpu.output[2] = b;
    }
}

void rgb2bw(deConversionCPU& cpu)
{
    deValue r = cpu.input[0];
    deValue g = cpu.input[1];
    deValue b = cpu.input[2];

    deValue mr = cpu.registers[CPU_REGISTER_BW_MIXER_R];
    deValue mg = cpu.registers[CPU_REGISTER_BW_MIXER_G];
    deValue mb = cpu.registers[CPU_REGISTER_BW_MIXER_B];

    deValue bw = mr * r + mg * g + mb * b;

    if (bw < 0)
    {
        bw = 0;
        cpu.incOverflow();
    }
    else if (bw > 1)
    {
        bw = 1;
        cpu.incOverflow();
    }

    cpu.output[0] = bw;
}

void cmy2rgb(deConversionCPU& cpu)
{
    cpu.output[0] = 1.0 - cpu.input[0];
    cpu.output[1] = 1.0 - cpu.input[1];
    cpu.output[2] = 1.0 - cpu.input[2];
}

void cmy2cmyk(deConversionCPU& cpu)
{
    deValue k;
    deValue c = cpu.input[0];
    deValue m = cpu.input[1];
    deValue y = cpu.input[2];

    if (c < m)
    {
        if (c < y)
        {
            k = c;
        }
        else
        {
            k = y;
        }
    }
    else
    {
        if (m < y)
        {
            k = m;
        }
        else
        {
            k = y;
        }
    }

    k = k - cpu.registers[CPU_REGISTER_CMYK_KEY_SUB];

    if (k < 0.0)
    {
        k = 0.0;
    }

    deValue max = cpu.registers[CPU_REGISTER_CMYK_KEY_MAX];
    if (k > max)
    {
        k = max;
    }

    if (c + m + y < cpu.registers[CPU_REGISTER_CMYK_MIN_SUM])
    {
        k = 0.0;
    }

    deValue kk = (1 - k);

    cpu.output[0] = (c - k) / kk;
    cpu.output[1] = (m - k) / kk;
    cpu.output[2] = (y - k) / kk;
    cpu.output[3] = k;
}

void cmyk2cmy(deConversionCPU& cpu)
{
    deValue k = cpu.input[3];
    deValue kk = 1.0 - k;

    deValue c = cpu.input[0] * kk + k;
    deValue m = cpu.input[1] * kk + k;
    deValue y = cpu.input[2] * kk + k;

    cpu.output[0] = c;
    cpu.output[1] = m;
    cpu.output[2] = y;
}

void rgb2prophoto(deConversionCPU& cpu)
{
    /*
         from dcraw

    octave:1> x = [0.529317, 0.330092, 0.140588; 0.098368, 0.873465, 0.028169; 0.016879, 0.117663, 0.865457]
    x =

       0.529317   0.330092   0.140588
       0.098368   0.873465   0.028169
       0.016879   0.117663   0.865457

    */


    deValue r = cpu.input[0];
    deValue g = cpu.input[1];
    deValue b = cpu.input[2];

    cpu.output[0] = 0.529317 * r + 0.330092 * g + 0.140588 * b;
    cpu.output[1] = 0.098368 * r + 0.873465 * g + 0.028169 * b;
    cpu.output[2] = 0.016879 * r + 0.117663 * g + 0.865457 * b;

}

void prophoto2rgb(deConversionCPU& cpu)
{

    /*
        invert matrix from dcraw

    octave:2> inv(x)
    ans =

       2.0341926  -0.7274198  -0.3067655
      -0.2288108   1.2317292  -0.0029216
      -0.0085649  -0.1532726   1.1618390

    */

    deValue pr = cpu.input[0];
    deValue pg = cpu.input[1];
    deValue pb = cpu.input[2];

    deValue r =  2.0341926 * pr -0.7274198  * pg -0.3067655 * pb;
    deValue g = -0.2288108 * pr + 1.2317292 * pg -0.0029216 * pb;
    deValue b = -0.0085649 * pr -0.1532726  * pg + 1.1618390 * pb;

    if (r < 0)
    {
        r = 0;
        cpu.incOverflow();
    }
    else if (r > 1)
    {
        r = 1;
        cpu.incOverflow();
    }
    if (g < 0)
    {
        g = 0;
        cpu.incOverflow();
    }
    else if (g > 1)
    {
        g = 1;
        cpu.incOverflow();
    }
    if (b < 0)
    {
        b = 0;
        cpu.incOverflow();
    }
    else if (b > 1)
    {
        b = 1;
        cpu.incOverflow();
    }

    cpu.output[0] = r;
    cpu.output[1] = g;
    cpu.output[2] = b;

}

void prophoto2xyz(deConversionCPU& cpu)
{

    /*

    octave:7> z
    z =

       2.0341926  -0.7274198  -0.3067655
      -0.2288108   1.2317292  -0.0029216
      -0.0085649  -0.1532726   1.1618390

    octave:8> y
    y =

       0.412400   0.357600   0.180500
       0.212600   0.715200   0.072200
       0.019300   0.119200   0.950500

    octave:10> y * z
    ans =

       0.7555323   0.1128127   0.0821571
       0.2682055   0.7152170   0.0165769
       0.0038447  -0.0129027   1.0980591

    */

    deValue pr = cpu.input[0];
    deValue pg = cpu.input[1];
    deValue pb = cpu.input[2];

    cpu.output[0] = 0.7555323 * pr + 0.1128127* pg + 0.0821571* pb;
    cpu.output[1] = 0.2682055 * pr + 0.7152170* pg + 0.0165769* pb;
    cpu.output[2] = 0.0038447 * pr -0.0129027 * pg + 1.0980591* pb;

}

void rgb2xyz(deConversionCPU& cpu)
{
    deValue r = cpu.input[0];
    deValue g = cpu.input[1];
    deValue b = cpu.input[2];

    cpu.output[0] = 0.4124 * r + 0.3576 * g + 0.1805 * b;
    cpu.output[1] = 0.2126 * r + 0.7152 * g + 0.0722 * b;
    cpu.output[2] = 0.0193 * r + 0.1192 * g + 0.9505 * b;

}

void xyz2rgb(deConversionCPU& cpu)
{
    deValue x = cpu.input[0];
    deValue y = cpu.input[1];
    deValue z = cpu.input[2];

    deValue r =  3.2410 * x - 1.5374 * y - 0.4986 * z;
    deValue g = -0.9692 * x + 1.8760 * y + 0.0416 * z;
    deValue b =  0.0556 * x - 0.2040 * y + 1.0570 * z;

    if (r < 0)
    {
        r = 0;
        cpu.incOverflow();
    }
    else if (r > 1)
    {
        r = 1;
        cpu.incOverflow();
    }
    if (g < 0)
    {
        g = 0;
        cpu.incOverflow();
    }
    else if (g > 1)
    {
        g = 1;
        cpu.incOverflow();
    }
    if (b < 0)
    {
        b = 0;
        cpu.incOverflow();
    }
    else if (b > 1)
    {
        b = 1;
        cpu.incOverflow();
    }

    cpu.output[0] = r;
    cpu.output[1] = g;
    cpu.output[2] = b;
}

void xyz2lab(deConversionCPU& cpu)
{
    static dePower power(1.0 / 3.0, 2);

    deValue xx = cpu.input[0] / Xn;    
    deValue yy = cpu.input[1] / Yn;    
    deValue zz = cpu.input[2] / Zn;    

    deValue fx;
    deValue fy;
    deValue fz;

    if (xx > c6_29_3)
    {
        fx = power.get(xx);
    }
    else
    {
        fx = 1.0 / 3.0 * c29_6_2 * xx + c4_29;
    }

    if (yy > c6_29_3)
    {
        fy = power.get(yy);
    }
    else
    {
        fy = 1.0 / 3.0 * c29_6_2 * yy + c4_29;
    }

    if (zz > c6_29_3)
    {
        fz = power.get(zz);
    }
    else
    {
        fz = 1.0 / 3.0 * c29_6_2 * zz + c4_29;
    }

    deValue l = 116.0 * fy - 16.0;
    deValue a = 500.0 * (fx - fy);
    deValue b = 200.0 * (fy - fz);

    l /= 100.0;
    a += 100.0;
    b += 100.0;
    a /= 200.0;
    b /= 200.0;

    if (l < 0)
    {
        l = 0;
        cpu.incOverflow();
    }
    else if (l > 1)
    {
        l = 1;
        cpu.incOverflow();
    }

    if (a < 0)
    {
        a = 0;
        cpu.incOverflow();
    }
    else if (a > 1)
    {
        a = 1;
        cpu.incOverflow();
    }

    if (b < 0)
    {
        b = 0;
        cpu.incOverflow();
    }
    else if (b > 1)
    {
        b = 1;
        cpu.incOverflow();
    }


    cpu.output[0] = l;
    cpu.output[1] = a;
    cpu.output[2] = b;

}

void lab2lch(deConversionCPU& cpu)
{
    deValue a = cpu.input[1];
    deValue b = cpu.input[2];

    a = ( a - 0.5) * 200.0;
    b = ( b - 0.5) * 200.0;

    deValue _c = sqrt(a * a + b * b);
    deValue _h = atan2(b, a);

    _c = _c / 100.0;
    _h = (_h / ( 2 * M_PI ));

    if (_h < 0)
    {
        _h += 1;
    }

    if ( _c > 1)
    {
        //logError("a: " + str(a) + " b: " + str(b) + " c: " + str(_c));
        _c = 1;
        cpu.incOverflow();
    }
    if ( _c < 0)
    {
        _c = 0;
        cpu.incOverflow();
    }

    cpu.output[0] = cpu.input[0];

    cpu.output[1] = _c;
    cpu.output[2] = _h;
}

void lch2lab(deConversionCPU& cpu)
{
    deValue c = cpu.input[1];
    deValue h = cpu.input[2];

    c = c * 100.0;
    h = h * (2 * M_PI);

    deValue a = c * cos(h);
    deValue b = c * sin(h);

    a = a / 200.0 + 0.5;
    b = b / 200.0 + 0.5;

    cpu.output[0] = cpu.input[0];

    cpu.output[1] = a;
    cpu.output[2] = b;
}

void prophoto2lab(deConversionCPU& cpu)
{
    prophoto2xyz(cpu);
    cpu.switchIO();
    xyz2lab(cpu);
}

void rgb2cmyk(deConversionCPU& cpu)
{
    rgb2cmy(cpu);
    cpu.switchIO();
    cmy2cmyk(cpu);
}

void prophoto2cmyk(deConversionCPU& cpu)
{
    prophoto2rgb(cpu);
    cpu.switchIO();
    rgb2cmyk(cpu);
}


void noConversion(deConversionCPU& cpu)
{
    cpu.switchIO();
}

void rgb2lab(deConversionCPU& cpu)
{
    rgb2xyz(cpu);
    cpu.switchIO();
    xyz2lab(cpu);
}

void rgb2lch(deConversionCPU& cpu)
{
    rgb2lab(cpu);
    cpu.switchIO();
    lab2lch(cpu);
}

void prophoto2lch(deConversionCPU& cpu)
{
    prophoto2lab(cpu);
    cpu.switchIO();
    lab2lch(cpu);
}

void cmyk2rgb(deConversionCPU& cpu)
{
    cmyk2cmy(cpu);
    cpu.switchIO();
    cmy2rgb(cpu);
}

void cmyk2lab(deConversionCPU& cpu)
{
    cmyk2rgb(cpu);
    cpu.switchIO();
    rgb2lab(cpu);
}

void lab2xyz(deConversionCPU& cpu)
{
    deValue l = cpu.input[0];
    deValue a = cpu.input[1];
    deValue b = cpu.input[2];

    l *= 100.0;
    a *= 200.0;
    b *= 200.0;
    a -= 100.0;
    b -= 100.0;

    deValue ll = (l + 16.0) / 116.0;
    deValue ll_aa = ll + a / 500.0;
    deValue ll_bb = ll - b / 200.0;

    deValue ffx;
    deValue ffy;
    deValue ffz;

    if (ll > c6_29)
    {
        ffy = ll * ll * ll;     
    }
    else
    {
        ffy = 3.0 * c6_29_2 * (ll - c4_29);
    }

    if (ll_aa > c6_29)
    {
        ffx = ll_aa * ll_aa * ll_aa;     
    }
    else
    {
        ffx = 3.0 * c6_29_2 * (ll_aa - c4_29);
    }

    if (ll_bb > c6_29)
    {
        ffz = ll_bb * ll_bb * ll_bb;     
    }
    else
    {
        ffz = 3.0 * c6_29_2 * (ll_bb - c4_29);
    }

    cpu.output[0] = Xn * ffx;
    cpu.output[1] = Yn * ffy;
    cpu.output[2] = Zn * ffz;
}    

void xyz2prophoto(deConversionCPU& cpu)
{
    /*

    x = [0.7555323, 0.1128127, 0.0821571; 0.2682055, 0.7152170, 0.0165769; 0.0038447, -0.0129027, 1.0980591]

    octave:2> inv(x)
    ans =

       1.403314  -0.223181  -0.101627
      -0.525984   1.481448   0.016990
      -0.011094   0.018189   0.911253

    */

    deValue x = cpu.input[0];
    deValue y = cpu.input[1];
    deValue z = cpu.input[2];

    cpu.output[0] = 1.403314* x -0.223181* y -0.101627* z;
    cpu.output[1] = -0.525984* x + 1.481448* y + 0.016990* z;
    cpu.output[2] = -0.011094* x + 0.018189* y + 0.911253* z;

}


void lab2prophoto(deConversionCPU& cpu)
{
    lab2xyz(cpu);
    cpu.switchIO();
    xyz2prophoto(cpu);
}

void lch2prophoto(deConversionCPU& cpu)
{
    lch2lab(cpu);
    cpu.switchIO();
    lab2prophoto(cpu);
}


void lab2rgb(deConversionCPU& cpu)
{
    lab2xyz(cpu);
    cpu.switchIO();
    xyz2rgb(cpu);
}

void lab2bw(deConversionCPU& cpu)
{
    lab2rgb(cpu);
    cpu.switchIO();
    rgb2bw(cpu);
}

void cmyk2bw(deConversionCPU& cpu)
{
    cmyk2rgb(cpu);
    cpu.switchIO();
    rgb2bw(cpu);
}

void bw2cmyk(deConversionCPU& cpu)
{
    bw2rgb(cpu);
    cpu.switchIO();
    rgb2cmyk(cpu);
}

void bw2lab(deConversionCPU& cpu)
{
    bw2rgb(cpu);
    cpu.switchIO();
    rgb2lab(cpu);
}

void bw2lch(deConversionCPU& cpu)
{
    bw2lab(cpu);
    cpu.switchIO();
    lab2lch(cpu);
}

void lch2bw(deConversionCPU& cpu)
{
    lch2lab(cpu);
    cpu.switchIO();
    lab2bw(cpu);
}

void lab2cmyk(deConversionCPU& cpu)
{
    lab2rgb(cpu);
    cpu.switchIO();
    rgb2cmy(cpu);
    cpu.switchIO();
    cmy2cmyk(cpu);
}

void lch2rgb(deConversionCPU& cpu)
{
    lch2lab(cpu);
    cpu.switchIO();
    lab2rgb(cpu);
}

void lch2cmyk(deConversionCPU& cpu)
{
    lch2rgb(cpu);
    cpu.switchIO();
    rgb2cmyk(cpu);
}

void cmyk2lch(deConversionCPU& cpu)
{
    cmyk2rgb(cpu);
    cpu.switchIO();
    rgb2lch(cpu);
}

deValue min3(deValue a, deValue b, deValue c)
{
    if (a < b)
    {
        if (a < c)
        {
            return a;   
        }
        else
        {
            return c;
        }
    }
    else
    {
        if (b < c)
        {
            return b;   
        }
        else
        {
            return c;
        }
    }
}

deValue max3(deValue a, deValue b, deValue c)
{
    if (a > b)
    {
        if (a > c)
        {
            return a;   
        }
        else
        {
            return c;
        }
    }
    else
    {
        if (b > c)
        {
            return b;   
        }
        else
        {
            return c;
        }
    }
}    

void rgb2hsl(deConversionCPU& cpu)
{
    deValue r = cpu.input[0];
    deValue g = cpu.input[1];
    deValue b = cpu.input[2];

    deValue minVal = min3( r, g, b );
    deValue maxVal = max3( r, g, b );
    deValue delta = maxVal - minVal;
    deValue sum = minVal + maxVal;

    deValue l = sum / 2.0;
    deValue h;
    deValue s;

    if (delta == 0)
    {
       h = 0;
       s = 0;
    }
    else                                   
    {
        if ( l < 0.5 )
        {
            s = delta / sum;
        }
        else
        {
            s = delta / ( 2.0 - sum );
        }            

        deValue deltaR = ( ( ( maxVal - r ) / 6 ) + ( delta / 2 ) ) / delta;
        deValue deltaG = ( ( ( maxVal - g ) / 6 ) + ( delta / 2 ) ) / delta;
        deValue deltaB = ( ( ( maxVal - b ) / 6 ) + ( delta / 2 ) ) / delta;

        if ( r == maxVal )
        {
            h = deltaB - deltaG;
        }            
        else if ( g == maxVal )
        {
            h = ( 1.0 / 3.0 ) + deltaR - deltaB;
        }            
        else if ( b == maxVal )
        {
            h = ( 2.0 / 3.0 ) + deltaG - deltaR;
        }            

        if ( h < 0 )
            h += 1;
        if ( h > 1 )
            h -= 1;
    }
    cpu.output[0] = h;
    cpu.output[1] = s;
    cpu.output[2] = l;
}

deValue min(deValue a, deValue b, deValue c)
{
    if (a < b)
    {
        if (a < c)
        {
            return a;   
        }
        else
        {
            return c;
        }
    }
    else
    {
        if (b < c)
        {
            return b;   
        }
        else
        {
            return c;
        }
    }
}

deValue max(deValue a, deValue b, deValue c)
{
    if (a > b)
    {
        if (a > c)
        {
            return a;   
        }
        else
        {
            return c;
        }
    }
    else
    {
        if (b > c)
        {
            return b;   
        }
        else
        {
            return c;
        }
    }
}    

deValue hue2rgb( deValue v1, deValue v2, deValue vH ) 
{
   if ( vH < 0 )
        vH += 1;
   if ( vH > 1 ) 
        vH -= 1;
   if ( ( 6.0 * vH ) < 1.0 ) 
        return ( v1 + ( v2 - v1 ) * 6.0 * vH );
   if ( ( 2.0 * vH ) < 1.0 ) 
        return ( v2 );
   if ( ( 3.0 * vH ) < 2.0 ) 
        return ( v1 + ( v2 - v1 ) * ( ( 2.0 / 3.0 ) - vH ) * 6.0 );
   return ( v1 );
}

void hsl2rgb(deConversionCPU& cpu)
{
    deValue h = cpu.input[0];
    deValue s = cpu.input[1];
    deValue l = cpu.input[2];

    deValue r = 0.0;
    deValue g = 0.0;
    deValue b = 0.0;

    if ( s == 0 )                       
    {
        r = l;
        g = l;
        b = l;
    }
    else
    {
        deValue v1;
        deValue v2;

        if ( l < 0.5 )
        {
            v2 = l * ( 1 + s );
        }            
        else
        {
            v2 = ( l + s ) - ( s * l );
        }                

        v1 = 2.0 * l - v2;

        r = hue2rgb( v1, v2, h + ( 1.0 / 3.0 ) );
        g = hue2rgb( v1, v2, h );
        b = hue2rgb( v1, v2, h - ( 1.0 / 3.0 ) );
    }

    cpu.output[0] = r;
    cpu.output[1] = g;
    cpu.output[2] = b;
}

void rgb2hsv(deConversionCPU& cpu)
{
    deValue r = cpu.input[0];
    deValue g = cpu.input[1];
    deValue b = cpu.input[2];

    deValue h = 0.0;
    deValue s = 0.0;
    deValue v = max( r, g, b );

    deValue minVal = min( r, g, b );
    deValue delta = v - minVal;

    if (delta == 0)
    {
       h = 0;
       s = 0;
    }
    else                                   
    {
        s = delta / v;

        deValue deltaR = ( ( ( v - r ) / 6.0 ) + ( delta / 2.0 ) ) / delta;
        deValue deltaG = ( ( ( v - g ) / 6.0 ) + ( delta / 2.0 ) ) / delta;
        deValue deltaB = ( ( ( v - b ) / 6.0 ) + ( delta / 2.0 ) ) / delta;

        if ( r == v )
        {
            h = deltaB - deltaG;
        }            
        else if ( g == v )
        {
            h = ( 1.0 / 3.0 ) + deltaR - deltaB;
        }            
        else if ( b == v )
        {
            h = ( 2.0 / 3.0 ) + deltaG - deltaR;
        }            

        if ( h < 0 )
            h += 1.0;
        if ( h > 1 )
            h -= 1.0;
    }

    cpu.output[0] = h;
    cpu.output[1] = s;
    cpu.output[2] = v;
}

void hsv2rgb(deConversionCPU& cpu)
{
    deValue h = cpu.input[0];
    deValue s = cpu.input[1];
    deValue v = cpu.input[2];

    deValue r = 0.0;
    deValue g = 0.0;
    deValue b = 0.0;
    
    if ( s == 0 )
    {
       r = v;
       g = v;
       b = v;
    }
    else
    {
        deValue var_h = h * 6.0;
    
        if ( var_h == 6.0 )
        {
             var_h = 0;
        }

        deValue var_i = int( var_h);
        deValue var_1 = v * ( 1.0 - s );
        deValue var_2 = v * ( 1.0 - s * ( var_h - var_i ) );
        deValue var_3 = v * ( 1.0 - s * ( 1.0 - ( var_h - var_i ) ) );

        if ( var_i == 0 ) 
        { 
            r = v;
            g = var_3;
            b = var_1;
        }
        else if ( var_i == 1 ) 
        { 
            r = var_2;
            g = v;
            b = var_1;
        }
        else if ( var_i == 2 )
        {
            r = var_1;
            g = v;
            b = var_3;
        }
        else if ( var_i == 3 )
        {
            r = var_1;
            g = var_2;
            b = v;
        }
        else if ( var_i == 4 )
        {
            r = var_3;
            g = var_1;
            b = v;
        }
        else
        {
            r = v;
            g = var_1;
            b = var_2;
        }
    }

    cpu.output[0] = r;
    cpu.output[1] = g;
    cpu.output[2] = b;
}


deConversionCPU::deFunction getConversion(deColorSpace sourceColorSpace, deColorSpace targetColorSpace)
{
    if (sourceColorSpace == targetColorSpace)
    {
        return noConversion;
    }

    if ((sourceColorSpace == deColorSpaceProPhoto) && (targetColorSpace == deColorSpaceRGB))
    {
        return prophoto2rgb;
    }
    if ((targetColorSpace == deColorSpaceProPhoto) && (sourceColorSpace == deColorSpaceRGB))
    {
        return rgb2prophoto;
    }
    if ((sourceColorSpace == deColorSpaceProPhoto) && (targetColorSpace == deColorSpaceLAB))
    {
        return prophoto2lab;
    }
    if ((targetColorSpace == deColorSpaceProPhoto) && (sourceColorSpace == deColorSpaceLAB))
    {
        return lab2prophoto;
    }
    if ((sourceColorSpace == deColorSpaceProPhoto) && (targetColorSpace == deColorSpaceLCH))
    {
        return prophoto2lch;
    }
    if ((targetColorSpace == deColorSpaceProPhoto) && (sourceColorSpace == deColorSpaceLCH))
    {
        return lch2prophoto;
    }
    if ((sourceColorSpace == deColorSpaceRGB) && (targetColorSpace == deColorSpaceLAB))
    {
        return rgb2lab;
    }
    if ((sourceColorSpace == deColorSpaceRGB) && (targetColorSpace == deColorSpaceHSL))
    {
        return rgb2hsl;
    }
    if ((sourceColorSpace == deColorSpaceRGB) && (targetColorSpace == deColorSpaceHSV))
    {
        return rgb2hsv;
    }
    if ((sourceColorSpace == deColorSpaceHSL) && (targetColorSpace == deColorSpaceRGB))
    {
        return hsl2rgb;
    }
    if ((sourceColorSpace == deColorSpaceHSV) && (targetColorSpace == deColorSpaceRGB))
    {
        return hsv2rgb;
    }
    if ((sourceColorSpace == deColorSpaceRGB) && (targetColorSpace == deColorSpaceLCH))
    {
        return rgb2lch;
    }
    if ((sourceColorSpace == deColorSpaceLCH) && (targetColorSpace == deColorSpaceRGB))
    {
        return lch2rgb;
    }
    if ((sourceColorSpace == deColorSpaceLCH) && (targetColorSpace == deColorSpaceCMYK))
    {
        return lch2cmyk;
    }
    if ((targetColorSpace == deColorSpaceLCH) && (sourceColorSpace == deColorSpaceCMYK))
    {
        return cmyk2lch;
    }
    if ((sourceColorSpace == deColorSpaceLAB) && (targetColorSpace == deColorSpaceRGB))
    {
        return lab2rgb;
    }
    if ((sourceColorSpace == deColorSpaceLAB) && (targetColorSpace == deColorSpaceLCH))
    {
        return lab2lch;
    }
    if ((sourceColorSpace == deColorSpaceLCH) && (targetColorSpace == deColorSpaceLAB))
    {
        return lch2lab;
    }
    if ((sourceColorSpace == deColorSpaceProPhoto) && (targetColorSpace == deColorSpaceCMYK))
    {
        return prophoto2cmyk;
    }
    if ((sourceColorSpace == deColorSpaceRGB) && (targetColorSpace == deColorSpaceCMYK))
    {
        return rgb2cmyk;
    }
    if ((sourceColorSpace == deColorSpaceCMYK) && (targetColorSpace == deColorSpaceRGB))
    {
        return cmyk2rgb;
    }
    if ((sourceColorSpace == deColorSpaceRGB) && (targetColorSpace == deColorSpaceBW))
    {
        return rgb2bw;
    }
    if ((sourceColorSpace == deColorSpaceProPhoto) && (targetColorSpace == deColorSpaceBW))
    {
        return rgb2bw;
    }
    if ((sourceColorSpace == deColorSpaceLAB) && (targetColorSpace == deColorSpaceBW))
    {
        return lab2bw;
    }
    if ((sourceColorSpace == deColorSpaceLCH) && (targetColorSpace == deColorSpaceBW))
    {
        return lch2bw;
    }
    if ((sourceColorSpace == deColorSpaceCMYK) && (targetColorSpace == deColorSpaceBW))
    {
        return cmyk2bw;
    }
    if ((sourceColorSpace == deColorSpaceBW) && (targetColorSpace == deColorSpaceRGB))
    {
        return bw2rgb;
    }
    if ((sourceColorSpace == deColorSpaceCMYK) && (targetColorSpace == deColorSpaceLAB))
    {
        return cmyk2lab;
    }
    if ((sourceColorSpace == deColorSpaceLAB) && (targetColorSpace == deColorSpaceCMYK))
    {
        return lab2cmyk;
    }
    if ((targetColorSpace == deColorSpaceLAB) && (sourceColorSpace == deColorSpaceBW))
    {
        return bw2lab;
    }
    if ((targetColorSpace == deColorSpaceLCH) && (sourceColorSpace == deColorSpaceBW))
    {
        return bw2lch;
    }
    if ((targetColorSpace == deColorSpaceCMYK) && (sourceColorSpace == deColorSpaceBW))
    {
        return bw2cmyk;
    }

    logError("no conversion for " + getColorSpaceName(sourceColorSpace) + " -> " + getColorSpaceName(targetColorSpace));
    return NULL;

}



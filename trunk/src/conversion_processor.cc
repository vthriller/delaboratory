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

#include "copy_image.h"
#include "image.h"
#include "channel_manager.h"
#include "color_space_utils.h"

#include "logger.h"

#include "power.h"

#include "str.h"

void applyContrastRGB(deConversionCPU& cpu)
{
    deValue a = cpu.registers[CPU_REGISTER_CONTRAST];
    deValue b = 0.5 - 0.5 * a;

    cpu.input[0] = a * cpu.input[0] + b;
    cpu.input[1] = a * cpu.input[1] + b;
    cpu.input[2] = a * cpu.input[2] + b;
}

void applyContrastSaturationLAB(deConversionCPU& cpu)
{
    deValue a1 = cpu.registers[CPU_REGISTER_CONTRAST];
    deValue b1 = 0.5 - 0.5 * a1;
    deValue a2 = cpu.registers[CPU_REGISTER_SATURATION];
    deValue b2 = 0.5 - 0.5 * a2;

    cpu.input[0] = a1 * cpu.input[0] + b1;
    cpu.input[1] = a2 * cpu.input[1] + b2;
    cpu.input[2] = a2 * cpu.input[2] + b2;
}

void applyContrastSaturationLCH(deConversionCPU& cpu)
{
    deValue a1 = cpu.registers[CPU_REGISTER_CONTRAST];
    deValue b1 = 0.5 - 0.5 * a1;
    deValue a2 = cpu.registers[CPU_REGISTER_SATURATION];

    cpu.input[0] = a1 * cpu.input[0] + b1;
    cpu.input[1] = a2 * cpu.input[1];
}



deConversionProcessor::deConversionProcessor()
{
}

deConversionProcessor::~deConversionProcessor()
{
}


int deConversionProcessor::convertImageNew(const deImage& sourceImage, deImage& image)
{
    deColorSpace sourceColorSpace = sourceImage.getColorSpace();
    deColorSpace targetColorSpace = image.getColorSpace();

    if (sourceColorSpace == targetColorSpace)
    {
        copyImage(sourceImage, image);
        return true;
    }

    deConversionCPU cpu(4);
    cpu.registers[CPU_REGISTER_OVERFLOW] = 0;

    int ss = getColorSpaceSize(sourceColorSpace);
    int ds = getColorSpaceSize(targetColorSpace);

    deConversionCPU::deFunction f1 = empty;
    deConversionCPU::deFunction f2 = getConversion(sourceColorSpace, targetColorSpace);

    if ((ss == 3) && (ds == 3))
    {
        if (f2)
        {
            cpu.convertImage3x3(sourceImage, image, f1, f2);
        }            
    }
    else
    if ((ss == 3) && (ds == 4))
    {
        if (f2)
        {
            cpu.convertImage3x4(sourceImage, image, f1, f2);
        }            
    }
    else
    if ((ss == 4) && (ds == 3))
    {
        if (f2)
        {
            cpu.convertImage4x3(sourceImage, image, f1, f2);
        }            
    }
    else
    if ((ss == 3) && (ds == 1))
    {
        if (f2)
        {
            cpu.convertImage3x1(sourceImage, image, f1, f2);
        }            
    }
    else
    if ((ss == 1) && (ds == 3))
    {
        if (f2)
        {
            cpu.convertImage1x3(sourceImage, image, f1, f2);
        }            
    }
    if ((ss == 4) && (ds == 1))
    {
        if (f2)
        {
            cpu.convertImage4x1(sourceImage, image, f1, f2);
        }            
    }
    else
    if ((ss == 1) && (ds == 4))
    {
        if (f2)
        {
            cpu.convertImage1x4(sourceImage, image, f1, f2);
        }            
    }

    deValue overflow = cpu.registers[CPU_REGISTER_OVERFLOW];
    int n = sourceImage.getChannelSize().getN();
    int percentage = overflow * 10000 / n;
    return percentage;

}

bool deConversionProcessor::convert(deColorSpace sourceColorSpace, deValue v1, deValue v2, deValue v3, deValue v4, deColorSpace targetColorSpace, deValue &r1, deValue& r2, deValue& r3, deValue& r4)
{
    deConversionCPU cpu(4);
    deConversionCPU::deFunction f = getConversion(sourceColorSpace, targetColorSpace);

    if (!f)
    {
        return false;
    }
    
    cpu.input[0] = v1;
    cpu.input[1] = v2;
    cpu.input[2] = v3;
    cpu.input[3] = v4;

    f(cpu);

    r1 = cpu.output[0];
    r2 = cpu.output[1];
    r3 = cpu.output[2];
    r4 = cpu.output[3];

    return true;
}

bool deConversionProcessor::renderImageToRGBNew(const deImage& image, unsigned char* data)
{
    deColorSpace colorSpace = image.getColorSpace();

    deConversionCPU cpu(4);
    cpu.registers[CPU_REGISTER_OVERFLOW] = 0;

    int s = getColorSpaceSize(colorSpace);

    deConversionCPU::deFunction f = getConversion(colorSpace, deColorSpaceRGB);

    if (!f)
    {
        return false;
    }

    if (s == 3)
    {
        return cpu.renderImage3(image, f, data);
    } else
    if (s == 4)
    {
        return cpu.renderImage4(image, f, data);
    } else
    if (s == 1)
    {
        return cpu.renderImage1(image, f, data);
    };

    return false;

}

void deConversionProcessor::convertImage(const deImage& sourceImage, deImage& image, deConversionCPU& cpu)
{
    deColorSpace sourceColorSpace = sourceImage.getColorSpace();
    deColorSpace targetColorSpace = image.getColorSpace();

    if (sourceColorSpace == targetColorSpace)
    {
        copyImage(sourceImage, image);
        return;
    }

    int ss = getColorSpaceSize(sourceColorSpace);
    int ds = getColorSpaceSize(targetColorSpace);

    deConversionCPU::deFunction f1 = empty;

    if (sourceColorSpace == deColorSpaceProPhoto)
    {
        if (cpu.registers[CPU_REGISTER_CONTRAST] < 1.0)
        {
            f1 = applyContrastRGB;
        }
    }
    if (sourceColorSpace == deColorSpaceLAB)
    {
        if ((cpu.registers[CPU_REGISTER_SATURATION] < 1.0) || (cpu.registers[CPU_REGISTER_CONTRAST] < 1.0))
        {
            f1 = applyContrastSaturationLAB;
        }
    }
    if (sourceColorSpace == deColorSpaceLCH)
    {
        if ((cpu.registers[CPU_REGISTER_SATURATION] < 1.0) || (cpu.registers[CPU_REGISTER_CONTRAST] < 1.0))
        {
            f1 = applyContrastSaturationLCH;
        }
    }

    deConversionCPU::deFunction f2 = getConversion(sourceColorSpace, targetColorSpace);

    if ((ss == 3) && (ds == 3))
    {
        if (f2)
        {
            cpu.convertImage3x3(sourceImage, image, f1, f2);
        }            
    }
    else
    if ((ss == 3) && (ds == 4))
    {
        if (f2)
        {
            cpu.convertImage3x4(sourceImage, image, f1, f2);
        }            
    }
    else
    if ((ss == 4) && (ds == 3))
    {
        if (f2)
        {
            cpu.convertImage4x3(sourceImage, image, f1, f2);
        }            
    }
    else
    if ((ss == 3) && (ds == 1))
    {
        if (f2)
        {
            cpu.convertImage3x1(sourceImage, image, f1, f2);
        }            
    }
    else
    if ((ss == 1) && (ds == 3))
    {
        if (f2)
        {
            cpu.convertImage1x3(sourceImage, image, f1, f2);
        }            
    }
    if ((ss == 4) && (ds == 1))
    {
        if (f2)
        {
            cpu.convertImage4x1(sourceImage, image, f1, f2);
        }            
    }
    else
    if ((ss == 1) && (ds == 4))
    {
        if (f2)
        {
            cpu.convertImage1x4(sourceImage, image, f1, f2);
        }            
    }

}

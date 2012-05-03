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
#include "conversion_functions.h"
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

bool deConversionProcessor::convertPixel(const deImage& image, int p, deColorSpace colorSpace, deValue &v1, deValue& v2, deValue& v3, deValue& v4)
{
    deColorSpace sourceColorSpace = image.getColorSpace();
    int sn = getColorSpaceSize(sourceColorSpace);
    int tn = getColorSpaceSize(colorSpace);

    if ((sn == 3) && (tn == 3))
    {
        deConversion3x3 conversion = getConversion3x3(sourceColorSpace, colorSpace);

        if (conversion)
        {
            const deValue* values1 = image.getValues(0);
            if (!values1)
            {
                return false;
            }
            const deValue* values2 = image.getValues(1);
            if (!values2)
            {
                return false;
            }
            const deValue* values3 = image.getValues(2);
            if (!values3)
            {
                return false;
            }
            deValue s1 = values1[p];
            deValue s2 = values2[p];
            deValue s3 = values3[p];
            conversion(s1, s2, s3, v1, v2, v3);
            v4 = -1;
            return true;
        } 
    }        

    if ((sn == 3) && (tn == 1))
    {
        deConversion3x1 conversion = getConversion3x1(sourceColorSpace, colorSpace);

        if (conversion)
        {
            const deValue* values1 = image.getValues(0);
            if (!values1)
            {
                return false;
            }
            const deValue* values2 = image.getValues(1);
            if (!values2)
            {
                return false;
            }
            const deValue* values3 = image.getValues(2);
            if (!values3)
            {
                return false;
            }
            deValue s1 = values1[p];
            deValue s2 = values2[p];
            deValue s3 = values3[p];
            conversion(s1, s2, s3, v1);
            return true;
        } 
    }        

    if ((sn == 3) && (tn == 4))
    {
        deConversion3x4 conversion = getConversion3x4(sourceColorSpace, colorSpace);

        if (conversion)
        {
            const deValue* values1 = image.getValues(0);
            if (!values1)
            {
                return false;
            }
            const deValue* values2 = image.getValues(1);
            if (!values2)
            {
                return false;
            }
            const deValue* values3 = image.getValues(2);
            if (!values3)
            {
                return false;
            }
            deValue s1 = values1[p];
            deValue s2 = values2[p];
            deValue s3 = values3[p];
            conversion(s1, s2, s3, v1, v2, v3, v4);
            return true;
        } 
    }        

    if ((sn == 4) && (tn == 4))
    {
        const deValue* values1 = image.getValues(0);
        if (!values1)
        {
            return false;
        }
        const deValue* values2 = image.getValues(1);
        if (!values2)
        {
            return false;
        }
        const deValue* values3 = image.getValues(2);
        if (!values3)
        {
            return false;
        }
        const deValue* values4 = image.getValues(3);
        if (!values4)
        {
            return false;
        }
        deValue s1 = values1[p];
        deValue s2 = values2[p];
        deValue s3 = values3[p];
        deValue s4 = values4[p];
        v1 = s1;
        v2 = s2;
        v3 = s3;
        v4 = s4;
        return true;
    }        

    if ((sn == 4) && (tn == 3))
    {
        deConversion4x3 conversion = getConversion4x3(sourceColorSpace, colorSpace);

        if (conversion)
        {
            const deValue* values1 = image.getValues(0);
            if (!values1)
            {
                return false;
            }
            const deValue* values2 = image.getValues(1);
            if (!values2)
            {
                return false;
            }
            const deValue* values3 = image.getValues(2);
            if (!values3)
            {
                return false;
            }
            const deValue* values4 = image.getValues(3);
            if (!values4)
            {
                return false;
            }
            deValue s1 = values1[p];
            deValue s2 = values2[p];
            deValue s3 = values3[p];
            deValue s4 = values4[p];
            conversion(s1, s2, s3, s4, v1, v2, v3);
            return true;
        } 
    }        

    if ((sn == 1) && (tn == 3))
    {
        deConversion1x3 conversion = getConversion1x3(sourceColorSpace, colorSpace);

        if (conversion)
        {
            const deValue* values1 = image.getValues(0);
            if (!values1)
            {
                return false;
            }
            deValue s1 = values1[p];
            conversion(s1, v1, v2, v3);
            return true;
        } 
    }        

    if ((sn == 1) && (tn == 4))
    {
        deConversion1x4 conversion = getConversion1x4(sourceColorSpace, colorSpace);

        if (conversion)
        {
            const deValue* values1 = image.getValues(0);
            if (!values1)
            {
                return false;
            }
            deValue s1 = values1[p];
            conversion(s1, v1, v2, v3, v4);
            return true;
        } 
    }        

    if ((sn == 1) && (tn == 1))
    {
        const deValue* values1 = image.getValues(0);
        if (!values1)
        {
            return false;
        }
        deValue s1 = values1[p];
        v1 = s1;
        return true;
    }        

    return false;
}

bool deConversionProcessor::convertToRGB(deValue v1, deValue v2, deValue v3, deValue v4, deColorSpace colorSpace, deValue &rr, deValue& gg, deValue& bb)
{
    deConversion1x3 f1x3 = getConversion1x3(colorSpace, deColorSpaceRGB);
    deConversion3x3 f3x3 = getConversion3x3(colorSpace, deColorSpaceRGB);
    deConversion4x3 f4x3 = getConversion4x3(colorSpace, deColorSpaceRGB);
    if (f3x3)
    {
        f3x3(v1, v2, v3, rr, gg, bb);
        return true;
    } else if (f4x3)
    {
        f4x3(v1, v2, v3, v4, rr, gg, bb);
        return true;
    } else if (f1x3)
    {
        f1x3(v1, rr, gg, bb);
        return true;
    }
    return false;
}

bool deConversionProcessor::convertToLAB(deValue v1, deValue v2, deValue v3, deValue v4, deColorSpace colorSpace, deValue &rr, deValue& gg, deValue& bb)
{
    deConversion1x3 f1x3 = getConversion1x3(colorSpace, deColorSpaceLAB);
    deConversion3x3 f3x3 = getConversion3x3(colorSpace, deColorSpaceLAB);
    deConversion4x3 f4x3 = getConversion4x3(colorSpace, deColorSpaceLAB);
    if (f3x3)
    {
        f3x3(v1, v2, v3, rr, gg, bb);
        return true;
    } else if (f4x3)
    {
        f4x3(v1, v2, v3, v4, rr, gg, bb);
        return true;
    } else if (f1x3)
    {
        f1x3(v1, rr, gg, bb);
        return true;
    }
    return false;
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

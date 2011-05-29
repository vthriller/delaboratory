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

#include "converter.h"
#include "preview.h"
#include "channel.h"
#include "convert_color.h"
#include "exception.h"

deConverter::deConverter()
:size(0,0)
{
    source = NULL;
    destination = NULL;
    n = -1;
    data = NULL;
    dataPosition = -1;

    conversionFunction = NULL;
}

deConverter::~deConverter()
{
}

void deConverter::setSource(const dePreview* _source)
{
    source = _source;
    size = source->getSize();
    n = size.getN();
    sourceColorSpace = source->getColorSpace();
}

void deConverter::setDestination(dePreview* _destination)
{
    destination = _destination;
    destinationColorSpace = destination->getColorSpace();
}

void deConverter::setData(unsigned char* _data)
{
    data = _data;
    dataPosition = 0;
}

void deConverter::storeSingleChannel(int channel)
{
    const deBaseChannel* channelG = source->getChannel(channel);

    for (index = 0; index < n; index++)
    {
        deValue gg = channelG->getValue(index);
        values[4] = gg;
        values[5] = gg;
        values[6] = gg;

        storeRGB();
    }
}

void deConverter::storeAllChannels()
/* this code is used for realtime rendering, must be quite fast */
{
    const deBaseChannel* channel0 = source->getChannel(0);
    const deBaseChannel* channel1 = source->getChannel(1);
    const deBaseChannel* channel2 = source->getChannel(2);
    const deBaseChannel* channel3 = source->getChannel(3);

    destinationColorSpace = deColorSpaceRGB;

    if (!setConversionFunction())
    {
        return;
    }

    int index;

    if (channel3)
    {
        for (index = 0; index < n; index++)
        {
            values[0] = channel0->getValue(index);
            values[1] = channel1->getValue(index);
            values[2] = channel2->getValue(index);
            values[3] = channel3->getValue(index);
            conversionFunction(values);
            storeRGB();
        }
    }        
    else
    {
        for (index = 0; index < n; index++)
        {
            values[0] = channel0->getValue(index);
            values[1] = channel1->getValue(index);
            values[2] = channel2->getValue(index);
            conversionFunction(values);
            storeRGB();
        }
    }        
}

void deConverter::convert()
{
    const deBaseChannel* channel0 = source->getChannel(0);
    const deBaseChannel* channel1 = source->getChannel(1);
    const deBaseChannel* channel2 = source->getChannel(2);
    const deBaseChannel* channel3 = source->getChannel(3);

    deBaseChannel* channelD0 = destination->getChannel(0);
    deBaseChannel* channelD1 = destination->getChannel(1);
    deBaseChannel* channelD2 = destination->getChannel(2);
    deBaseChannel* channelD3 = destination->getChannel(3);

    if (!setConversionFunction())
    {
        return;
    }

    int index;
    for (index = 0; index < n; index++)
    {
        values[0] = channel0->getValue(index);
        values[1] = channel1->getValue(index);
        values[2] = channel2->getValue(index);
        values[3] = channel3->getValue(index);

        conversionFunction(values);

        channelD0->setValue(index, values[4]);
        channelD1->setValue(index, values[5]);
        channelD2->setValue(index, values[6]);
        channelD3->setValue(index, values[7]);
    }
}

void deConverter::storeRGB()
{
    deValue rr = 255 * values[4];
    deValue gg = 255 * values[5];
    deValue bb = 255 * values[6];

    if ( rr > 255)
    {
        data[dataPosition] = 255;
    }
    else if (rr < 0)
    {
        data[dataPosition] = 0;
    }
    else
    {
        data[dataPosition] = rr;
    }
    dataPosition++;

    if ( gg > 255)
    {
        data[dataPosition] = 255;
    }
    else if (gg < 0)
    {
        data[dataPosition] = 0;
    }
    else
    {
        data[dataPosition] = gg;
    }
    dataPosition++;

    if ( bb > 255)
    {
        data[dataPosition] = 255;
    }
    else if (bb < 0)
    {
        data[dataPosition] = 0;
    }
    else
    {
        data[dataPosition] = bb;
    }
    dataPosition++;

}

bool deConverter::setConversionFunction()
{
    if (sourceColorSpace == destinationColorSpace)
    {
        conversionFunction = simpleCopy;
        return true;
    }

    switch (sourceColorSpace)
    {
        case deColorSpaceRGB:
            switch (destinationColorSpace)
            {
                case deColorSpaceCMYK:
                {
                    conversionFunction = rgb2cmyk;
                    return true;
                }
                case deColorSpaceCMY:
                {
                    conversionFunction = rgb2cmy;
                    return true;
                }
                case deColorSpaceXYZ:
                {
                    conversionFunction = rgb2xyz;
                    return true;
                }
                case deColorSpaceLAB:
                {
                    conversionFunction = rgb2lab;
                    return true;
                }
                case deColorSpaceHSL:
                {
                    conversionFunction = rgb2hsl;
                    return true;
                }
                case deColorSpaceHSV:
                {
                    conversionFunction = rgb2hsv;
                    return true;
                }
                case deColorSpaceBW:
                {
                    conversionFunction = rgb2bw;
                    return true;
                }
                default:                    
                    return false;                        
            }
            break;
        case deColorSpaceHSL:
            switch (destinationColorSpace)
            {
                case deColorSpaceRGB:
                {
                    conversionFunction = hsl2rgb;
                    return true;
                }
                default:                    
                    return false;
            }
            break;
        case deColorSpaceBW:
            switch (destinationColorSpace)
            {
                case deColorSpaceRGB:
                {
                    conversionFunction = bw2rgb;
                    return true;
                }
                default:                    
                    return false;                        
            }
            break;
        case deColorSpaceHSV:
            switch (destinationColorSpace)
            {
                case deColorSpaceRGB:
                {
                    conversionFunction = hsv2rgb;
                    return true;
                }
                default:                    
                    return false;                        
            }
            break;
        case deColorSpaceXYZ:
            switch (destinationColorSpace)
            {
                case deColorSpaceRGB:
                {
                    conversionFunction = xyz2rgb;
                    return true;
                }
                case deColorSpaceLAB:
                {
                    conversionFunction = xyz2lab;
                    return true;
                }
                default:                    
                    return false;                        
            }
            break;
        case deColorSpaceLAB:
            switch (destinationColorSpace)
            {
                case deColorSpaceRGB:
                {
                    conversionFunction = lab2rgb;
                    return true;
                }
                case deColorSpaceXYZ:
                {
                    conversionFunction = lab2xyz;
                    return true;
                }
                case deColorSpaceCMYK:
                {
                    conversionFunction = lab2cmyk;
                    return true;
                }
                default:                    
                    return false;                        
            }
            break;
        case deColorSpaceCMY:
            switch (destinationColorSpace)
            {
                case deColorSpaceCMYK:
                {
                    conversionFunction = cmy2cmyk;
                    return true;
                }
                case deColorSpaceRGB:
                {
                    conversionFunction = cmy2rgb;
                    return true;
                }
                default:                    
                    return false;                        
            }
            break;
        case deColorSpaceCMYK:
            switch (destinationColorSpace)
            {
                case deColorSpaceRGB:
                {
                    conversionFunction = cmyk2rgb;
                    return true;
                }
                case deColorSpaceLAB:
                {
                    conversionFunction = cmyk2lab;
                    return true;
                }
                case deColorSpaceCMY:
                {
                    conversionFunction = cmyk2cmy;
                    return true;
                }
                default:                    
                    return false;                        
            }
            break;
        default:
            return false;                        
    }

}

bool deConverter::getPixel(int pos, deColorSpace colorSpace, deValue& v0, deValue& v1, deValue& v2, deValue& v3)
{
    const deBaseChannel* channel0 = source->getChannel(0);
    const deBaseChannel* channel1 = source->getChannel(1);
    const deBaseChannel* channel2 = source->getChannel(2);
    const deBaseChannel* channel3 = source->getChannel(3);

    destinationColorSpace = colorSpace;

    bool result = setConversionFunction();
    if (!result)
    {
        return false;
    }

    values[0] = channel0->getValue(pos);
    values[1] = channel1->getValue(pos);
    values[2] = channel2->getValue(pos);
    values[3] = channel3->getValue(pos);
    conversionFunction(values);

    v0 = values[4];
    v1 = values[5];
    v2 = values[6];
    v3 = values[7];

    return true;
}

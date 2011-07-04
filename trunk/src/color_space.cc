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

#include "color_space.h"
#include "exception.h"
#include "channels.h"
#include "preview.h"
#include "converter.h"

int getColorSpaceSize(const deColorSpace& colorSpace)
{
    switch (colorSpace)
    {
        case deColorSpaceBW:
            return 1;
        case deColorSpaceRGB:
            return 3;
        case deColorSpaceHSL:
            return 3;
        case deColorSpaceHSV:
            return 3;
        case deColorSpaceLAB:
            return 3;
        case deColorSpaceXYZ:
            return 3;
        case deColorSpaceCMY:
            return 3;
        case deColorSpaceCMYK:
            return 4;
        default:
            return 0;
//            throw deException("unsupported ColorSpace " + getColorSpaceName(colorSpace) + " in getColorSpaceSize");
    }
}

void getColorSpaceChannelRanges(const deColorSpace& colorSpace, int index, deValue& min, deValue& max)
{
    min = 0;
    max = 1;
}

std::string getColorSpaceName(deColorSpace colorSpace)
{
    switch (colorSpace)
    {
        case deColorSpaceInvalid:
            return "invalid";
        case deColorSpaceBW:
            return "BW";
        case deColorSpaceRGB:
            return "RGB";
        case deColorSpaceHSL:
            return "HSL";
        case deColorSpaceHSV:
            return "HSV";
        case deColorSpaceLAB:
            return "LAB";
        case deColorSpaceXYZ:
            return "XYZ";
        case deColorSpaceCMY:
            return "CMY";
        case deColorSpaceCMYK:
            return "CMYK";
        default:
            return "unknown";
    }
}

deColorSpace colorSpaceFromString(const std::string& name)
{
    if (name == "RGB")
    {
        return deColorSpaceRGB;
    }
    if (name == "LAB")
    {
        return deColorSpaceLAB;
    }
    if (name == "CMYK")
    {
        return deColorSpaceCMYK;
    }

    return deColorSpaceInvalid;
}

std::string getChannelName(deColorSpace colorSpace, int channel)
{
    switch (colorSpace)
    {
        case deColorSpaceInvalid:
            return "invalid";
        case deColorSpaceBW:
            return "BW";
        case deColorSpaceRGB:
        {
            switch (channel)
            {
                case DE_CHANNEL_RED:
                    return "red";
                case DE_CHANNEL_GREEN:
                    return "green";
                case DE_CHANNEL_BLUE:
                    return "blue";
            }                    
        }            
        case deColorSpaceLAB:
            switch (channel)
            {
                case DE_CHANNEL_L:
                    return "L";
                case DE_CHANNEL_A:
                    return "A";
                case DE_CHANNEL_B:
                    return "B";
            }                    
        case deColorSpaceXYZ:
            switch (channel)
            {
                case DE_CHANNEL_X:
                    return "X";
                case DE_CHANNEL_Y:
                    return "Y";
                case DE_CHANNEL_Z:
                    return "Z";
            }                    
        case deColorSpaceHSL:
            switch (channel)
            {
                case DE_CHANNEL_HUE:
                    return "hue";
                case DE_CHANNEL_SATURATION:
                    return "saturation";
                case DE_CHANNEL_LIGHTNESS:
                    return "lightness";
            }                    
        case deColorSpaceHSV:
            switch (channel)
            {
                case DE_CHANNEL_HUE:
                    return "hue";
                case DE_CHANNEL_SATURATION:
                    return "saturation";
                case DE_CHANNEL_VALUE:
                    return "value";
            }                    
        case deColorSpaceCMY:
        case deColorSpaceCMYK:
            switch (channel)
            {
                case DE_CHANNEL_CYAN:
                    return "cyan";
                case DE_CHANNEL_MAGENTA:
                    return "magenta";
                case DE_CHANNEL_YELLOW:
                    return "yellow";
                case DE_CHANNEL_KEY:
                    return "key";
            }                    
        default:
            return "unknown";
    }
}

void getSupportedColorSpaces(std::vector<deColorSpace>& result)
{
    result.push_back(deColorSpaceRGB);
    result.push_back(deColorSpaceBW);
    result.push_back(deColorSpaceLAB);
    result.push_back(deColorSpaceXYZ);
    result.push_back(deColorSpaceCMY);
    result.push_back(deColorSpaceCMYK);
    result.push_back(deColorSpaceHSL);
    result.push_back(deColorSpaceHSV);
}

void getSupportedConversions(const deColorSpace& colorSpace, std::vector<deColorSpace>& result)
{
    deConverter converter;

    dePreview sourcePreview(colorSpace, deSize(0,0));
    converter.setSource(&sourcePreview);

    std::vector<deColorSpace> all;
    getSupportedColorSpaces(all);
    std::vector<deColorSpace>::iterator i;
    for (i = all.begin(); i != all.end(); i++)
    {
        deColorSpace destination = *i;
        dePreview destinationPreview(destination, deSize(0,0));
        converter.setDestination(&destinationPreview);
        if (converter.setConversionFunction())
        {
            result.push_back(destination);
        }                
    }
}
wxColour getChannelwxColour(deColorSpace colorSpace, int channel)
{
    // FIXME add different colors for different colorspaces
    switch (channel)
    {
        case 0:
            return wxColour(255, 0, 0);
        case 1:
            return wxColour(0, 255, 0);
        case 2:
            return wxColour(0, 0, 255);
        case 3:
            return wxColour(255, 255, 255);
        default:            
            return wxColour(255, 255, 255);
    }
}


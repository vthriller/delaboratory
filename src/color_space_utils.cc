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

#include "color_space_utils.h"
#include "channels.h"

int getEqualizerChannel(deColorSpace colorSpace)
{
    switch (colorSpace)
    {
        case deColorSpaceBW:
            return 0;
        case deColorSpaceRGB:
            return -1;
        case deColorSpaceProPhoto:
            return -1;
        case deColorSpaceHSL:
            return 0;
        case deColorSpaceHSV:
            return 0;
        case deColorSpaceLAB:
            return 0;
        case deColorSpaceLCH:
            return 2;
        case deColorSpaceCMY:
            return -1;
        case deColorSpaceCMYK:
            return 3;
        default:
            return 0;
    }
}

int getColorSpaceSize(const deColorSpace& colorSpace)
{
    switch (colorSpace)
    {
        case deColorSpaceBW:
            return 1;
        case deColorSpaceRGB:
            return 3;
        case deColorSpaceProPhoto:
            return 3;
        case deColorSpaceHSL:
            return 3;
        case deColorSpaceHSV:
            return 3;
        case deColorSpaceLAB:
            return 3;
        case deColorSpaceLCH:
            return 3;
        case deColorSpaceXYZ:
            return 3;
        case deColorSpaceCMY:
            return 3;
        case deColorSpaceCMYK:
            return 4;
        default:
            return 0;
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
            return "sRGB";
        case deColorSpaceProPhoto:
            return "ProPhoto";
        case deColorSpaceHSL:
            return "HSL";
        case deColorSpaceHSV:
            return "HSV";
        case deColorSpaceLAB:
            return "LAB";
        case deColorSpaceLCH:
            return "LCH";
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
    if (name == "sRGB")
    {
        return deColorSpaceRGB;
    }
    if (name == "ProPhoto")
    {
        return deColorSpaceProPhoto;
    }
    if (name == "LAB")
    {
        return deColorSpaceLAB;
    }
    if (name == "LCH")
    {
        return deColorSpaceLCH;
    }
    if (name == "CMYK")
    {
        return deColorSpaceCMYK;
    }
    if (name == "HSL")
    {
        return deColorSpaceHSL;
    }
    if (name == "HSV")
    {
        return deColorSpaceHSV;
    }
    if (name == "BW")
    {
        return deColorSpaceBW;
    }
    if (name == "XYZ")
    {
        return deColorSpaceXYZ;
    }
    if (name == "CMY")
    {
        return deColorSpaceCMY;
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
        case deColorSpaceProPhoto:
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
        case deColorSpaceLCH:
            switch (channel)
            {
                case DE_CHANNEL_L:
                    return "lightness";
                case DE_CHANNEL_C:
                    return "chroma";
                case DE_CHANNEL_H:
                    return "hue";
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
    result.push_back(deColorSpaceProPhoto);
    result.push_back(deColorSpaceBW);
    result.push_back(deColorSpaceLAB);
    result.push_back(deColorSpaceLCH);
    result.push_back(deColorSpaceHSL);
    result.push_back(deColorSpaceHSV);
    result.push_back(deColorSpaceCMYK);
}

deValue getPresentationValue(deColorSpace colorSpace, int channel, deValue v)
{
    if (colorSpace == deColorSpaceLAB)
    {
        if (channel == DE_CHANNEL_L)
        {
            return 100 * v;
        }
        else
        {
            return 200 * v - 100.0;
        }
    }
    if (colorSpace == deColorSpaceRGB)
    {
        return 255 * v;
    }
    if (colorSpace == deColorSpaceProPhoto)
    {
        return 255 * v;
    }
    if ((colorSpace == deColorSpaceCMYK) || (colorSpace == deColorSpaceCMY))
    {
        return 100 * v;
    }
    if ((colorSpace == deColorSpaceHSL) || (colorSpace == deColorSpaceHSV))
    {
        if (channel == DE_CHANNEL_HUE)
        {
           return 360 * v;
        }       
        else
        {
           return 100 * v;
        }
    }
    if (colorSpace == deColorSpaceLCH)
    {
        if (channel == DE_CHANNEL_H)
        {
           return 360 * v;
        }       
        else
        {
           return 100 * v;
        }
    }
    return v;
}

bool shouldChannelBeSharpened(deColorSpace colorSpace, int channel)
{
    switch (colorSpace)
    {
        case deColorSpaceLAB:
        case deColorSpaceLCH:
        {
            if (channel == 0)
            {
                return true;
            }
            break;
        }
        case deColorSpaceHSV:
        case deColorSpaceHSL:
        {
            if (channel == 2)
            {
                return true;
            }
            break;
        }
        case deColorSpaceCMYK:
        {
            if (channel == 3)
            {
                return true;
            }
            break;
        }
        default:
        {
            return true;
            break;
        }
    }

    return false;
}

bool isChannelLuminance(deColorSpace colorSpace, int channel)
{
    switch (colorSpace)
    {
        case deColorSpaceLAB:
        case deColorSpaceLCH:
        {
            if (channel == 0)
            {
                return true;
            }
            break;
        }
        case deColorSpaceHSV:
        case deColorSpaceHSL:
        {
            if (channel == 2)
            {
                return true;
            }
            break;
        }
        default:
        {
            return true;
            break;
        }
    }

    return false;
}

bool isChannelWrapped(deColorSpace colorSpace, int channel)
{
    switch (colorSpace)
    {
        case deColorSpaceLCH:
        {
            if (channel == 2)
            {
                return true;
            }
            break;
        }
        case deColorSpaceHSV:
        case deColorSpaceHSL:
        {
            if (channel == 0)
            {
                return true;
            }
            break;
        }
        default:
            break;
    }

    return false;
}

bool isActionSupported(const std::string& action, deColorSpace colorSpace)
{
    if ((action == "equalizer8") || (action == "equalizer16"))
    {
        switch (colorSpace)
        {
            case deColorSpaceRGB:
            case deColorSpaceCMY:
            case deColorSpaceProPhoto:
                return false;
            default:
                return true;
        }                
    }
    return true;
}

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

wxColour getChannelwxColour(deColorSpace colorSpace, int channel)
{
    switch (colorSpace)
    {
        case deColorSpaceRGB:
        case deColorSpaceProPhoto:
        {
            int g = 200;
            switch (channel)
            {
                case DE_CHANNEL_RED:
                    return wxColour(g, 0, 0);
                case DE_CHANNEL_GREEN:
                    return wxColour(0, g, 0);
                case DE_CHANNEL_BLUE:
                    return wxColour(0, 0, g);
            }                    
        }            
        case deColorSpaceCMYK:
        case deColorSpaceCMY:
        {
            int g = 240;
            int g2 = 50;
            switch (channel)
            {
                case DE_CHANNEL_CYAN:
                    return wxColour(0, g, g);
                case DE_CHANNEL_MAGENTA:
                    return wxColour(g, 0, g);
                case DE_CHANNEL_YELLOW:
                    return wxColour(g, g, 0);
                case DE_CHANNEL_KEY:
                    return wxColour(g2, g2, g2);
            }                    
        }            
        default:
        {
            // in any other cases just use dark gray
            int g = 50;
            return wxColour(g, g, g);
        }            
    }

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

void getBasicSettings(deColorSpace colorSpace, std::vector<deBasicSetting>& settings1, std::vector<deBasicSetting>& settings2)
{
    switch (colorSpace)
    {
        case deColorSpaceLAB:
        {
            deBasicSetting b("brightness");
            b.setBrightness();
            b.addChannel(0);
            settings1.push_back(b);

            deBasicSetting c("contrast");
            c.setContrast();
            c.addChannel(0);
            settings1.push_back(c);

            deBasicSetting s("saturation");
            s.setContrast();
            s.addChannel(1);
            s.addChannel(2);
            settings1.push_back(s);

            deBasicSetting gms("A tint");
            gms.setBrightness();
            gms.addChannel(1);
            settings2.push_back(gms);

            deBasicSetting gmc("A contrast");
            gmc.setContrast();
            gmc.addChannel(1);
            settings2.push_back(gmc);

            deBasicSetting bys("B tint");
            bys.setBrightness();
            bys.addChannel(2);
            settings2.push_back(bys);

            deBasicSetting byc("B contrast");
            byc.setContrast();
            byc.addChannel(2);
            settings2.push_back(byc);

            break;
        }

        case deColorSpaceLCH:
        {
            deBasicSetting b("brightness");
            b.setBrightness();
            b.addChannel(0);
            settings1.push_back(b);

            deBasicSetting c("contrast");
            c.setContrast();
            c.addChannel(0);
            settings1.push_back(c);

            deBasicSetting st("saturation");
            st.setBrightness();
            st.addChannel(1);
            settings1.push_back(st);

            deBasicSetting s("C contrast");
            s.setContrast();
            s.addChannel(1);
            settings2.push_back(s);

            deBasicSetting h("hue shift");
            h.setShift();
            h.addChannel(2);
            settings2.push_back(h);

            break;
        }

        case deColorSpaceHSV:
        case deColorSpaceHSL:
        {
            deBasicSetting b("brightness");
            b.setBrightness();
            b.addChannel(2);
            settings1.push_back(b);

            deBasicSetting c("contrast");
            c.setContrast();
            c.addChannel(2);
            settings1.push_back(c);

            deBasicSetting st("saturation");
            st.setBrightness();
            st.addChannel(1);
            settings1.push_back(st);

            deBasicSetting s("S contrast");
            s.setContrast();
            s.addChannel(1);
            settings2.push_back(s);

            deBasicSetting h("hue shift");
            h.setShift();
            h.addChannel(0);
            settings2.push_back(h);

            break;
        }

        case deColorSpaceBW:
        {
            deBasicSetting b("brightness");
            b.setBrightness();
            b.addChannel(0);
            settings1.push_back(b);

            deBasicSetting c("contrast");
            c.setContrast();
            c.addChannel(0);
            settings1.push_back(c);

            break;
        }            

        case deColorSpaceRGB:
        case deColorSpaceProPhoto:
        {
            deBasicSetting b("brightness");
            b.setBrightness();
            b.addChannel(0);
            b.addChannel(1);
            b.addChannel(2);
            settings1.push_back(b);

            deBasicSetting c("contrast");
            c.setContrast();
            c.addChannel(0);
            c.addChannel(1);
            c.addChannel(2);
            settings1.push_back(c);

            deBasicSetting b0("Red tint");
            b0.setBrightness();
            b0.addChannel(0);
            settings2.push_back(b0);

            deBasicSetting c0("Red contrast");
            c0.setContrast();
            c0.addChannel(0);
            settings2.push_back(c0);

            deBasicSetting b1("Green tint");
            b1.setBrightness();
            b1.addChannel(1);
            settings2.push_back(b1);

            deBasicSetting c1("Green contrast");
            c1.setContrast();
            c1.addChannel(1);
            settings2.push_back(c1);

            deBasicSetting b2("Blue tint");
            b2.setBrightness();
            b2.addChannel(2);
            settings2.push_back(b2);

            deBasicSetting c2("Blue contrast");
            c2.setContrast();
            c2.addChannel(2);
            settings2.push_back(c2);

            break;
        }

        case deColorSpaceCMYK:
        case deColorSpaceCMY:
        {
            bool cmyk = false;
            if (colorSpace == deColorSpaceCMYK)
            {
                cmyk = true;
            }

            deBasicSetting b("brightness");
            b.setBrightness();
            b.addChannel(0);
            b.addChannel(1);
            b.addChannel(2);
            if (cmyk)
            {
                b.addChannel(3);
            }
            settings1.push_back(b);

            deBasicSetting c("contrast");
            c.setContrast();
            c.addChannel(0);
            c.addChannel(1);
            c.addChannel(2);
            if (cmyk)
            {
                c.addChannel(3);
            }
            settings1.push_back(c);

            deBasicSetting b0("Cyan tint");
            b0.setBrightness();
            b0.addChannel(0);
            settings2.push_back(b0);

            deBasicSetting c0("Cyan contrast");
            c0.setContrast();
            c0.addChannel(0);
            settings2.push_back(c0);

            deBasicSetting b1("Magenta tint");
            b1.setBrightness();
            b1.addChannel(1);
            settings2.push_back(b1);

            deBasicSetting c1("Magenta contrast");
            c1.setContrast();
            c1.addChannel(1);
            settings2.push_back(c1);

            deBasicSetting b2("Yellow tint");
            b2.setBrightness();
            b2.addChannel(2);
            settings2.push_back(b2);

            deBasicSetting c2("Yellow contrast");
            c2.setContrast();
            c2.addChannel(2);
            settings2.push_back(c2);

            if (cmyk)
            {
                deBasicSetting b3("Key tint");
                b3.setBrightness();
                b3.addChannel(3);
                settings2.push_back(b3);

                deBasicSetting c3("Key contrast");
                c3.setContrast();
                c3.addChannel(3);
                settings2.push_back(c3);
            }

            break;
        }

        default:
            break;


    };
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

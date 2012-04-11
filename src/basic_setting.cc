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

#include "basic_setting.h"

deBasicSetting::deBasicSetting(const std::string& _name)
:name(_name)
{
    contrast = false;
    brightness = false;
    shift = false;
    scale = 1.0;
}

deBasicSetting::~deBasicSetting()
{
}


void deBasicSetting::setScale(deValue _scale)
{
    scale = _scale;
}

deValue deBasicSetting::getScale() const
{
    return scale;
}

void deBasicSetting::addChannel(int c)
{
    channels.insert(c);
}

void deBasicSetting::setContrast()
{
    contrast = true;
}

void deBasicSetting::setBrightness()
{
    brightness = true;
}

void deBasicSetting::setShift()
{
    shift = true;
}

bool deBasicSetting::affects(int c) const
{
    if (channels.count(c) == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int deBasicSetting::getChannel() const
{
    if (channels.size() != 1)
    {
        return -1;
    }
    std::set<int>::const_iterator i = channels.begin();
    return *i;
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
            gms.setScale(0.5);
            settings2.push_back(gms);

            deBasicSetting gmc("A contrast");
            gmc.setContrast();
            gmc.addChannel(1);
            settings2.push_back(gmc);

            deBasicSetting bys("B tint");
            bys.setBrightness();
            bys.addChannel(2);
            bys.setScale(0.5);
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
            st.setScale(0.5);
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
            b.setScale(-1);
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


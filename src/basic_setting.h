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

#ifndef _DE_BASIC_SETTING_H
#define _DE_BASIC_SETTING_H

#include <set>
#include <string>

class deBasicSetting
{
    private:
        std::string name;
        std::set<int> channels;
        bool contrast;
        bool brightness;
        bool shift;
    public:    
        deBasicSetting(const std::string& _name);
        virtual ~deBasicSetting();

        void setContrast();
        void setBrightness();
        void setShift();

        void addChannel(int c);

        std::string getName() const {return name;};
        bool affects(int c) const;

        bool isContrast() const {return contrast;};
        bool isBrightness() const {return brightness;};
        bool isShift() const {return shift;};

        int getChannel() const;
};

#endif

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
}

deBasicSetting::~deBasicSetting()
{
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

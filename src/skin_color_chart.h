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

#ifndef _DE_SKIN_COLOR_CHART_H
#define _DE_SKIN_COLOR_CHART_H

#include "value.h"

#include <vector>
#include <string>

struct deSkinCMYK
{
    deValue c;
    deValue m;
    deValue y;
    deValue k;
};

struct deSkinRGB
{
    int r;
    int g;
    int b;

    deSkinRGB(int _r, int _g, int _b)
    :r(_r), g(_g), b(_b)
    {
    }

};

struct deSkinRange
{
    int first;
    int last;
    std::string description;
};

void getFelixVonLuschan(std::vector<deSkinRGB>& skins);
void getSkinRanges(std::vector<deSkinRange>& ranges);

#endif

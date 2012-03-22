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

#include "palette.h"

dePalette3::dePalette3(deColorSpace _colorSpace)
:colorSpace(_colorSpace)
{
}

dePalette3::~dePalette3()
{
}

void dePalette3::addColor(const deColor3& color)
{
    colors.push_back(color);
}

void dePalette3::optimize(const dePalette3& source, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        deColor3 c = source.colors[i];
        colors.push_back(c);

    }
}

deColor3 dePalette3::getColor(int index)
{
    return colors[index];
}

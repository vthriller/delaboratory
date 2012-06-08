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

#include "radial_lut.h"
#include <cstdlib>
#include "value.h"
#include <cmath>

deRadialLUT::deRadialLUT(int _size, int r)
:size(_size)
{
    lut = new int [2 * size];

    srand(0);

    int i;

    for (i = 0; i < size; i++)
    {
        deValue angle = (rand () % 1024) / 1024.0 * 2 * M_PI;
        deValue radius_1 = ((rand () % 1024) / 1024.0 * r + 1.0);
        deValue radius_2 = ((rand () % 1024) / 1024.0 * radius_1 + 1.0);
        int xx = sin(angle) * radius_2;
        int yy = cos(angle) * radius_2;
        lut[2*i+0] = xx;
        lut[2*i+1] = yy;
    }
}

deRadialLUT::~deRadialLUT()
{
    delete [] lut;
}

void deRadialLUT::get(int i, int& x, int& y) const
{
    if ((i >= 0) && (i < size))
    {
        x = lut[2*i+0];
        y = lut[2*i+1];
    }
}

int deRadialLUT::getStart(int s) const
{
    return rand() % (size - s);
}

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

#include "color_matrix.h"
#include <iostream>

deColorMatrix::deColorMatrix(int _w, int _h)
{
    width = _w;
    height = _h;

    int n = width * height;
    matrix = new deValue [n];
    density = new int [n];
}

deColorMatrix::~deColorMatrix()
{
    delete [] matrix;
    delete [] density;
}

void deColorMatrix::clear()
{
    int n = width * height;
    int i;
    for (i = 0; i < n; i++)
    {
        matrix[i] = -1;
        density[i] = 0;
    }
}

void deColorMatrix::build(const deValue* channelH, const deValue* channelV, const deValue* channelA, int n)
{
    int i;

    int maxY = 0;
    for (i = 0; i < n; i++)
    {
        deValue h = channelH[i];
        deValue v = channelV[i];
        deValue a = channelA[i];

        if ((h >= 0) && (h < 1) && (v >= 0) && (v < 1) && ((a >= 0) && (a < 1)))
        {
            int x = h * width;
            int y = v * height;

            if (y > maxY)
            {
                maxY = y;
            }

            int pos = width * y + x;
        
            int d = density[pos];

            if (d == 0)
            {
                density[pos] = 1;
                matrix[pos] = a;
            }
            else
            {
                density[pos]++;
                matrix[pos] += a;
            }
        }            
    }

    int nn = width * height;
    for (i = 0; i < nn; i++)
    {
        int d = density[i];
        if (d > 0)
        {
            matrix[i] /= d;
        }
    }
}

deValue deColorMatrix::get(int x, int y, int min) const
{
    if (x < 0)
    {
        return -1;
    }
    if (y < 0)
    {
        return -1;
    }
    if (x >= width)
    {
        return -1;
    }
    if (y >= height)
    {
        return -1;
    }
    int pos = width * y + x;
    if (density[pos] < min)
    {
        return -1;
    }
    return matrix[pos];
}

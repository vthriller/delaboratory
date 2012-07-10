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
#include <cmath>

deColorMatrix::deColorMatrix(int _w, int _h)
{
    width = _w;
    height = _h;

    minX = 0;
    minY = 0;
    maxX = 1;
    maxY = 1;

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

void deColorMatrix::buildZoomed(const deValue* channelH, const deValue* channelV, const deValue* channelA, int n, int min)
{
    minX = 0;
    minY = 0;
    maxX = 1;
    maxY = 1;

    clear();
    build(channelH, channelV, channelA, n);
    int xx;
    int yy;
    int minxx = width;
    int maxxx = -1;
    int minyy = height;
    int maxyy = -1;

    for (xx = 0; xx < width; xx++)
    {
        for (yy = 0; yy < height; yy++)
        {
            int pos = width * yy + xx;
            if (density[pos] >= min)
            {
                if (xx < minxx)
                {
                    minxx = xx;
                }
                if (yy < minyy)
                {
                    minyy = yy;
                }
                if (xx > maxxx)
                {
                    maxxx = xx;
                }
                if (yy > maxyy)
                {
                    maxyy = yy;
                }
            }
        }
    }

    if (minxx > 0)
    {
        minxx--;
    }
    if (minyy > 0)
    {
        minyy--;
    }
    if (maxxx < width - 1)
    {
        maxxx++;
    }
    if (maxyy < height - 1)
    {
        maxyy++;
    }

    minX = (deValue) minxx / width;
    minY = (deValue) minyy / height;
    maxX = (deValue) maxxx / width;
    maxY = (deValue) maxyy / height;

    deValue margin = -0.01;

    if (maxX < 0.5 - margin)
    {
        maxX = 0.5 - margin;
    }
    if (maxY < 0.5 - margin)
    {
        maxY = 0.5 - margin;
    }
    if (minX > 0.5 + margin)
    {
        minX = 0.5 + margin;
    }
    if (minY > 0.5 + margin)
    {
        minY = 0.5 + margin;
    }

    clear();
    build(channelH, channelV, channelA, n);

}

void deColorMatrix::build(const deValue* channelH, const deValue* channelV, const deValue* channelA, int n)
{
    if (!channelH)
    {
        return;
    }
    if (!channelV)
    {
        return;
    }
    if (!channelA)
    {
        return;
    }

    int i;

    for (i = 0; i < n; i++)
    {
        deValue h = channelH[i];
        deValue v = channelV[i];
        deValue a = channelA[i];

        if ((h >= 0) && (h < 1) && (v >= 0) && (v < 1) && ((a >= 0) && (a < 1)))
        {
            int x = (h - minX) / (maxX - minX) * width;
            int y = (v - minY) / (maxY - minY) * height;

            if ((x >= 0) && (y >= 0) && (x < width) && (y < height))
            {
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

deValue deColorMatrix::get(int x, int y, int min, deValue& vx, deValue& vy, bool& center) const
{
    vx = minX + ((deValue) x / width) * (maxX - minX);
    deValue vx2 = minX + ((deValue) (x+1) / width) * (maxX - minX);
    deValue vx3 = minX + ((deValue) (x-1) / width) * (maxX - minX);
    deValue dx = fabs(0.5 - vx);
    deValue dx2 = fabs(0.5 - vx2);
    deValue dx3 = fabs(0.5 - vx3);

    vy = minY + ((deValue) y / height) * (maxY - minY);
    deValue vy2 = minY + ((deValue) (y+1) / height) * (maxY - minY);
    deValue vy3 = minY + ((deValue) (y-1) / height) * (maxY - minY);
    deValue dy = fabs(0.5 - vy);
    deValue dy2 = fabs(0.5 - vy2);
    deValue dy3 = fabs(0.5 - vy3);

    center = false;
    if ((dx < dx2) && (dx < dx3))
    {
        center = true;
    }
    if ((dy < dy2) && (dy < dy3))
    {
        center = true;
    }

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

    deValue result = matrix[pos];
    return result;
}

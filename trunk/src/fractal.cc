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

#include "fractal.h"
#include <cmath>
#include <cassert>
#include "blur.h"
#include "copy_channel.h"

void generateFractalColor(deValue v, deValue& r, deValue& g, deValue& b)
{
    deValue v1 = sin(v * M_PI);
    assert(v1 <= 1.0);
    assert(v1 >= 0.0);
    r = v1;
    g = v1;
    b = v1;
}

void generateMandelbrot(deValue* r, deValue* g, deValue* b, deSize size, deValue x1, deValue y1, deValue x2, deValue y2)
{
    deValue w = size.getW();
    deValue h = size.getH();

    int p = 0;

    deValue maxI = 400.0;

    deValue marg = 4.0;

    deValue zoomX = x2 - x1;
    deValue zoomY = y2 - y1;
    deValue offX = x1;
    deValue offY = y1;

    deValue y;
    for (y = 0; y < h; y++)
    {
        deValue x;
        deValue cy = (y / h) * zoomY + offY;
        for (x = 0; x < w; x++)
        {
            deValue cx = (x / w) * zoomX + offX;

            deValue zx = 0;
            deValue zy = 0;

            deValue zx2 = 0;
            deValue zy2 = 0;

            int i;
            for (i = 0; i < maxI && (zx2 + zy2 < marg); i++)
            {
                zy = 2 * zx * zy + cy;
                zx = zx2 - zy2 + cx;
                zx2 = zx * zx;
                zy2 = zy * zy;
            }

            deValue v = i / maxI;
            deValue v1;
            deValue v2;
            deValue v3;
            generateFractalColor(v, v1, v2, v3);
            r[p] = v1;
            g[p] = v2;
            b[p] = v3;

            p++;
        }
    }
}

void generateFractal(deValue* r, deValue* g, deValue* b, deSize size)
{
    generateMandelbrot(r, g, b, size, -0.42, -0.69, -0.32, -0.59);

    deValue r1 = 20;
    deValue r2 = 50;

    deValue* tmp = new deValue[size.getN()];

    blurChannel(r, tmp, size, r1, r1, deGaussianBlur, 0);
    copyChannel(tmp, r, size.getN());

    blurChannel(b, tmp, size, r2, r2, deGaussianBlur, 0);
    copyChannel(tmp, b, size.getN());

    delete [] tmp;
}

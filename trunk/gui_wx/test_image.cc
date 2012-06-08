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

#include "test_image.h"
#include "static_image.h"
#include <cmath>
#include <iostream>

void generateImage1(deValue* pixels0, deValue* pixels1, deValue* pixels2, int w, int h)
{
    int x;
    int y;
    int p = 0;

    deValue margin = 0.1;

    deValue L = 0.8;
    deValue A;
    deValue B;

    for (y = 0; y < h; y++)
    {
        deValue yy = (deValue) y / h;
        if ((yy > margin) && (yy < 1 - margin))
        {
            B = (yy - margin) / (1.0 - 2 * margin);
        }
        else
        {
            B = 0;
        }

        for (x = 0; x < w; x++)
        {
            deValue xx = (deValue) x / w;
            if ((xx > margin) && (xx < 1 - margin))
            {
                A = (xx - margin) / (1.0 - 2 * margin);
            }
            else
            {
                A = 0;
            }

            if ((A == 0) || (B == 0))
            {
                deValue m = yy;
                if (xx > m)
                {
                    m = xx;
                }
                pixels0[p] = m;
                pixels1[p] = 0.5;
                pixels2[p] = 0.5;
            }
            else
            {
                pixels0[p] = L;
                pixels1[p] = A;
                pixels2[p] = B;
            }

            p++;
        }
    }        
}

void generateTestImage(deStaticImage& image, int s)
{
    int w = s;
    int h = s;
    deColorSpace colorSpace = deColorSpaceLAB;

    image.lock();

    deSize size(w, h);
    image.setSize(size);

    image.setColorSpace(colorSpace);

    deValue* pixels0 = image.startWriteStatic(0);
    deValue* pixels1 = image.startWriteStatic(1);
    deValue* pixels2 = image.startWriteStatic(2);

    generateImage1(pixels0, pixels1, pixels2, w, h);

    image.finishWriteStatic(0);
    image.finishWriteStatic(1);
    image.finishWriteStatic(2);

    image.unlock();
}


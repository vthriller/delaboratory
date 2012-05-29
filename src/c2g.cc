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

#include "c2g.h"
#include <cmath>
#include <cstdlib>
#include <iostream>

void c2g(const deValue* source0, const deValue* source1, const deValue* source2, deValue* mask, const deSize& size, deValue r, int samples)
{
    int w = size.getW();
    int h = size.getH();

    srand(0);

    int x;
    for (x = 0; x < w; x++)
    {
        int xx1 = x - r;
        int xx2 = x + r;
        if (xx1 < 0)
        {
            xx1 = 0;
        }
        if (xx2 >= w)
        {
            xx2 = w -1;
        }
        int y;
        for (y = 0; y < h; y++)
        {
            int yy1 = y - r;
            int yy2 = y + r;
            if (yy1 < 0)
            {
                yy1 = 0;
            }
            if (yy2 >= h)
            {
                yy2 = h -1;
            }

            int p = w * y + x;
            deValue v0 = source0[p];
            deValue v1 = source1[p];
            deValue v2 = source2[p];

            deValue min0 = 1.0;
            deValue max0 = 0.0;
            deValue min1 = 1.0;
            deValue max1 = 0.0;
            deValue min2 = 1.0;
            deValue max2 = 0.0;

            int dxx = xx2 - xx1 + 1;
            int dyy = yy2 - yy1 + 1;

            int i;
            for (i = 0; i < samples; i++)
            {
                int xx = xx1 + (rand() % dxx);
                int yy = yy1 + (rand() % dyy);

                int pp = w * yy + xx;

                deValue vv0 = source0[pp];
                deValue vv1 = source1[pp];
                deValue vv2 = source2[pp];

                if (vv0 < min0)
                {
                    min0 = vv0;
                }
                if (vv0 > max0)
                {
                    max0 = vv0;
                }

                if (vv1 < min1)
                {
                    min1 = vv1;
                }
                if (vv1 > max1)
                {
                    max1 = vv1;
                }

                if (vv2 < min2)
                {
                    min2 = vv2;
                }
                if (vv2 > max2)
                {
                    max2 = vv2;
                }
                
            }

            deValue n0 = v0 - min0;
            deValue d0 = v0 - max0;
            deValue n1 = v1 - min1;
            deValue d1 = v1 - max1;
            deValue n2 = v2 - min2;
            deValue d2 = v2 - max2;

            deValue n = sqrt(n0 * n0 + n1 * n1 + n2 * n2);
            deValue d = sqrt(d0 * d0 + d1 * d1 + d2 * d2);

            d = d + n;

            deValue result = (v0 + v1 + v2) / 3.0;

            if (d != 0)
            {
                deValue a = n / d;

                result = a;

                if (result < 0)
                {
                    result = 0;
                }
                else if (result > 1)
                {
                    result = 1;
                }
            }

            mask[p] = result;
        }
    }

}


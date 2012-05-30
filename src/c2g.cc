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
#include <iostream>

#include "radial_lut.h"

void c2g(const deValue* source0, const deValue* source1, const deValue* source2, deValue* mask, const deSize& size, deValue r, int samples)
{
    int w = size.getW();
    int h = size.getH();

    int lutsize = 10 * 1024;
    deRadialLUT lut(lutsize, r);

    int x;
    for (x = 0; x < w; x++)
    {
        int y;
        for (y = 0; y < h; y++)
        {
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

            deValue sum = 0.0;
            int nn = 0;

            int rr = r;
            int offset = lut.getStart(samples);

            int i = 0;
            while ((i < samples) && (offset < lutsize))
            {
                int xx;
                int yy;
                lut.get(offset, xx, yy);
                xx += x;
                yy += y;

                if ((xx >= 0) && (xx < w) && (yy >= 0) && (yy < h))
                {
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

                    i++;
                }

                offset++;
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

            deValue result;

            if (d != 0)
            {
                result = n / d;

                if (result < 0)
                {
                    result = 0;
                }
                else if (result > 1)
                {
                    result = 1;
                }
            }
            else
            {
                result = (v0 + v1 + v2) / 3.0;
            }

            mask[p] = result;
        }
    }

}


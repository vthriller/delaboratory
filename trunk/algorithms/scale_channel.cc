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

#include "scale_channel.h"
#include "sample_pixel.h"

void scaleChannel(const deValue* src, deValue* dst, deValue z_x1, deValue z_y1, deValue z_x2, deValue z_y2, int w, int h, bool mirrorX, bool mirrorY, int rotate, int ws, int hs, deValue contrast)
{
    int x1;
    int y1;
    int x2;
    int y2;
    x1 = ws * z_x1;
    y1 = hs * z_y1;
    x2 = ws * z_x2;
    y2 = hs * z_y2;
    if ((rotate == 90) || (rotate == 270))
    {
        x1 = hs * z_x1;
        y1 = ws * z_y1;
        x2 = hs * z_x2;
        y2 = ws * z_y2;
    }

    deValue scaleW;
    deValue scaleH;

    deValue dx = x2 - x1;
    deValue dy = y2 - y1;

    scaleW = dx / w;
    scaleH = dy / h;

    if (scaleW <= 0)
    {
    }
    if (scaleH <= 0)
    {
    }

    int yy1;
    int yy2;
    int xx1;
    int xx2;

    int x;
    for (x = 0; x < w; x++)
    {
        xx1 = scaleW * x;
        xx2 = scaleW * (x + 1);

        int y;
        for (y = 0; y < h; y++)
        {
            yy1 = scaleH * y;
            yy2 = scaleH * (y + 1);

            deValue v = 1;

            if (rotate == 0)
            {
                v = samplePixel(src, x1 + xx1, x1 + xx2, y1 + yy1, y1 + yy2, mirrorX, mirrorY, ws, hs);
            }   
            if (rotate == 90)
            {
                v = samplePixel(src, ws - 1 - yy2 - y1, ws - 1 - yy1 - y1, xx1 + x1, xx2 + x1,  mirrorX, mirrorY, ws, hs);
            }   
            if (rotate == 180)
            {
                v = samplePixel(src, ws - 1 - xx2 - x1, ws - 1 - xx1 - x1, hs - 1 - yy2 - y1, hs - 1 - yy1 - y1, mirrorX, mirrorY, ws, hs);
            }   
            if (rotate == 270)
            {
                v = samplePixel(src, y1 + yy1, y1 + yy2, hs - 1 - xx2 - x1, hs - 1 - xx1 - x1, mirrorX, mirrorY, ws, hs);
            }   

            dst[y * w + x] = contrast * v + 0.5 - 0.5 * contrast;

        }

    }        

}


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
#include "channel.h"
#include <iostream>
#include <cassert>

/*
void scaleChannel(const deChannel& s, deChannel& d, const deSize& ss, const deSize& ds)
{
    int ws = ss.getW();
    int hs = ss.getH();
    int wd = ds.getW();
    int hd = ds.getH();
    float scaleW = (float) ws / wd;
    float scaleH = (float) hs / hd;

    const deValue* src = s.getPixels();
    deValue* dst = d.getPixels();

    int x;
    int y;
    for (x = 0; x < wd; x++)
    {
        for (y = 0; y < hd; y++)
        {
            int xx = scaleW * x;
            int yy = scaleH * y;
            deValue v = src[yy*ws+xx];
            dst[y*wd+x] = v;
        }
    }
}

void scaleChannel(const deChannel& s, deChannel& d, const deSize& ss, const deSize& ds, deValue scale, deValue offsetX, deValue offsetY)
{

    deValue centerX = (offsetX + 1.0) / 2.0;
    deValue centerY = (offsetY + 1.0) / 2.0;

    int ws = ss.getW();
    int hs = ss.getH();
    deValue sAspect = ss.getAspect();
    deValue dAspect = ds.getAspect();

    centerX *= ws;
    centerY *= hs;

    int wd = ds.getW();
    int hd = ds.getH();

    float scaleW = (float) ws / wd;
    float scaleH = (float) hs / hd;

    scaleH *= (sAspect / dAspect);

    scaleW /= scale;
    scaleH /= scale;

    int ox = centerX - scaleW * (wd / 2);
    int oy = centerY - scaleH * (hd / 2);

    int maxox = ws - scaleW * ( wd - 1);
    int maxoy = hs - scaleH * ( hd - 1);

    if (ox > maxox)
    {
        ox = maxox;
    }
    if (oy > maxoy)
    {
        oy = maxoy;
    }
    if (ox < 0)
    {
        ox = 0;
    }
    if (oy < 0)
    {
        oy = 0;
    }

    int maxy = hd;
    int maxyy = (hs - oy) / scaleH;
    if (maxyy < maxy)
    {
        maxy = maxyy;
    }

    int maxx = wd;
    int maxxx = (ws - ox) / scaleW;
    if (maxxx < maxx)
    {
        maxx = maxxx;
    }

    const deValue* src = s.getPixels();
    deValue* dst = d.getPixels();

    int x;
    int y;
    for (x = 0; x < maxx; x++)
    {
        int xx = ox + scaleW * x;
        assert(xx < ws);
        assert(xx >= 0);
        for (y = 0; y < maxy; y++)
        {
            int yy = oy + scaleH * y;
            assert(yy < hs);
            assert(yy >= 0);
            deValue v = src[yy*ws+xx];
            dst[y*wd+x] = v;
        }
    }
}
*/

void scaleChannel(deValue* src, deValue* dst, int maxx, int ox, deValue scaleW, int maxy, int oy, deValue scaleH, int ws, int wd)
{
    int x;
    int y;
    for (x = 0; x < maxx; x++)
    {
        int xx = ox + scaleW * x;
        for (y = 0; y < maxy; y++)
        {
            int yy = oy + scaleH * y;
            deValue v = src[yy*ws+xx];
            dst[y*wd+x] = v;
        }
    }
}


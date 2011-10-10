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


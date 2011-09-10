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

#include "channel.h"
#include <iostream>

deTrueChannel::deTrueChannel(const deSize& _size, deValue _min, deValue _max)
:deBaseChannel(_size), min(_min), max(_max)
{
    pixels = new deValue [size.getN()];
}

deTrueChannel::~deTrueChannel()
{
    delete [] pixels;
}

deValue deTrueChannel::getValue(int pos) const
{
    return pixels[pos];
}

void deTrueChannel::setValue(int pos, deValue value)
{
    if (value < min)
    {
        value = min;
    }
    else if (value > max)
    {
        value = max;
    }
    pixels[pos] = value;
}

bool deTrueChannel::copy(const deBaseChannel* channel)
{
    if (!channel)        
    {
        return false;
    }
    if (size != channel->getSize())
    {
        return false;
    }

    const deTrueChannel* c = dynamic_cast<const deTrueChannel*>(channel);
    if (!c)
    {
        return false;
    }

    int i;
    int n = size.getN();
    for (i = 0; i < n; i++)
    {
        pixels[i] = c->pixels[i];
    }
    return true;
}

bool deTrueChannel::scale(const deTrueChannel* channel)
{
    if (!channel)        
    {
        return false;
    }

    int ws = channel->size.getW();
    int hs = channel->size.getH();
    int wd = size.getW();
    int hd = size.getH();
    float scaleW = (float) ws / wd;
    float scaleH = (float) hs / hd;

    int x;
    int y;
    for (x = 0; x < wd; x++)
    {
        for (y = 0; y < hd; y++)
        {
            int xx = scaleW * x;
            int yy = scaleH * y;
            pixels[y*wd+x] = channel->pixels[yy*ws+xx];
        }
    }
    return true;
}

const deValue* deTrueChannel::getPixels() const
{
    return pixels;
}

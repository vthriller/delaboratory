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

#include "sampler.h"
#include "layer.h"
#include "preview.h"
#include "color_space.h"
#include "channels.h"
#include "channel.h"
#include <cassert>
#include "convert_color.h"

#include "converter.h"
#include <iostream>

deSampler::deSampler()
{
    x = -1;
    y = -1;
    disable();
}

deSampler::deSampler(const deSampler& sampler)
{
    x = sampler.x;
    y = sampler.y;
    enabled = sampler.enabled;
}

deSampler::~deSampler()
{
}

void deSampler::setPosition(float _x, float _y)
{
    x = _x;
    y = _y;
}

float deSampler::getX() const
{
    return x;
}

float deSampler::getY() const
{
    return y;
}

bool deSampler::getPixel(float &v0, float &v1, float &v2, float &v3, deColorSpace mode, dePreview& preview) const
{
    if (!enabled)
    {
        return false;
    }
    assert(x >= 0);
    assert(y >= 0);
    assert(x < 1);
    assert(y < 1);

    const deSize& s = preview.getRawSize();

    int w = s.getW();
    int h = s.getH();

    int xxx = x * w;
    int yyy = y * h;

    int pos = yyy * w + xxx;

    deConverter converter;
    converter.setSource(&preview);

    bool result = converter.getPixel(pos, mode, v0, v1, v2, v3);
    return result;
}

bool deSampler::isEnabled() const
{
    return enabled;
}

void deSampler::enable()
{
    enabled = true;
}

void deSampler::disable()
{
    enabled = false;
}

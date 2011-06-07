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

#include "blur.h"
#include "preview.h"
#include "channel.h"


void blurVertical(const deChannel& source, deChannel& destination, int col, int blurSize)
{
    const deSize& size = source.getSize();
    int w = size.getW();
    int h = size.getH();

    int i;
    for (i = 0; i < h; i++)
    {
        deValue result = 0.0;
        deValue sum = 0.0;

        int n1 = i - blurSize;
        int n2 = i + blurSize;
        if (n1 < 0)
        {
            n1 = 0;
        }

        if (n2 >= h)
        {
            n2 = h - 1;
        }
        
        int j;
        for (j = n1; j <= n2; j++)
        {
            int pos2 = j * w + col;
            deValue v = source.getValue(pos2);
            deValue weight = 1.0;
            result += weight * v;
            sum += weight;
        }
        result /= sum;
        int pos = i * w + col;
        destination.setValue(pos, result);
    }
}

void blurHorizontal(const deChannel& source, deChannel& destination, int row, int blurSize)
{
    const deSize& size = source.getSize();
    int w = size.getW();

    int p = row * w;

    int i;
    for (i = 0; i < w; i++)
    {
        deValue result = 0.0;
        deValue sum = 0.0;

        int n1 = i - blurSize;
        int n2 = i + blurSize;
        if (n1 < 0)
        {
            n1 = 0;
        }

        if (n2 >= w)
        {
            n2 = w - 1;
        }
        
        int j;
        for (j = n1; j <= n2; j++)
        {
            int pos2 = p + j;
            deValue v = source.getValue(pos2);
            deValue weight = 1.0;
            result += weight * v;
            sum += weight;
        }
        result /= sum;
        int pos = p + i;
        destination.setValue(pos, result);
    }
}

void blur(const deBaseChannel* source, deBaseChannel* destination, deBlurDirection direction, deValue radius)
{
    deChannel* d = dynamic_cast<deChannel*>(destination);
    if (!d)
    {
        return;
    }
    
    const deChannel* s = dynamic_cast<const deChannel*>(source);
    if (!s)
    {
        return;
    }

    const deSize& size = s->getSize();

    int w = size.getW();
    int h = size.getH();

    int i;

    if (direction == deBlurHorizontal)
    {
        int blurSize = w * radius;
        for (i = 0; i < h; i++)
        {
            blurHorizontal(*s, *d, i, blurSize);
        }
    }
    else
    {
        int blurSize = h * radius;
        for (i = 0; i < w; i++)
        {
            blurVertical(*s, *d, i, blurSize);
        }
    }        
}


void blur(const dePreview& sourcePreview, dePreview& destinationPreview, deBlurDirection direction, deValue radius, std::set<int>& enabledChannels)
{
    deColorSpace sc = sourcePreview.getColorSpace();
    deColorSpace dc = destinationPreview.getColorSpace();

    int sn = getColorSpaceSize(sc);
    int dn = getColorSpaceSize(dc);

    int n = sn;
    if (dn < n)
    {
        n = dn;
    }

    int i;
    for (i = 0; i < n; i++)
    {
        if (enabledChannels.count(i) == 1)
        {
            blur(sourcePreview.getChannel(i), destinationPreview.getChannel(i), direction, radius);
        }
        else
        {
            destinationPreview.getChannel(i)->copy(sourcePreview.getChannel(i));
        }
    }
}

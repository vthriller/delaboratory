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

void blurVertical(const deChannel& source, deChannel& destination, int col)
{
    const deSize& size = source.getSize();
    int w = size.getW();
    int h = size.getH();

    int i;
    for (i = 0; i < h; i++)
    {
        int pos = i * w + col;
        deValue v = source.getValue(pos);
        destination.setValue(pos, v);
    }
}

void blurHorizontal(const deChannel& source, deChannel& destination, int row)
{
    const deSize& size = source.getSize();
    int w = size.getW();

    int i;
    for (i = 0; i < w; i++)
    {
        int pos = row * w + i;
        deValue v = source.getValue(pos);
        destination.setValue(pos, v);
    }
}

void blur(const deBaseChannel* source, deBaseChannel* destination, deBlurDirection direction)
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
        for (i = 0; i < h; i++)
        {
            blurHorizontal(*s, *d, i);
        }
    }
    else
    {
        for (i = 0; i < w; i++)
        {
            blurVertical(*s, *d, i);
        }
    }        
}


void blur(const dePreview& sourcePreview, dePreview& destinationPreview, deBlurDirection direction)
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
        blur(sourcePreview.getChannel(i), destinationPreview.getChannel(i), direction);
    }
}

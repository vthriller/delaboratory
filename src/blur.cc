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

void blur(deValue* source, deValue* destination, int n, int s)
{
    int i;
    for (i = 0; i < n; i++)
    {
        deValue result = 0.0;

        int n1 = i - s;
        int n2 = i + s;
        if (n1 < 0)
        {
            n1 = 0;
        }

        if (n2 >= n)
        {
            n2 = n - 1;
        }

        int j;
        for (j = n1; j <= n2; j++)
        {
            result += source[j];
        }
        destination[i] = result / (n2 - n1 + 1.0);
    }
}

void blurFaster(const deBaseChannel* source, deBaseChannel* destination, deBlurDirection direction, deValue radius)
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

    int n;

    if (direction == deBlurHorizontal)
    {
        n = w;
    }
    else
    {
        n = h;
    }

    int blurSize = n * radius;

    deValue* sourceBuffer = new deValue[n];
    deValue* destinationBuffer = new deValue[n];

    int i;
    int j;
    const deValue* pixels = s->getPixels();

    if (direction == deBlurHorizontal)
    {
        for (i = 0; i < h; i++)
        {
            int p = i * w;
            for (j = 0; j < n; j++)
            {
                sourceBuffer[j] = pixels[p + j]; 
            }
            blur(sourceBuffer, destinationBuffer, n, blurSize);
            for (j = 0; j < n; j++)
            {
                d->setValue(p + j, destinationBuffer[j]);
            }
        }
    }
    else
    {
        for (i = 0; i < w; i++)
        {
            for (j = 0; j < n; j++)
            {
                sourceBuffer[j] = pixels[j * w + i]; 
            }
            blur(sourceBuffer, destinationBuffer, n, blurSize);
            for (j = 0; j < n; j++)
            {
                d->setValue(j * w + i, destinationBuffer[j]);
            }
        }
    }        

    delete [] destinationBuffer;
    delete [] sourceBuffer;

}

void blur(const dePreview& sourcePreview, dePreview& destinationPreview, deBlurDirection direction, deValue radius, const deChannels& enabledChannels)
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
            blurFaster(sourcePreview.getChannel(i), destinationPreview.getChannel(i), direction, radius);
        }
        else
        {
            destinationPreview.getChannel(i)->copy(sourcePreview.getChannel(i));
        }
    }

}

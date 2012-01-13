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
#include <cmath>
#include <cassert>

void boxBlur(deValue* source, deValue* destination, int n, int s)
{
    int i;
    for (i = 0; i < n; i++)
    {
        deValue result = 0.0;

        int n1 = i - s + 1;
        int n2 = i + s - 1;
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

void gaussianBlur(deValue* source, deValue* destination, int n, int s, deValue* weights)
{
    int i;
    for (i = 0; i < n; i++)
    {
        deValue result = 0.0;
        deValue sum = 0.0;
        
        int n1 = i - s + 1;
        int n2 = i + s - 1;
        if (n1 < 0)
        {
            n1 = 0;
        }

        if (n2 >= n)
        {
            n2 = n - 1;
        }

        int j;
        int p;

        j = n1;
        p = i - n1;
        while (p >= 0)
        {
            deValue v = source[j];
            deValue w = weights[p];
            result += w * v;
            sum += w;

            p--;
            j++;
        }
        p = 1;
        while (j <= n2)
        {

            deValue v = source[j];
            deValue w = weights[p];
            result += w * v;
            sum += w;

            p++;
            j++;
        }


        destination[i] = result / sum;
    }
}

void surfaceBlur(deValue* source, deValue* destination, int n, int s, deValue* weights, deValue t)
{
    deValue tt = 1.0 - t;

    int i;
    for (i = 0; i < n; i++)
    {
        deValue result = 0.0;
        deValue sum = 0.0;
        
        deValue reference = source[i];

        int n1 = i - s + 1;
        int n2 = i + s - 1;
        if (n1 < 0)
        {
            n1 = 0;
        }

        if (n2 >= n)
        {
            n2 = n - 1;
        }

        int j;
        int p;

        j = n1;
        p = i - n1;
        while (p >= 0)
        {
            deValue v = source[j];
            if (fabs(v - reference) <= tt)
            {
                deValue w = weights[p];
                result += w * v;
                sum += w;
            }                

            p--;
            j++;
        }
        p = 1;
        while (j <= n2)
        {

            deValue v = source[j];
            if (fabs(v - reference) <= tt)
            {
                deValue w = weights[p];
                result += w * v;
                sum += w;
            }                

            p++;
            j++;
        }


        destination[i] = result / sum;
    }
}

void fillWeightsFlat(deValue* weights, int blurSize)
{
    int i;
    for (i = 0 ; i < blurSize; i++)
    {
        weights[i] = 1.0;
    }
}    

void fillWeightsGaussian(deValue* weights, int blurSize)
{
    int i;
    deValue radius = blurSize / 3.0;
    deValue rr2 = 2.0 * radius * radius;
    for (i = 0 ; i < blurSize; i++)
    {
        deValue ii = i * i;
        deValue ee = exp( - ii / rr2 );
        weights[i] = 1.0 / sqrt(rr2 * M_PI) * ee;
    }
}    

void blurChannel(const deValue* source, deValue* destination, deSize size, deValue radius, deBlurType type, deValue t)
{
    assert(source);
    assert(destination);

    int w = size.getW();
    int h = size.getH();

    assert(w > 0);
    assert(h > 0);
    assert(radius > 0);

    int blurSizeW = radius;
    if (blurSizeW < 1)
    {
        blurSizeW = 1;
    }
    int blurSizeH = radius;
    if (blurSizeH < 1)
    {
        blurSizeH = 1;
    }

    int maxSize = blurSizeW;
    if (blurSizeH > maxSize)
    {
        maxSize = blurSizeH;
    }

    int max = w;
    if (h > max)
    {
        max = h;
    }

    deValue* tmp = new deValue[size.getN()];

    deValue* sourceBuffer = new deValue[max];
    deValue* destinationBuffer = new deValue[max];
    deValue* weights = new deValue[maxSize];

    int i;
    int j;


    {
        if (type != deBoxBlur)
        {
            fillWeightsGaussian(weights, blurSizeW);
        }

        for (i = 0; i < h; i++)
        {
            int p = i * w;
            for (j = 0; j < w; j++)
            {
                sourceBuffer[j] = source[p + j]; 
            }
            switch (type)
            {
                case deBoxBlur:
                {
                    boxBlur(sourceBuffer, destinationBuffer, w, blurSizeW);
                    break;
                }                    
                case deGaussianBlur:
                {
                    gaussianBlur(sourceBuffer, destinationBuffer, w, blurSizeW, weights);
                    break;
                }                    
                case deSurfaceBlur:
                {
                    surfaceBlur(sourceBuffer, destinationBuffer, w, blurSizeW, weights, t);
                    break;
                }                    
                default:
                    break;
            }
            for (j = 0; j < w; j++)
            {
                tmp[p + j] = destinationBuffer[j];
            }
        }
    }


    {
        if (type != deBoxBlur)
        {
            fillWeightsGaussian(weights, blurSizeH);
        }

        for (i = 0; i < w; i++)
        {
            for (j = 0; j < h; j++)
            {
                sourceBuffer[j] = tmp[j * w + i]; 
            }
            switch (type)
            {
                case deBoxBlur:
                {
                    boxBlur(sourceBuffer, destinationBuffer, h, blurSizeH);
                    break;
                }                    
                case deGaussianBlur:
                {
                    gaussianBlur(sourceBuffer, destinationBuffer, h, blurSizeH, weights);
                    break;
                }                        
                case deSurfaceBlur:
                {
                    surfaceBlur(sourceBuffer, destinationBuffer, h, blurSizeH, weights, t);
                    break;
                }                    
                default:
                    break;
            }
            for (j = 0; j < h; j++)
            {
                destination[j * w + i] = destinationBuffer[j];
            }
        }
    }        

    delete [] tmp;
    delete [] weights;
    delete [] destinationBuffer;
    delete [] sourceBuffer;

}


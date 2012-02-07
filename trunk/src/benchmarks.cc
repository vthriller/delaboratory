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

#include "benchmarks.h"
#include <wx/wx.h>
#include "conversion_functions.h"

int benchmarkBlur(deSize size, deValue radius, deBlurType type)
{
    deValue* channel1 = new deValue[size.getN()];
    deValue* channel2 = new deValue[size.getN()];

    int i;
    int n = size.getN();

    for (i = 0; i < n; i++)
    {
        channel1[i] = (deValue) rand() / RAND_MAX;
    }            

    int t;
    {
        wxStopWatch sw;

        blurChannel(channel1, channel2, size, radius, radius, type, 0.1);

        t = sw.Time();
    }

    delete [] channel1;
    delete [] channel2;

    return t;
}


deBenchmark::deBenchmark()
{
}

deBenchmark::~deBenchmark()
{
}

deBenchmarkBlur::deBenchmarkBlur(deSize _size, deValue _radius, deBlurType _type)
:size(_size), radius(_radius), type(_type)
{
}
 
deBenchmarkBlur::~deBenchmarkBlur()
{
}

int deBenchmarkBlur::perform()
{
    return benchmarkBlur(size, radius, type);
}

deBenchmarkColor::deBenchmarkColor(int _size, deColorSpace _src, deColorSpace _dst)
:size(_size), src(_src), dst(_dst)
{
}

deBenchmarkColor::~deBenchmarkColor()
{
}

int deBenchmarkColor::perform()
{
    deValue* s1 = new deValue[size];
    deValue* s2 = new deValue[size];
    deValue* s3 = new deValue[size];
    deValue* s4 = new deValue[size];

    deValue* d1 = new deValue[size];
    deValue* d2 = new deValue[size];
    deValue* d3 = new deValue[size];
    deValue* d4 = new deValue[size];

    int i;
    for (i = 0; i < size; i++)
    {
        s1[i] = (deValue) rand() / RAND_MAX;
        s2[i] = (deValue) rand() / RAND_MAX;
        s3[i] = (deValue) rand() / RAND_MAX;
        s4[i] = (deValue) rand() / RAND_MAX;
    }            

    int t = -0.666;

    {
        wxStopWatch sw;


        deConversion3x3 conversion3x3 = getConversion3x3(src, dst);
        if (conversion3x3)
        {
            for (i = 0; i < size; i++)
            {
                conversion3x3(s1[i], s2[i], s3[i], d1[i], d2[i], d3[i]);
            }            
        }

        deConversion3x1 conversion3x1 = getConversion3x1(src, dst);
        if (conversion3x1)
        {
            for (i = 0; i < size; i++)
            {
                conversion3x1(s1[i], s2[i], s3[i], d1[i]);
            }            
        }

        deConversion1x3 conversion1x3 = getConversion1x3(src, dst);
        if (conversion1x3)
        {
            for (i = 0; i < size; i++)
            {
                conversion1x3(s1[i], d1[i], d2[i], d3[i]);
            }            
        }

        deConversion3x4 conversion3x4 = getConversion3x4(src, dst);
        if (conversion3x4)
        {
            for (i = 0; i < size; i++)
            {
                conversion3x4(s1[i], s2[i], s3[i], d1[i], d2[i], d3[i], d4[i]);
            }            
        }

        deConversion4x3 conversion4x3 = getConversion4x3(src, dst);
        if (conversion4x3)
        {
            for (i = 0; i < size; i++)
            {
                conversion4x3(s1[i], s2[i], s3[i], s4[i], d1[i], d2[i], d3[i]);
            }            
        }

        t = sw.Time();
    }

    delete [] s1;
    delete [] s2;
    delete [] s3;
    delete [] s4;

    delete [] d1;
    delete [] d2;
    delete [] d3;
    delete [] d4;

    return t;
}

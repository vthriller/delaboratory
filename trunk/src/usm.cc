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

#include "usm.h"
#include "logger.h"
#include "blur.h"
#include <cmath>

void unsharpMask(const deValue* source, const deValue* mask, deValue* destination, int n, deValue a)
{
    int i;
    for (i = 0; i < n; i++)
    {
        deValue src = source[i];
        deValue u = src - mask[i];

        deValue d = src + a * u;

        if (d < 0)
        {
            destination[i] = 0;
        }
        else if (d > 1)
        {
            destination[i] = 1;
        }
        else
        {    
            destination[i] = d;
        }               
    }
}        

void unsharpMask(const deValue* source, const deValue* mask, deValue* destination, int n, deValue a, deValue t)
{
    int i;
    for (i = 0; i < n; i++)
    {
        deValue src = source[i];
        deValue u = src - mask[i];

        if (fabs(2 * u) >= t)
        {
            deValue d = src + a * u;

            if (d < 0)
            {
                destination[i] = 0;
            }
            else if (d > 1)
            {
                destination[i] = 1;
            }
            else
            {    
                destination[i] = d;
            }               
        }
        else
        {
            destination[i] = src;
        }
    }
}        

bool unsharpMask(const deValue* source, deValue* destination, deSize& size, deValue a, deValue r, deValue t, deBlurType type)
{
    int n = size.getN();

    deValue* mask = NULL;
    try
    {
        mask = new deValue [n];
    }
    catch (std::bad_alloc)
    {
        logError("ERROR allocating memory in USM");
        if (mask)
        {
            delete [] mask;
        }
        return false;
    }

    deValue b_t = 0.0;

    if (!blurChannel(source, mask, size, r, r, type, b_t))
    {
        return false;
    }

    if (t > 0)
    {
        unsharpMask(source, mask, destination, n, a, t);
    }
    else
    {
        unsharpMask(source, mask, destination, n, a);
    }

    delete [] mask;

    return true;
}        


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

#include "mixer.h"
#include "str.h"
#include <sstream>
#include <cassert>
#include "logger.h"
#include "image.h"

deMixer::deMixer(int _size)
:size(_size)
{
    weights = new deValue [size];
}

deMixer::~deMixer()
{
    delete [] weights;
}

deValue deMixer::getWeight(int c) const
{
    lock();

    if ((c < 0) || (c >= size))
    {
        unlock();
        return 0.0;
    }

    unlock();
    return weights[c];
}

void deMixer::setWeight(int c, deValue value)
{
    if ((c < 0) || (c >= size))
    {
        return;
    }

    lock();

    weights[c] = value;

    unlock();
}

void deMixer::process(const deImageOld& sourceImage, deValue* destination, int n)
{
    if (size < 1)
    {
        return;
    }

    int i;

    lock();

    const deValue *p1 = sourceImage.startRead(0);

    // FIXME clipping

    if (size == 1)
    {
        for (i = 0; i < n; i++)
        {
            deValue result = weights[0] * p1[i];
            destination[i] = result;
        }
    }        

    if (size > 2)
    {
        const deValue *p2 = sourceImage.startRead(1);
        const deValue *p3 = sourceImage.startRead(2);

        if (size == 3)
        {
            for (i = 0; i < n; i++)
            {
                deValue result = weights[0] * p1[i];
                result += weights[1] * p2[i];
                result += weights[2] * p3[i];
                destination[i] = result;
            }
        }        

        if (size == 4)
        {
            const deValue *p4 = sourceImage.startRead(3);
            for (i = 0; i < n; i++)
            {
                deValue result = weights[0] * p1[i];
                result += weights[1] * p2[i];
                result += weights[2] * p3[i];
                result += weights[3] * p4[i];
                destination[i] = result;
            }
            sourceImage.finishRead(3);
        }

        sourceImage.finishRead(1);
        sourceImage.finishRead(2);
    }

    sourceImage.finishRead(0);
    unlock();
}

bool deMixer::isNeutral(int index) const
{
    int i;
    for (i = 0; i < size; i++)
    {
        if (i == index)
        {
            if (weights[i] != 1.0)
            {
                return false;
            }
        }
        else
        {
            if (weights[i] != 0.0)
            {
                return false;
            }
        }
    }
    return true;
}

void deMixer::lock() const
{
    mutex.lock();
}

void deMixer::unlock() const
{
    mutex.unlock();
}

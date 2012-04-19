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
#include "channel.h"
#include "str.h"
#include "xml.h"
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

void deMixer::reset(int index)
{
    lock();

    int i;
    for (i = 0; i < size; i++)
    {
        if (i == index)
        {
            weights[i] = 1.0;
        }
        else
        {
            weights[i] = 0.0;
        }
    }

    unlock();
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

void deMixer::process(const deImage& sourceImage, deChannel& destination, int n)
{
    if (size < 1)
    {
        return;
    }

    int i;

    lock();

    const deValue *p1 = sourceImage.getValues(0);
    if (size == 1)
    {
        for (i = 0; i < n; i++)
        {
            deValue result = weights[0] * p1[i];
            destination.setValueClip(i, result);
        }
        unlock();
        return;
    }        

    const deValue *p2 = sourceImage.getValues(1);
    const deValue *p3 = sourceImage.getValues(2);
    if (size == 3)
    {
        for (i = 0; i < n; i++)
        {
            deValue result = weights[0] * p1[i];
            result += weights[1] * p2[i];
            result += weights[2] * p3[i];
            destination.setValueClip(i, result);
        }
        unlock();
        return;
    }        

    const deValue *p4 = sourceImage.getValues(3);
    for (i = 0; i < n; i++)
    {
        deValue result = weights[0] * p1[i];
        result += weights[1] * p2[i];
        result += weights[2] * p3[i];
        result += weights[3] * p4[i];
        destination.setValueClip(i, result);
    }

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

void deMixer::save(xmlNodePtr node) 
{
    int i;
    for (i = 0; i < size; i++)
    {
        saveChild(node, "weight", str(weights[i]));
    }
}

void deMixer::load(xmlNodePtr node) 
{
    xmlNodePtr child = node->xmlChildrenNode;

    int i = 0;
    while (child)
    {
        if ((!xmlStrcmp(child->name, BAD_CAST("weight")))) 
        {
            assert(i < size);
            weights[i] = getValue(getContent(child));
            i++;
        }            

        child = child->next;
    }
}

void deMixer::lock() const
{
    lockWithLog(mutex, "mixer mutex");
}

void deMixer::unlock() const
{
    mutex.unlock();
}

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

#include "equalizer.h"
#include "channel.h"
#include "str.h"
#include "xml.h"
#include <sstream>
#include <cassert>
#include "logger.h"

deEqualizer::deEqualizer(int _size)
:bands(_size),
 mutex(wxMUTEX_RECURSIVE)
{
    values = new deValue [bands];

    reset();
}

deEqualizer::~deEqualizer()
{
    delete [] values;
}

void deEqualizer::reset()
{
    lock();

    int i;
    for (i = 0; i < bands; i++)
    {
        values[i] = 0.0;
    }

    unlock();
}

deValue deEqualizer::getValue(int c) const
{
    lock();

    deValue v = 0.0;

    if ((c >= 0) && (c < bands))
    {
        v = values[c];
    }
    
    unlock();

    return v;
}

void deEqualizer::setValue(int c, deValue value)
{
    if ((c < 0) || (c >= bands))
    {
        return;
    }

    lock();

    values[c] = value;

    unlock();
}

void deEqualizer::process(const deChannel& s, const deChannel& e, deChannel& destination, int n, bool wrap)
{
    lock();

    deValue limitScale = 3.0;

    const deValue *ps = s.getPixels();
    const deValue *pe = e.getPixels();

    deValue bandSize = 1.0 / bands;

    deValue limit = bandSize * limitScale;
    deValue scale = 1.0 / limit;

    int i;
    for (i = 0; i < n; i++)
    {
        deValue ve = pe[i];
        deValue vs = ps[i];

        deValue delta = 0.0;
        int n = 0;

        deValue center = bandSize * 0.5;
        int j;
        for (j = 0; j < bands; j++)
        {
            deValue d = fabs(center - ve);
            deValue r = scale * (limit - d);
            if (r > 0)
            {
                deValue v = r * values[j];
                delta += v;
                n++;
            }
            else
                if (wrap)
                {
                    if (ve < 0.5)
                    {
                        ve += 1.0;
                    }
                    else
                    {
                        ve -= 1.0;
                    }
                    deValue d = fabs(center - ve);
                    deValue r = scale * (limit - d);
                    if (r > 0)
                    {
                        deValue v = r * values[j];
                        delta += v;
                        n++;
                    }
                }
            center += bandSize;
        }

        if (n > 0)
        {
            delta /= n;
        }

        deValue result = vs + delta;

        destination.setValueClip(i, result);
    }

    unlock();
}

void deEqualizer::save(xmlNodePtr node) 
{
/*
    int i;
    for (i = 0; i < size; i++)
    {
        saveChild(node, "weight", str(weights[i]));
    }
    */
}

void deEqualizer::load(xmlNodePtr node) 
{
    xmlNodePtr child = node->xmlChildrenNode;

    int i = 0;
    while (child)
    {
    /*
        if ((!xmlStrcmp(child->name, BAD_CAST("weight")))) 
        {
            assert(i < size);
            weights[i] = getValue(getContent(child));
            i++;
        }            
        */

        child = child->next;
    }
}

void deEqualizer::lock() const
{
    lockWithLog(mutex, "mixer mutex");
}

void deEqualizer::unlock() const
{
    mutex.Unlock();
}

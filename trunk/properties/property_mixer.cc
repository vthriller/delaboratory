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

#include "property_mixer.h"
#include "mixer.h"
#include "logger.h"
#include "str.h"

dePropertyMixer::dePropertyMixer(const std::string& _name, int _size)
:deProperty(_name), size(_size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        mixers.push_back( new deMixer(size));
        //mixers[i]->reset(i);
    }
}

dePropertyMixer::~dePropertyMixer()
{
    int i;
    for (i = 0; i < size; i++)
    {
        delete mixers[i];
    }
}

const deMixer* dePropertyMixer::getMixer(int index) const
{
    if (index < 0)
    {
        logError("getMixer index: " + str(index));
        return NULL;
    }
    if (index >= size)
    {
        logError("getMixer index: " + str(index));
        return NULL;
    }

    return mixers[index];
}

deMixer* dePropertyMixer::getMixer(int index)
{
    if (index < 0)
    {
        logError("getMixer index: " + str(index));
        return NULL;
    }
    if (index >= size)
    {
        logError("getMixer index: " + str(index));
        return NULL;
    }

    return mixers[index];
}

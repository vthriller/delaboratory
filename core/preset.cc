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

#include "preset.h"
#include "property_numeric.h"

void dePreset::apply(deBaseLayerWithProperties& layer) const
{
    std::map<std::string, deValue>::const_iterator i;
    for (i = numericValues.begin(); i != numericValues.end(); i++)
    {
        dePropertyNumeric* p = layer.getPropertyNumeric(i->first);
        if (p)
        {
            p->set(i->second);
        }
    }

    std::vector<std::string>::const_iterator j;
    for (j = operations.begin(); j != operations.end(); j++)
    {
        layer.executeOperation(*j);
    }
}

void dePreset::addNumericValue(const std::string& n, deValue v)
{
    numericValues[n] = v;
}

void dePreset::addOperation(const std::string& n)
{
    operations.push_back(n);
}

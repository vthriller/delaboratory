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

#include "power.h"
#include <cmath>
#include "logger.h"
#include "str.h"

dePower::dePower(double _power, int s)
{
    power = _power;

    scale = (POWER_CACHE_SIZE - 1.0) / s;

    logMessage("create dePower power: " + str((float)power) + " scale: " +str((float)scale));

    int i;
    for (i = 0; i < POWER_CACHE_SIZE; i++)
    {
        double p = i / scale;
        values[i] = pow(p, power);
    }

}
    
dePower::~dePower()
{
    logMessage("destroy dePower power: " + str((float)power) + " scale: " +str((float)scale));
}

double dePower::get(double v)
{
    int i = v * scale;
    if ((i >= 0) && (i < POWER_CACHE_SIZE))
        return values[i];
    return pow(v, power);
}


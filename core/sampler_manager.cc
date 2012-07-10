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

#include "sampler_manager.h"
#include "layer_processor.h"
#include <cmath>

deSamplerManager::deSamplerManager()
{
    size = 5;

    int i;
    for (i = 0; i < size; i++)
    {
        deSampler s;
        samplers.push_back(s);
    }        

    selected = -1;
    moving = false;
}

deSamplerManager::~deSamplerManager()
{
}


deSampler* deSamplerManager::getSampler(int index)
{
    if ((index < 0) || ((unsigned int)index >= samplers.size()))
    {
        return NULL;
    }

    return &(samplers[index]);
}

bool deSamplerManager::select(deValue x, deValue y)
{
    int i;

    for (i = 0; i < size; i++)
    {
        deSampler& s = samplers[i];
        s.setSelected(false);
    }        

    for (i = 0; i < size; i++)
    {
        deSampler& s = samplers[i];
        if (s.isEnabled())
        {
            deValue dx = s.getX() - x;
            deValue dy = s.getY() - y;
            deValue r = sqrt(dx*dx + dy*dy);

            if (r < 0.01)
            {
                selected = i;
                s.setSelected(true);
                return true;
            }
        }
    }        

    for (i = 0; i < size; i++)
    {
        deSampler& s = samplers[i];
        if (!s.isEnabled())
        {
            s.enable();
            selected = i;
            s.setSelected(true);
            return true;
        }
    }        

    return false;
}

bool deSamplerManager::onClick(deValue x, deValue y)
{
    if (!moving)
    {
        return false;
    }

    if (select(x,y))
    {
        deSampler& s = samplers[selected];
        s.setPosition(x, y);

        return true;
    }

    return false;

}

bool deSamplerManager::onMove(deValue x, deValue y)
{
    if (!moving)
    {
        return false;
    }

    if (selected >= 0)
    {
        deSampler& s = samplers[selected];

        if ((x < -0.01) || (y < -0.01) || (x>1.01) || (y>1.01))
        {
            s.disable();
            selected = -1;
            s.setSelected(false);
        }
        else
        {
            s.setPosition(x, y);
        }            

        return true;
    }

    return false;
}

bool deSamplerManager::onRelease()
{
    int i;

    for (i = 0; i < size; i++)
    {
        deSampler& s = samplers[i];
        s.setSelected(false);
    }        

    selected = -1;

    return true;
}

void deSamplerManager::setMoving(bool m)
{
    moving = m;
}

bool deSamplerManager::getMoving() const
{
    return moving;
}

void deSamplerManager::clear()
{
    std::vector<deSampler>::iterator i;
    for (i = samplers.begin(); i != samplers.end(); i++)
    {
        (*i).disable();
    }
}

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

#include "sampler_list.h"
#include <iostream>
#include "sampler.h"
#include "sampler_list_panel.h"
#include "project.h"

deSamplerList::deSamplerList()
{

    selected = -1;
    project = NULL;
}

deSamplerList::~deSamplerList()
{
    while (samplers.size() > 0)
    {
        deSamplers::iterator i = samplers.begin();
        delete *i;
        samplers.erase(i);
    }
}

void deSamplerList::setProject(deProject* _project)
{
    assert(!project);
    project = _project;
    samplers.push_back(new deSampler());
    samplers.push_back(new deSampler());
    samplers.push_back(new deSampler());
    samplers.push_back(new deSampler());
    samplers.push_back(new deSampler());
}

void deSamplerList::click(float x, float y)
{
    if (selected >= 0)
    {
        deselect();
    }

    deSamplers::const_iterator j;
    int n = 0;
    for (j = samplers.begin(); j != samplers.end(); j++)
    {
        const deSampler* sampler = *j;
        if (sampler->isEnabled())
        {
            float xx = x - sampler->getX();
            float yy = y - sampler->getY();
            if (sqrt (xx * xx + yy * yy) < 0.01)
            {
                select(n);
                return;
            }
        }            
        n++;
    }

    addSampler(x, y);
}

void deSamplerList::select(int sampler)
{
    assert(selected < 0);
    selected = sampler;

    project->getLayerStack().traceSampler((samplers[selected]));
    project->getGUI().updateSamplerList();
    project->getGUI().refreshView();
}

void deSamplerList::deselect()
{
//    assert(selected >= 0);
    selected =  -1;

    project->getGUI().updateSamplerList();
    project->getGUI().refreshView();
}

void deSamplerList::release(float x, float y)
{
    if (selected >= 0)
    {
        deselect();
    }

}

void deSamplerList::move(float x, float y)
{
    if (selected >= 0)
    {
        if ((x < 0) || (y < 0) || (x>=1) || (y>=1))
        {
            removeSampler();
        }
        else
        {
            moveSampler(x, y);
            project->getLayerStack().traceSampler((samplers[selected]));
        }
    }

}

void deSamplerList::moveSampler(float x, float y)
{
    samplers[selected]->setPosition(x, y);
    project->getGUI().updateSamplerList();
    project->getGUI().refreshView();
}

void deSamplerList::addSampler(float x, float y)
{
    deSamplers::iterator j;
    int n = 0;
    for (j = samplers.begin(); j != samplers.end(); j++)
    {
        deSampler* sampler = *j;
        if (!sampler->isEnabled())
        {
            sampler->setPosition(x,y);
            sampler->enable();
            selected = n;
            return;
        }
        n++;
    }

    j = samplers.begin();
    deSampler* sampler = *j;
    sampler->setPosition(x,y);
    selected = 0;
}

void deSamplerList::removeSampler()
{
    samplers[selected]->disable();
    deselect();
}

void deSamplerList::removeAllSamplers()
{
    deSamplers::iterator j;
    for (j = samplers.begin(); j != samplers.end(); j++)
    {
        (*j)->disable();
    }
    deselect();
}


deSamplers& deSamplerList::getSamplers() 
{
    return samplers;
}

int deSamplerList::getSelected() const
{
    return selected;
}

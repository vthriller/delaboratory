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
#include "project.h"

deSamplerManager::deSamplerManager(deProject& _project)
:project(_project)
{
    int i;
    for (i = 0; i < 5; i++)
    {
        deSampler s;
        samplers.push_back(s);
    }        

    selected = 0;
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

void deSamplerManager::onImageClick(deValue x, deValue y)
{
    deSampler& s = samplers[selected];

    s.setPosition(x, y);

    project.updateSamplers();
    project.repaintImage();
}

void deSamplerManager::setSelected(int s)
{
    selected = s;
    project.repaintImage();
}

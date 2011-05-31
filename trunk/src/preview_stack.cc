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

#include "preview_stack.h"
#include "preview.h"
#include "exception.h"
#include "project.h"
#include "layer.h"
#include <iostream>
#include <wx/wx.h>
#include <sstream>

dePreviewStack::dePreviewStack()
{
    project = NULL;
    nowUpdating = false;
    previewsCalculated = 0;
}

dePreviewStack::~dePreviewStack()
{
    while (previews.size() > 0)
    {
        removeTopPreview();
    }
}

dePreview* dePreviewStack::getPreview(int id) 
{
    unsigned int i = id;
    if ((i >= previews.size()) || (id < 0))
    {
        throw deException("request for preview with unknown index");
    }
    if (!nowUpdating)
    {
        realUpdatePreviews();
    }        

    return previews[i];
}

void dePreviewStack::addPreview()
{
    previews.push_back(NULL);
}

void dePreviewStack::removeTopPreview()
{
    std::vector<dePreview*>::iterator i;
    i = previews.end();    
    i--;
    dePreview* preview = *i;
    delete preview;
    previews.erase(i);
}

int dePreviewStack::getSize() const
{
    return previews.size();
}

void dePreviewStack::updatePreviews(int n)
{
    dirty.insert(n);
}

void dePreviewStack::setProject(deProject* _project)
{
    project = _project;
}

void dePreviewStack::realUpdatePreviews()
{
    nowUpdating = true;
    wxStopWatch sw;

    int min = -1;
    std::set<int>::iterator j;
    for (j = dirty.begin(); j!= dirty.end(); j++)
    {
        int v = *j;
        if ((min < 0) || (v < min))
        {
            min = v;
        }
    }
    if (min < 0)
    {
        nowUpdating = false;
        return;
    }

    deLayerStack& layerStack = project->getLayerStack();
    int last = project->getVisibleLayerID();
    int i;
    for (i = min; i <= last; i++)
    {
        dePreview* preview = previews[i];
        if (preview)
        {
            delete preview;
        }
        try
        {
            previews[i] = layerStack.getLayer(i)->createPreview(*this);
        }
        catch (deException& e)
        {
            nowUpdating = false;
            return;
        }
        previewsCalculated++;
    }

    dirty.clear();
    nowUpdating = false;

    long t = sw.Time();
    std::ostringstream oss;
    oss << "up: " << t << std::endl;
    project->getGUI().setInfo(0, oss.str());

    oss.str("");
    oss << previewsCalculated << " pc" << std::endl;
    project->getGUI().setInfo(2, oss.str());
}

void dePreviewStack::refreshView()
{
    project->getGUI().refreshView();
}    

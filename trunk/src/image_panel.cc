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

#include "image_panel.h"
#include "size.h"
#include "layer.h"
#include "project.h"
#include "color_space.h"
#include "preview.h"
#include "channels.h"
#include "channel.h"
#include "sampler_list.h"
#include "sampler.h"
#include "exception.h"

BEGIN_EVENT_TABLE(deImagePanel, wxPanel)
EVT_PAINT(deImagePanel::paintEvent)
END_EVENT_TABLE()

void deImagePanel::click(wxMouseEvent &event)
{
    int ex = event.GetX();
    int ey = event.GetY();
    int xx;
    int yy;
    GetSize(&xx, &yy);
    float x = (float) ex / xx;
    float y = (float) ey / yy;

    deSamplerList& samplerList = project->getSamplerList();

    samplerList.click(x, y);
}

void deImagePanel::release(wxMouseEvent &event)
{
    int ex = event.GetX();
    int ey = event.GetY();
    int xx;
    int yy;
    GetSize(&xx, &yy);
    float x = (float) ex / xx;
    float y = (float) ey / yy;

    deSamplerList& samplerList = project->getSamplerList();

    samplerList.release(x, y);
}

void deImagePanel::move(wxMouseEvent &event)
{
    int ex = event.GetX();
    int ey = event.GetY();
    int xx;
    int yy;
    GetSize(&xx, &yy);
    float x = (float) ex / xx;
    float y = (float) ey / yy;

    deSamplerList& samplerList = project->getSamplerList();

    samplerList.move(x, y);
}

deImagePanel::deImagePanel(wxWindow* parent, deProject* _project)
:wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize), project(_project), renderer(project)
{
    project->getGUI().setImagePanel(this);

    Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(deImagePanel::click));
    Connect(wxEVT_LEFT_UP, wxMouseEventHandler(deImagePanel::release));
    Connect(wxEVT_MOTION, wxMouseEventHandler(deImagePanel::move));
}

deImagePanel::~deImagePanel()
{
}

void deImagePanel::paintEvent(wxPaintEvent & evt)
{
    try
    {
        wxPaintDC dc(this);
        render(dc);
    }
    catch (deException& e)
    {
    }
}

void deImagePanel::paint()
{
    try
    {
        wxClientDC dc(this);
        render(dc);
    }
    catch (deException& e)
    {
    }
}

void deImagePanel::render(wxDC& dc)
{
    renderer.render(dc);

    drawSamplers(dc);
}


void deImagePanel::drawSamplers(wxDC& dc)
{
    int w;
    int h;
    GetSize(&w, &h);

    deSamplerList& samplerList = project->getSamplerList();

    int selected = samplerList.getSelected();

    deSamplers& samplers = samplerList.getSamplers();

    wxPen penBLACK(*wxBLACK);
    wxPen penGREEN(*wxGREEN);

    deSamplers::const_iterator j;
    int n = 0;
    for (j = samplers.begin(); j != samplers.end(); j++)
    {
        const deSampler* sampler = *j;
        if (sampler->isEnabled())
        {
            if (n == selected)
            {
                dc.SetPen(penGREEN);
            }
            else
            {
                dc.SetPen(penBLACK);
            }
            float x = sampler->getX();
            float y = sampler->getY();
            dc.DrawCircle(w * x, h * y, 3);
        }
        n++;
    }
    dc.SetPen(penBLACK);
}

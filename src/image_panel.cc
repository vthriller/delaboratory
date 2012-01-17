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
#include <wx/dcbuffer.h>
#include "project.h"
#include "layer.h"
#include "layer_processor.h"

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

    clicked = true;

    if (!setPosition(x, y))
    {
        grabX = x;
        grabY = y;
    }
}

void deImagePanel::release(wxMouseEvent &event)
{
    clicked = false;
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

    if (!setPosition(x, y))
    {
        if (clicked)
        {
            project->setViewOffset(x - grabX, y - grabY);
            grabX = x;
            grabY = y;
        }
    }
}

void deImagePanel::wheel(wxMouseEvent &event)
{
    //project->zoom(event.GetWheelRotation());
}


bool deImagePanel::setPosition(deValue x, deValue y)
{
    if (clicked)
    {
        bool used = project->getLayerFrameManager().onImageClick(x, y);

        deSamplerManager& samplerManager = project->getSamplerManager();
        samplerManager.onImageClick(x, y);

        return used;
    }
    return false;
}

deImagePanel::deImagePanel(wxWindow* parent, deProject* _project)
:wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize), project(_project), renderer(project)
{
    Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(deImagePanel::click));
    Connect(wxEVT_LEFT_UP, wxMouseEventHandler(deImagePanel::release));
    Connect(wxEVT_MOTION, wxMouseEventHandler(deImagePanel::move));
    Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(deImagePanel::wheel));
    clicked = false;
}

deImagePanel::~deImagePanel()
{
}

void deImagePanel::paintEvent(wxPaintEvent & evt)
{
    project->getLayerProcessor().lock();
    int view = project->getLayerProcessor().getLastValidLayer();
    if (view >= 0)
    {
        wxBufferedPaintDC dc(this);
        dc.Clear();
        render(dc);
    }
    else
    {
        std::cout << "view: " << view << " in paintEvent" << std::endl;
    }
    project->getLayerProcessor().unlock();
}

void deImagePanel::repaintImagePanel()
{
//    project->getLayerProcessor().lock();

    int view = project->getLayerProcessor().getLastValidLayer();
    if (view >= 0)
    {
        wxClientDC dc(this);
        wxBufferedDC bufferedDC(&dc);
        bufferedDC.Clear();
        render(bufferedDC);
    }
    else
    {
        std::cout << "view: " << view << " in repaintImagePanel" << std::endl;
    }
//    project->getLayerProcessor().unlock();
}

void deImagePanel::render(wxDC& dc)
{
//    std::cout << "deImagePanel::render" << std::endl;
//    renderer.prepareImage();
    renderer.render(dc);
    if (project->samplersVisible())
    {
        drawSamplers(dc);
    }        
}


void deImagePanel::drawSamplers(wxDC& dc)
{
    int w;
    int h;
    GetSize(&w, &h);

    deSamplerManager& samplerManager = project->getSamplerManager();
    int n = samplerManager.getNumberOfSamplers();
    int selected = samplerManager.getSelected();

    int i;

    wxPen penBLACK(*wxBLACK);
    wxPen penGREEN(*wxGREEN);

    for (i = 0; i < n; i++)
    {
        deSampler* sampler = samplerManager.getSampler(i);
        if (i == selected)
        {
            dc.SetPen(penGREEN);
        }
        else
        {
            dc.SetPen(penBLACK);
        }
        float x = sampler->getX();
        float y = sampler->getY();

        if ((x >= 0) && (y >= 0) && (x <= 1) && (y<= 1))
        {
            dc.DrawCircle(w * x, h * y, 5);
        }            
    }

}


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
#include "base_layer.h"
#include "layer_processor.h"
#include <iostream>
#include "str.h"
#include "layer_frame_manager.h"
#include "zoom_manager.h"
#include "zoom_panel.h"
#include "image_area_panel.h"
#include "canvas_wx.h"

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

    onClick(x,y);

}

void deImagePanel::release(wxMouseEvent &event)
{
    clicked = false;

    onRelease();

    project.getLayerProcessor().onGUIUpdate();
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

    if (clicked)
    {
        if (onMove(x,y))
        {
            project.getLayerProcessor().onGUIUpdate();
        }
    }        

}

void deImagePanel::wheel(wxMouseEvent &event)
{
}


bool deImagePanel::onClick(deValue x, deValue y)
{
    if (zoomManager.isInSelectionMode())
    {
        bool result = zoomManager.onClick(x, y);
        zoomPanel->updateButtons();
        return result;
    }

    bool used = project.getLayerFrameManager().onImageClick(x, y);

    if (!used)
    {
        used = samplerManager.onClick(x, y);
        if (used)
        {
            project.getLayerProcessor().onGUIUpdate();
        }
    }            

    return used;
}

bool deImagePanel::onMove(deValue x, deValue y)
{
    if (zoomManager.isInSelectionMode())
    {
        bool result = zoomManager.onMove(x, y);
        zoomPanel->updateButtons();
        return result;
    }

    bool used = project.getLayerFrameManager().onImageClick(x, y);

    if (!used)
    {
        used = samplerManager.onMove(x, y);
    }            

    return used;
}

bool deImagePanel::onRelease()
{
    if (zoomManager.isInSelectionMode())
    {
        bool result = zoomManager.onRelease();
        zoomPanel->updateButtons();
        area->updateSize(false);
        return result;
    }

    samplerManager.onRelease();

    return false;
}

deImagePanel::deImagePanel(deImageAreaPanel* _area, deProject& _project, deSamplerManager& _samplerManager, deZoomManager& _zoomManager, deZoomPanel* _zoomPanel)
:wxPanel(_area, wxID_ANY, wxDefaultPosition, wxDefaultSize), area(_area), project(_project), samplerManager(_samplerManager), zoomManager(_zoomManager), zoomPanel(_zoomPanel)
{
    clicked = false;

    Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(deImagePanel::click));
    Connect(wxEVT_LEFT_UP, wxMouseEventHandler(deImagePanel::release));
    Connect(wxEVT_MOTION, wxMouseEventHandler(deImagePanel::move));
    Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(deImagePanel::wheel));
}

deImagePanel::~deImagePanel()
{
}

void deImagePanel::paintEvent(wxPaintEvent & evt)
{
    project.getLayerProcessor().lock();
    int view = project.getLayerProcessor().getLastValidLayer();
    if (view >= 0)
    {
        wxBufferedPaintDC dc(this);

        deCanvasWX canvas(dc);

        canvas.clear();

        render(canvas);
    }
    else
    {
    }
    project.getLayerProcessor().unlock();
}

void deImagePanel::repaintImagePanel()
{
    int view = project.getLayerProcessor().getLastValidLayer();
    if (view >= 0)
    {
        wxClientDC dc(this);
        wxBufferedDC bufferedDC(&dc);

        deCanvasWX canvas(bufferedDC);

        canvas.clear();

        render(canvas);
    }
    else
    {
    }
}

void deImagePanel::render(deCanvas& canvas)
{
    project.getLayerProcessor().render(canvas);

    if (samplerManager.getMoving())
    {
        drawSamplers(canvas);
    }        
    if (zoomManager.isInSelectionMode())
    {
        drawSelection(canvas);
    }        
}


void deImagePanel::drawSamplers(deCanvas& canvas)
{
    int w;
    int h;
    GetSize(&w, &h);

    int n = samplerManager.getNumberOfSamplers();
    int selected = samplerManager.getSelected();

    int i;

    for (i = 0; i < n; i++)
    {
        deSampler* sampler = samplerManager.getSampler(i);
        if (sampler->isEnabled())
        {
            if (i == selected)
            {
                canvas.setPen(200, 200, 200);
            }
            else
            {
                canvas.setPen(0, 0, 0);
            }
            float x = sampler->getX();
            float y = sampler->getY();

            if ((x >= 0) && (y >= 0) && (x <= 1) && (y<= 1))
            {
                canvas.drawCircle(w * x, h * y, 5);
            }
        }
    }

}

void deImagePanel::drawSelection(deCanvas& canvas)
{
    int w;
    int h;
    GetSize(&w, &h);

    deValue x1;
    deValue y1;
    deValue x2;
    deValue y2;

    zoomManager.getSelection(x1, y1, x2, y2);

    int xx1 = w * x1;
    int yy1 = h * y1;
    int xx2 = w * x2;
    int yy2 = h * y2;

    canvas.setPen(220, 220, 220);

    canvas.drawLine(xx1, yy1, xx2, yy1);
    canvas.drawLine(xx1, yy2, xx2, yy2);
    canvas.drawLine(xx1, yy1, xx1, yy2);
    canvas.drawLine(xx2, yy1, xx2, yy2);

}


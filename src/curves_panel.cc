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

#include "curves_panel.h"
#include <wx/dcbuffer.h>
#include "curves_layer.h"


BEGIN_EVENT_TABLE(deCurvesPanel, wxPanel)
EVT_PAINT(deCurvesPanel::paintEvent)
END_EVENT_TABLE()

deCurvesPanel::deCurvesPanel(wxWindow* parent, deCurvesLayer& _layer)
:wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(CURVES_PANEL_SIZE_X, CURVES_PANEL_SIZE_Y)),
sizeX(CURVES_PANEL_SIZE_X), sizeY(CURVES_PANEL_SIZE_Y), layer(_layer), histogram(CURVES_PANEL_SIZE_X)
{
    SetFocus();
    Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(deCurvesPanel::click));
    Connect(wxEVT_LEFT_UP, wxMouseEventHandler(deCurvesPanel::release));
    Connect(wxEVT_MOTION, wxMouseEventHandler(deCurvesPanel::move));
    channel = 0;
    selectedPoint = -1;
    marker = -1;

    backgroundBitmap = NULL;
    clickPosition = -1;

    realtime = false;

    generateBackground();
}

deCurvesPanel::~deCurvesPanel()
{
    delete backgroundBitmap;
}

void deCurvesPanel::generateBackground()
{
    if (backgroundBitmap)
    {
        delete backgroundBitmap;
    }

    deChannel* c = layer.getSourceChannel(channel);
    int n = layer.getChannelSize().getN();

    histogram.clear();
    histogram.calc(c, n);

    wxImage* image = new wxImage(sizeX, sizeY);
    unsigned char* data = image->GetData();

    unsigned char g1 = 220;
    unsigned char g2 = 120;

    histogram.render(data, sizeX, sizeY, g1, g2);

    backgroundBitmap = new wxBitmap(*image);
    delete image;
}


void deCurvesPanel::paintEvent(wxPaintEvent & evt)
{
    wxPaintDC dc(this);
    render(dc);
}

void deCurvesPanel::render(wxDC& dc_orig)
{
    wxBufferedDC dc(&dc_orig, bitmap, wxBUFFER_CLIENT_AREA);

    int g = 50;
    wxColour colourB(g, g, g);
    wxBrush brush(colourB);

    dc.DrawBitmap(*backgroundBitmap, 0, 0, false);

    drawLines(dc);

    wxColour colour = getChannelwxColour(layer.getColorSpace(), channel);
    wxPen pen(colour);
    dc.SetPen(pen);

    drawCurve(dc);
}

void deCurvesPanel::drawPoint(wxDC& dc, deValue x, deValue y)
{
    int xx = (sizeX - 1) * x;
    int yy = (sizeY - 1) * (1 - y);
    dc.DrawCircle(xx, yy, 5);
}


void deCurvesPanel::drawLine(wxDC& dc, deValue x1, deValue y1, deValue x2, deValue y2)
{
    int xx1 = (sizeX - 1) * x1;
    int xx2 = (sizeX - 1) * x2;
    int yy1 = (sizeY - 1) * (1 - y1);
    int yy2 = (sizeY - 1) * (1 - y2);
    dc.DrawLine(xx1, yy1, xx2, yy2);
}

void deCurvesPanel::drawLines(wxDC& dc)
{
    int g1 = 180;
    int g2 = 80;

    wxPen pen1(wxColour(g1, g1, g1));
    wxPen pen2(wxColour(g2, g2, g2));

    dc.SetPen(pen1);

    float y;
    for (y = 0.0; y <= 1.0; y += (1/8.0))
    {
        drawLine(dc, 0.0, y, 1.0, y);
    }        

    float x;
    for (x = 0.0; x <= 1.0; x += (1/8.0))
    {
        drawLine(dc, x, 0.0, x, 1.0);
    }        

    dc.SetPen(pen2);

    if (marker >= 0)
    {
        drawLine(dc, 0.0, marker, 1.0, marker);
        drawLine(dc, marker, 0.0, marker, 1.0);
    }        
}

void deCurvesPanel::drawCurve(wxDC& dc)
{
    deCurve* curve = layer.getCurve(channel);

    if (!curve)
    {
        return;
    }

    deCurvePoints::const_iterator i;

    deCurvePoints curvePoints;
    curve->getCurvePoints(curvePoints);

    deValue xp = -1;
    deValue yp = -1;

    for (i = curvePoints.begin(); i != curvePoints.end(); i++)
    {
        const deCurvePoint& point = *i;
        deValue x = point.getX();
        deValue y = point.getY();
        if (xp >= 0)
        {
            drawLine(dc, xp, yp, x, y);
        }
        xp = x;
        yp = y;
    }

    deCurvePoints controlPoints;
    curve->getControlPoints(controlPoints);


    int index = 0;
    for (i = controlPoints.begin(); i != controlPoints.end(); i++)
    {
        const deCurvePoint& point = *i;
        deValue x = point.getX();
        deValue y = point.getY();
        drawPoint(dc, x, y);
        index++;
    }
}

void deCurvesPanel::click(wxMouseEvent &event)
{
    deCurve* curve = layer.getCurve(channel);

    if (!curve)
    {
        return;
    }

    selectedPoint = -1;

    deValue x;
    deValue y;
    getPosition(event, x, y);
    int n = curve->findPoint(x, y);

    if (n >= 0)
    {
        selectedPoint = n;
    }
    else
    {
        selectedPoint = curve->addPoint(x, y);
    }

    lastSelectedPoint = selectedPoint;

    const deCurvePoint& point = curve->getPoint(selectedPoint);
    grabX = point.getX() - x;
    grabY = point.getY() - y;

    update(false);
}

void deCurvesPanel::reset()
{
    deCurve* curve = layer.getCurve(channel);

    if (!curve)
    {
        return;
    }
    curve->reset();
    update(true);
}

void deCurvesPanel::invert()
{
    deCurve* curve = layer.getCurve(channel);

    if (!curve)
    {
        return;
    }
    curve->invert();
    update(true);
}

void deCurvesPanel::setConst(deValue v)
{
    deCurve* curve = layer.getCurve(channel);

    if (!curve)
    {
        return;
    }
    curve->setConst(v);
    update(true);
}

void deCurvesPanel::setAngle(int a)
{
    deCurve* curve = layer.getCurve(channel);

    if (!curve)
    {
        return;
    }
    curve->setAngle(a);
    update(true);
}

void deCurvesPanel::setS(int a)
{
    deCurve* curve = layer.getCurve(channel);

    if (!curve)
    {
        return;
    }
    curve->setS(a);
    update(true);
}


void deCurvesPanel::update(bool finished)
{
    paint();
    if ((finished) || (realtime))
    {
        layer.onChannelChange(channel);
        layer.updateOtherLayers();
        layer.repaint();
    }
}

void deCurvesPanel::release(wxMouseEvent &event)
{
    deCurve* curve = layer.getCurve(channel);

    if (!curve)
    {
        return;
    }

    if (selectedPoint)
    {
        deValue x;
        deValue y;
        getPosition(event, x, y);
        if ((x < -0.1) || (y < -0.1) || (x>1.1) || (y>1.1))
        {
            curve->deletePoint(selectedPoint);
            lastSelectedPoint = -1;
            selectedPoint = -1;
            update(true);
            return;
        }
    }

    selectedPoint = -1;

    update(true);

}

void deCurvesPanel::move(wxMouseEvent &event)
{
    deCurve* curve = layer.getCurve(channel);

    if (!curve)
    {
        return;
    }

    if (selectedPoint >= 0)
    {
        deValue x;
        deValue y;
        getPosition(event, x, y);

        x+= grabX;
        y+= grabY;

        if (x < 0)
        {
            x = 0;
        }
        if (y < 0)
        {
            y = 0;
        }
        if (x > 1)
        {
            x = 1;
        }
        if (y > 1)
        {
            y = 1;
        }

        curve->movePoint(selectedPoint, x, y);
        update(false);
    }
}

void deCurvesPanel::getPosition(wxMouseEvent &event, deValue& x, deValue &y)
{
    deValue dX = 1.0 / (sizeX - 1);
    deValue dY = 1.0 / (sizeY - 1);
    x = event.GetX() * dX;
    y = 1 - (event.GetY() * dY);
}

void deCurvesPanel::paint()
{
    wxClientDC dc(this);
    render(dc);
}

void deCurvesPanel::changeChannel(int _channel)
{
    channel = _channel;
    generateBackground();
    setMarker();
    paint();
}

void deCurvesPanel::onImageClick(deValue x, deValue y)
{
    deSize size = layer.getChannelSize();
    clickPosition = (y * size.getH() )  * size.getW() + (x * size.getW());
    setMarker();
    paint();
}

void deCurvesPanel::setMarker()
{
    if (clickPosition < 0)
    {
        marker = -1;
    }
    else
    {
        deChannel* c = layer.getSourceChannel(channel);
        marker = c->getValue(clickPosition);
    }        
}

void deCurvesPanel::onKey(int key)
{
    deColorSpace colorSpace = layer.getColorSpace();
    int s = getColorSpaceSize(colorSpace);

    if (key == WXK_CONTROL)
    {
        if (clickPosition >= 0)
        {
            int i;
            for (i = 0; i < s; i++)
            {
                deChannel* c = layer.getSourceChannel(i);
                deValue m = c->getValue(clickPosition);

                deCurve* curve = layer.getCurve(i);

                curve->addPoint(m, curve->calcValue(m));
            }
            update(true);
        }
        return;
    }
    deValue delta = 0.0;

    if (key == 'A')
    {
        delta = 1.0;
    }

    if (key == 'Z')
    {
        delta = -1.0;
    }

    if (key == 'R')
    {
        realtime = !realtime;
    }

    if (delta != 0.0)
    {
        deValue dY = delta / (sizeY - 1);
        if (lastSelectedPoint >= 0)
        {
            deCurve* curve = layer.getCurve(channel);
            curve->movePointVertically(lastSelectedPoint, dY);
            update(true);
        }
    }        

    if (key == 'X')
    {
        if (lastSelectedPoint >= 0)
        {
            deCurve* curve = layer.getCurve(channel);
            curve->deletePoint(lastSelectedPoint);
            lastSelectedPoint = -1;
            update(true);
        }
    }

}

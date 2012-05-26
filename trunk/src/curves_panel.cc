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
#include "layer_processor.h"
#include "logger.h"
#include "channels.h"
#include "color_space_utils.h"
#include "channel_manager.h"
#include "property_curves.h"

wxColour getChannelwxColour(deColorSpace colorSpace, int channel)
{
    switch (colorSpace)
    {
        case deColorSpaceRGB:
        case deColorSpaceProPhoto:
        {
            int g = 200;
            switch (channel)
            {
                case DE_CHANNEL_RED:
                    return wxColour(g, 0, 0);
                case DE_CHANNEL_GREEN:
                    return wxColour(0, g, 0);
                case DE_CHANNEL_BLUE:
                    return wxColour(0, 0, g);
            }                    
        }            
        case deColorSpaceCMYK:
        case deColorSpaceCMY:
        {
            int g = 240;
            int g2 = 100;
            switch (channel)
            {
                case DE_CHANNEL_CYAN:
                    return wxColour(0, g, g);
                case DE_CHANNEL_MAGENTA:
                    return wxColour(g, 0, g);
                case DE_CHANNEL_YELLOW:
                    return wxColour(g, g, 0);
                case DE_CHANNEL_KEY:
                    return wxColour(g2, g2, g2);
            }                    
        }            
        default:
        {
            // in any other cases just use dark gray
            int g = 50;
            return wxColour(g, g, g);
        }            
    }

}


BEGIN_EVENT_TABLE(deCurvesPanel, wxPanel)
EVT_PAINT(deCurvesPanel::paintEvent)
END_EVENT_TABLE()

deCurvesPanel::deCurvesPanel(wxWindow* parent, deLayerProcessor& _layerProcessor, int _layerIndex, dePropertyCurves& _property, deColorSpace _colorSpace)
:wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(CURVES_PANEL_SIZE_X, CURVES_PANEL_SIZE_Y)),
sizeX(CURVES_PANEL_SIZE_X), sizeY(CURVES_PANEL_SIZE_Y), 
layerProcessor(_layerProcessor), layerIndex(_layerIndex), property(_property), colorSpace(_colorSpace)
{
    SetFocus();
    Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(deCurvesPanel::click));
    Connect(wxEVT_LEFT_UP, wxMouseEventHandler(deCurvesPanel::release));
    Connect(wxEVT_MOTION, wxMouseEventHandler(deCurvesPanel::move));
    channel = 0;
    selectedPoint = -1;
    lastSelectedPoint = -1;
    marker = -1;

    backgroundBitmap = NULL;
    clickPosition = -1;

}

deCurvesPanel::~deCurvesPanel()
{
    delete backgroundBitmap;
}

void deCurvesPanel::generateBackground(const deValue* c, int n)
{
    if (backgroundBitmap)
    {
        delete backgroundBitmap;
    }

    deHistogram histogram(CURVES_PANEL_SIZE_X);

    histogram.clear();
    histogram.calc(c, n);

    wxImage* image = new wxImage(sizeX, sizeY);
    unsigned char* data = image->GetData();

    unsigned char g1 = 255;
    unsigned char g2 = 200;
    wxColour colour = getChannelwxColour(colorSpace, channel);

    int margin = 0;

    histogram.render(data, sizeX, sizeY, g1, g2, margin);

    backgroundBitmap = new wxBitmap(*image);
    delete image;
}


void deCurvesPanel::render(wxDC& dc_orig)
{
    wxBufferedDC dc(&dc_orig, bitmap, wxBUFFER_CLIENT_AREA);

    int g = 50;
    wxColour colourB(g, g, g);
    wxBrush brush(colourB);

    if (backgroundBitmap)
    {
        dc.DrawBitmap(*backgroundBitmap, 0, 0, false);
    }        
    else
    {
        dc.Clear();
    }

    drawLines(dc);

    wxColour colour = getChannelwxColour(colorSpace, channel);
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
    deBaseCurve* curve = property.getCurve(channel);

    if (!curve)
    {
        return;
    }

    int i;
    deValue lastY = 0.0;
    deValue lastX = 0.0;
    for (i = 0; i < sizeX; i++)
    {
        deValue x = (deValue) i / (sizeX - 1.0);
        deValue y = curve->calcValue(x);
        if (i > 0)
        {
            drawLine(dc, lastX, lastY, x, y);
        }
        lastY = y;
        lastX = x;
    }

    const deCurvePoints& controlPoints = curve->getControlPoints();
    deCurvePoints::const_iterator j;

    int index = 0;
    for (j = controlPoints.begin(); j != controlPoints.end(); j++)
    {
        const deCurvePoint& point = *j;
        deValue x = point.getX();
        deValue y = point.getY();
        drawPoint(dc, x, y);
        index++;
    }
}

void deCurvesPanel::click(wxMouseEvent &event)
{
    deBaseCurve* curve = property.getCurve(channel);

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
        curve->build();
    }

    lastSelectedPoint = selectedPoint;

    const deCurvePoint& point = curve->getPoint(selectedPoint);
    grabX = point.getX() - x;
    grabY = point.getY() - y;

    update(false);
}

void deCurvesPanel::update(bool finished)
{
    paint();
    if ((finished) || (layerProcessor.isRealtime()))
    {
        layerProcessor.markUpdateSingleChannel(layerIndex, channel);
    }
}

void deCurvesPanel::release(wxMouseEvent &event)
{
    deBaseCurve* curve = property.getCurve(channel);

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
            curve->build();
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
    if (!event.ButtonIsDown(wxMOUSE_BTN_LEFT))
    {
        selectedPoint = -1;
        return;
    }

    deBaseCurve* curve = property.getCurve(channel);

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
        curve->build();
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

void deCurvesPanel::paintEvent(wxPaintEvent & evt)
{
    wxPaintDC dc(this);

    render(dc);
}


void deCurvesPanel::changeChannel(int _channel)
{
    layerProcessor.lock();

    channel = _channel;
    layerProcessor.setHistogramChannel(channel);
    paint();

    layerProcessor.unlock();
}

void deCurvesPanel::onImageClick(deValue x, deValue y, const deValue* c, const deSize& size)
{
    if ((x < 0) || (y < 0) || (x >= 1) || (y >= 1))
    {
        return;
    }
    clickPosition = (y * size.getH() )  * size.getW() + (x * size.getW());
    setMarker(c, size.getN());
    paint();
}

void deCurvesPanel::setMarker(const deValue* c, int n)
{
    if ((clickPosition < 0) || (clickPosition > n))
    {
        marker = -1;
    }
    else
    {
        marker = c[clickPosition];
    }        
}

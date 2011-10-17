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

#ifndef _DE_CURVES_PANEL_H
#define _DE_CURVES_PANEL_H

#define CURVES_PANEL_SIZE_X 384
#define CURVES_PANEL_SIZE_Y 256

#include "wx/wx.h"
class deCurvesLayer;
#include "value.h"
#include "histogram.h"

class deCurvesPanel:public wxPanel
{
private:
    deHistogram histogram;
    wxBitmap bitmap;
    wxBitmap* backgroundBitmap;
    int sizeX;
    int sizeY;
    deCurvesLayer& layer;
    int channel;

    void drawPoint(wxDC& dc, deValue x, deValue y);
    void drawCurve(wxDC& dc);

	void click(wxMouseEvent &event);
	void release(wxMouseEvent &event);
	void move(wxMouseEvent &event);
    void update(bool finished);

    int selectedPoint;
    int lastSelectedPoint;

    deValue grabX;
    deValue grabY;

    void getPosition(wxMouseEvent &event, deValue& x, deValue &y);

    deValue marker;

    int clickPosition;

    void drawLine(wxDC& dc, deValue x1, deValue y1, deValue x2, deValue y2);
    void drawLines(wxDC& dc);

    void generateBackground();
public:
    deCurvesPanel(wxWindow* parent, deCurvesLayer& layer);
    ~deCurvesPanel();

	void paintEvent(wxPaintEvent & evt);
	void render(wxDC& dc);
    void paint();

    void changeChannel(int _channel);
    void onImageClick(deValue x, deValue y);

    void reset();
    void invert();
    void setConst(deValue v);
    void setAngle(int a);

    void onKey(int key);

    void setMarker();

	DECLARE_EVENT_TABLE()

};

#endif

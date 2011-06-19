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

#include "wx/wx.h"
#include "value.h"
#include "sampler.h"
class deCurve;
class deCurvesLayer;
class dePreviewStack;
class dePropertyCurves;

class deCurvesPanel:public wxPanel
{
private:
	void click(wxMouseEvent &event);
	void release(wxMouseEvent &event);
	void move(wxMouseEvent &event);

    void drawPoint(wxDC& dc, deValue x, deValue y);
    void drawLine(wxDC& dc, deValue x1, deValue y1, deValue x2, deValue y2);
    void drawLines(wxDC& dc);
    void drawCurve(wxDC& dc);

    void getPosition(wxMouseEvent &event, deValue& x, deValue &y);

    int selectedPoint;
    deCurvesLayer& layer;
    wxBitmap bitmap;
    int channel;

    deValue grabX;
    deValue grabY;

    dePreviewStack& stack;

    void update();

    deValue marker;

    deSampler sampler;

    dePropertyCurves& property;

    void updateMarker();

public:
	deCurvesPanel(wxWindow* parent, deCurvesLayer& _layer, dePreviewStack& _stack, dePropertyCurves& _property);
	virtual ~deCurvesPanel();

	void paintEvent(wxPaintEvent & evt);
	void render(wxDC& dc);
    void paint();

    void changeChannel(int _channel);
    void traceSampler(deSampler* sampler);

	DECLARE_EVENT_TABLE()

};

#endif

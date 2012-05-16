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
class deLayerProcessor;
class deChannelManager;
class deCanvas;
class dePropertyCurves;
#include "value.h"
#include "histogram.h"
#include "color_space.h"
#include "size.h"

class deCurvesPanel:public wxPanel
{
private:
    wxBitmap bitmap;
    wxBitmap* backgroundBitmap;
    int sizeX;
    int sizeY;
    int channel;
    
    bool realtime;

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

    deLayerProcessor& layerProcessor;

    int layerIndex;

    dePropertyCurves& property;

    deColorSpace colorSpace;

    void drawLine(wxDC& dc, deValue x1, deValue y1, deValue x2, deValue y2);
    void drawLines(wxDC& dc);

public:
    deCurvesPanel(wxWindow* parent, deLayerProcessor& _layerProcessor, int _layerIndex, dePropertyCurves& _property, deColorSpace _colorSpace);
    ~deCurvesPanel();

	void paintEvent(wxPaintEvent & evt);
	void render(wxDC& dc);
    void paint();

    void changeChannel(int _channel);
    void onImageClick(deValue x, deValue y, const deValue* c, const deSize& size);

    void reset();
    void invert();
    void setConst(deValue v);
    void setAngle(int a);
    void setS(int a);

    void onKey(int key);

    void setMarker(const deValue* c, int n);

    void addRandom(int n);

    void generateBackground(const deValue* c, int n);

	DECLARE_EVENT_TABLE()

};

#endif

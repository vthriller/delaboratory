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

#ifndef _DE_IMAGE_PANEL_H
#define _DE_IMAGE_PANEL_H

#include "wx/wx.h"
#include "renderer.h"
#include "value.h"
class deProject;
class deSamplerManager;
class deZoomManager;
class deZoomPanel;
class deImageAreaPanel;
class deCanvas;

class deImagePanel:public wxPanel
{
private:
    deImageAreaPanel* area;

    deProject& project;
    bool clicked;
    deSamplerManager& samplerManager;
    deZoomManager& zoomManager;
    deZoomPanel* zoomPanel;

	void click(wxMouseEvent &event);
	void release(wxMouseEvent &event);
	void move(wxMouseEvent &event);
	void wheel(wxMouseEvent &event);

    bool onClick(deValue x, deValue y);
    bool onMove(deValue x, deValue y);
    bool onRelease();

    void drawSamplers(deCanvas& canvas);
    void drawSelection(deCanvas& canvas);
	void render(deCanvas& canvas);

public:
	deImagePanel(deImageAreaPanel* _area, deProject& _project, deSamplerManager& _samplerManager, deZoomManager& _zoomManager, deZoomPanel* _zoomPanel);
	virtual ~deImagePanel();

	void paintEvent(wxPaintEvent & evt);
    void repaintImagePanel();

	DECLARE_EVENT_TABLE()

};

#endif

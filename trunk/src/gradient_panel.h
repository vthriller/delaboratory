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

#ifndef _DE_GRADIENT_PANEL_H
#define _DE_GRADIENT_PANEL_H

#include <wx/wx.h>
#include "color_space.h"

class deGradientPanel:public wxPanel
{
private:
    wxBitmap* bitmap;

    void generateBitmap();

    deColorSpace colorSpace;
    int channel1;
    int channel2;

    deValue c1;
    deValue c2;
    deValue c3;

public:
    deGradientPanel(wxWindow* parent, wxSize _size, deColorSpace _colorSpace, int _channel1, int _channel2, deValue _c1, deValue _c2, deValue _c3);
    ~deGradientPanel();

	void paintEvent(wxPaintEvent & evt);
    void changeChannel(int _channel);

	DECLARE_EVENT_TABLE()

};

class deGradientPanel2:public wxPanel
{
private:
    wxBitmap* bitmap;

    void generateBitmap();

    deColorSpace colorSpace;

    deValue c1;
    deValue c2;
    deValue c3;
    deValue c4;

public:
    deGradientPanel2(wxWindow* parent, wxSize _size, deColorSpace _colorSpace, deValue _c1, deValue _c2, deValue _c3, deValue _c4);
    ~deGradientPanel2();

	void paintEvent(wxPaintEvent & evt);

	DECLARE_EVENT_TABLE()

};

#endif

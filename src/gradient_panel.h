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
#include "value.h"
class dePalette3;

class deColorPanel:public wxPanel
{
private:
    deValue r;
    deValue g;
    deValue b;
    dePalette3* palette;

protected:
    void click(wxMouseEvent &event);

public:
    deColorPanel(wxWindow* parent, wxSize _size, dePalette3* _palette, int style);
    virtual ~deColorPanel();

    void setRGB(deValue rr, deValue gg, deValue bb);
    void setColor(deColorSpace colorSpace, int channel, deValue value);
};

class deGradientPanel:public wxPanel
{
protected:
    wxBitmap* bitmap;
    deColorSpace colorSpace;

public:
    deGradientPanel(wxWindow* parent, wxSize _size, deColorSpace _colorSpace);
    virtual ~deGradientPanel();

	void paintEvent(wxPaintEvent & evt);
};


class deGradientPanel1:public deGradientPanel
{
private:

    void generateBitmap();

    int channel1;
    int channel2;

    deValue c1;
    deValue c2;
    deValue c3;

public:
    deGradientPanel1(wxWindow* parent, wxSize _size, deColorSpace _colorSpace, int _channel1, int _channel2, deValue _c1, deValue _c2, deValue _c3);
    ~deGradientPanel1();

    void changeChannel(int _channel);


};

class deGradientPanel2:public deGradientPanel
{
private:

    void generateBitmap();

    deValue c1;
    deValue c2;
    deValue c3;
    deValue c4;

public:
    deGradientPanel2(wxWindow* parent, wxSize _size, deColorSpace _colorSpace, deValue _c1, deValue _c2, deValue _c3, deValue _c4);
    ~deGradientPanel2();


};

#endif

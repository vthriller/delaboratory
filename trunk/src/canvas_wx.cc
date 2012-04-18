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

#include "canvas_wx.h"
#include "bitmap_wx.h"

deCanvasWX::deCanvasWX(wxDC& _dc)
:dc(_dc)
{
}

deCanvasWX::~deCanvasWX()
{
}

void deCanvasWX::clear()
{
    dc.Clear();
}

void deCanvasWX::drawCircle(int x, int y, int r)
{
    dc.DrawCircle(x, y, r);
}

void deCanvasWX::drawLine(int x1, int y1, int x2, int y2)
{
    dc.DrawLine(x1, y1, x2, y2);
}    

void deCanvasWX::setPen(int r, int g, int b)
{
    wxPen pen(wxColour(r, g, b));

    dc.SetPen(pen);
}    

void deCanvasWX::drawBitmap(deBitmap& bitmap)
{
    deBitmapWX& bitmapWX = dynamic_cast<deBitmapWX&>(bitmap);

    wxBitmap* b = bitmapWX.getBitmap();
    if (b)
    {
        dc.DrawBitmap(*b, 0, 0, false);
    }            
}    

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

#include "bitmap_wx.h"

deBitmapWX::deBitmapWX()
{
    bitmap = NULL;
}

deBitmapWX::~deBitmapWX()
{
    if (bitmap)
    {
        delete bitmap;
    }
}

void deBitmapWX::resize(int w, int h)
{
    if (bitmap)
    {
        delete bitmap;
        bitmap = NULL;
    }

    bitmap = new wxBitmap(w, h, 24);
}


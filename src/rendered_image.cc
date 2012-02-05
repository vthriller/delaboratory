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

#include "rendered_image.h"
#include "logger.h"
#include "str.h"

void deRenderedImage::setSize(const deSize& _size)
{
    requestedSize = _size;
}

unsigned char* deRenderedImage::getCurrentImageData()
{
    if (requestedSize != size)
    {
        if (image)
        {
            logMessage("delete image");
            delete image;
        }            
        size = requestedSize;
        int w = size.getW();
        int h = size.getH();
        logMessage("create image " + str(w) + "x" + str(h));
        image = new wxImage(w,h);
    }

    unsigned char* data = image->GetData();

    return data;
}


deRenderedImage::deRenderedImage()
:size(0,0), requestedSize(0,0)
{
    image = NULL;
}

deRenderedImage::~deRenderedImage()
{
    if (image)
    {
        delete image;
    }        
}

bool deRenderedImage::render(wxDC& dc)
{
    if (image)
    {
        wxBitmap bitmap(*image);
        dc.DrawBitmap(bitmap, 0, 0, false);
        return true;
    }
    else
    {
        logMessage("renderer - no image");
        return false;
    }
}   

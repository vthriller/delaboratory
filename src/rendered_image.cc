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
#include <wx/rawbmp.h>
#include "bitmap_wx.h"
#include "canvas_wx.h"

void deRenderedImage::setSize(const deSize& _size)
{
    requestedSize = _size;
}

unsigned char* deRenderedImage::getCurrentImageData()
{
    if (requestedSize != size)
    {
        if (internalData)
        {
            delete [] internalData;
        }
        size = requestedSize;
        int w = size.getW();
        int h = size.getH();
        internalData = new unsigned char [ 3 * w * h ];
    }

    return internalData;
}


deRenderedImage::deRenderedImage()
:size(0,0), 
requestedSize(0,0),
bitmapSize(0,0)
{
    logMessage("create rendered image");
    renderedBitmap = new deBitmapWX();
    internalData = NULL;
}

deRenderedImage::~deRenderedImage()
{
    logMessage("destroy rendered image");
    if (renderedBitmap)
    {
        delete renderedBitmap;
    }            
    if (internalData)
    {
        delete [] internalData;
    }
}

bool deRenderedImage::render(deCanvas& canvas)
{
    if (!internalData)
    {
        logMessage("ERROR can't render - no internal data");
        return false;
    }

    int w = size.getW();
    int h = size.getH();

    if (bitmapSize != size)
    {
        renderedBitmap->resize(w, h);
        bitmapSize = size;
    }

    deBitmapWX* bitmapWX = dynamic_cast<deBitmapWX*>(renderedBitmap);

    wxNativePixelData bitmapData(*(bitmapWX->getBitmap()));
    if (!bitmapData)
    {
        logMessage("ERROR can't render - wxNativePixelData doesn't work");
        return false;
    }

    wxNativePixelData::Iterator p(bitmapData);

    p.Offset(bitmapData, 0, 0);
    
    int x;
    int y;
    int pos = 0;
    for (y = 0; y < h; y++)
    {
        wxNativePixelData::Iterator rowStart = p;

        for (x = 0; x < w; x++)
        {
            unsigned char r = internalData[pos];
            p.Red() = r;
            pos++;
            unsigned char g = internalData[pos];
            p.Green() = g;
            pos++;
            unsigned char b = internalData[pos];
            p.Blue() = b;
            pos++;
            p++;
        }            

        p = rowStart;
        p.OffsetY(bitmapData, 1);
    }

    canvas.drawBitmap(*renderedBitmap);
    return true;
}   

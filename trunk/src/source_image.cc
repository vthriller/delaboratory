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

#include "source_image.h"
#include <wx/wx.h>
#include "channel.h"
#include "channels.h"
#include "color_space.h"
#include <tiffio.h>

deSourceImage::deSourceImage()
:dePreview(deColorSpaceRGB)
{
}

void deSourceImage::load(const std::string& fileName)
{
    {
        size_t pos = fileName.rfind("tif");
        if ((pos == fileName.size() - 3) || (pos == fileName.size() - 4))
        {
            loadTIFF(fileName);
            return;
        }
    }
    {
        size_t pos = fileName.rfind("TIF");
        if ((pos == fileName.size() - 3) || (pos == fileName.size() - 4))
        {
            loadTIFF(fileName);
            return;
        }
    }
    loadJPEG(fileName);
}

void deSourceImage::loadTIFF(const std::string& fileName)
{
    TIFF* tif = TIFFOpen(fileName.c_str(), "r");
    if (!tif)
    {
        return;
    }
    tdata_t buf;

    int w;
    int h;
    uint16 bps;
    uint16 spp;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bps);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &spp);
    size = deSize(w,h);

    createChannels(size);

    deBaseChannel* channelR = getChannel(DE_CHANNEL_RED);
    deBaseChannel* channelG = getChannel(DE_CHANNEL_GREEN);
    deBaseChannel* channelB = getChannel(DE_CHANNEL_BLUE);

    int pos = 0;
    int y;

    int ssize = TIFFScanlineSize(tif);
    buf = _TIFFmalloc(ssize);

    for (y = 0; y < h; y++)
    {
        TIFFReadScanline(tif, buf, y);
        int x;
        for (x = 0; x < w; x++)
        {
            deValue r;
            deValue g;
            deValue b;
            if (bps == 16)
            {
                uint16* bb = (uint16*)(buf);
                uint16 u1 = bb[spp*x+0];
                uint16 u2 = bb[spp*x+1];
                uint16 u3 = bb[spp*x+2];
                deValue d = (256 * 256) - 1;
                r = u1 / d;
                g = u2 / d;
                b = u3 / d;
            }         
            else
            {
                uint8* bb = (uint8*)(buf);
                uint8 u1 = bb[spp*x+0];
                uint8 u2 = bb[spp*x+1];
                uint8 u3 = bb[spp*x+2];
                deValue d = 256 - 1;
                r = u1 / d;
                g = u2 / d;
                b = u3 / d;
            }
            channelR->setValue(pos, r );
            channelG->setValue(pos, g );
            channelB->setValue(pos, b );
            pos++;
        }
    }
    _TIFFfree(buf);
    TIFFClose(tif);
}

void deSourceImage::loadJPEG( const std::string& fileName)
{
    wxImage image;
    image.LoadFile(wxString::FromAscii(fileName.c_str()), wxBITMAP_TYPE_JPEG);
    int w = image.GetWidth();
    int h = image.GetHeight();

    size = deSize(w,h);

    createChannels(size);

    deBaseChannel* channelR = getChannel(DE_CHANNEL_RED);
    deBaseChannel* channelG = getChannel(DE_CHANNEL_GREEN);
    deBaseChannel* channelB = getChannel(DE_CHANNEL_BLUE);

    int pos = 0;

    int y;
    for (y =0; y < h; y++)
    {   
        int x;
        for (x = 0; x < w; x++)
        {
            deValue r = image.GetRed(x, y) / 255.0; 
            deValue g = image.GetGreen(x, y) / 255.0; 
            deValue b = image.GetBlue(x, y) / 255.0; 
            channelR->setValue(pos, r );
            channelG->setValue(pos, g );
            channelB->setValue(pos, b );
            pos++;
        }
    }

}

deSourceImage::~deSourceImage()
{
}

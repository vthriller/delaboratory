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

#include "final_image.h"
#include "preview.h"
#include "color_space.h"
#include "channels.h"
#include "channel.h"
#include "converter.h"
#include "tiffio.h"
#include <iostream>

deFinalImage::deFinalImage(const dePreview& preview)
:dePreview(deColorSpaceRGB, preview.getSize())
{
    deConverter converter;
    converter.setSource(&preview);
    converter.setDestination(this);
    converter.convert();
}

deFinalImage::~deFinalImage()
{
}

void deFinalImage::saveJPEG(const std::string& fileName)
{
    wxImage* image;
    int w = size.getW();
    int h = size.getH();

    const deBaseChannel* channelR = getChannel(DE_CHANNEL_RED);
    const deBaseChannel* channelG = getChannel(DE_CHANNEL_GREEN);
    const deBaseChannel* channelB = getChannel(DE_CHANNEL_BLUE);

    image = new wxImage(w, h);


    int pos = 0;
    int y;
    for (y = 0; y < h; y++)
    {
        int x;
        for (x = 0; x < w; x++)
        {
            deValue r = 255 * channelR->getValue(pos);
            deValue g = 255 * channelG->getValue(pos);
            deValue b = 255 * channelB->getValue(pos);
            image->SetRGB(x, y, r, g, b); 

            pos++;

        }
    }
    image->SaveFile(wxString::FromAscii(fileName.c_str()));
    delete image;
}

void deFinalImage::saveTIFF(const std::string& fileName)
{
    int w = size.getW();
    int h = size.getH();

    TIFF* tif = TIFFOpen(fileName.c_str(), "w");

    TIFFSetField (tif, TIFFTAG_SOFTWARE, "delaboratory");
    TIFFSetField (tif, TIFFTAG_IMAGEWIDTH, w);
    TIFFSetField (tif, TIFFTAG_IMAGELENGTH, h);
    TIFFSetField (tif, TIFFTAG_SAMPLESPERPIXEL, 3);
    TIFFSetField (tif, TIFFTAG_BITSPERSAMPLE, 16);
    TIFFSetField (tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    TIFFSetField (tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);
    TIFFSetField (tif, TIFFTAG_ROWSPERSTRIP, h);
    TIFFSetField (tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField (tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

    const deBaseChannel* channelR = getChannel(DE_CHANNEL_RED);
    const deBaseChannel* channelG = getChannel(DE_CHANNEL_GREEN);
    const deBaseChannel* channelB = getChannel(DE_CHANNEL_BLUE);

    tdata_t buf;
    int ssize = TIFFScanlineSize(tif);
    buf = _TIFFmalloc(ssize);
    uint16* bb = (uint16*)(buf);

    int pos = 0;
    int y;
    for (y = 0; y < h; y++)
    {
        int x;
        for (x = 0; x < w; x++)
        {
            deValue d = 255 * 255;
            deValue r = d * channelR->getValue(pos);
            deValue g = d * channelG->getValue(pos);
            deValue b = d * channelB->getValue(pos);
            bb[3*x+0] = r;
            bb[3*x+1] = g;
            bb[3*x+2] = b;;

            pos++;

        }
        TIFFWriteScanline (tif, buf, y, 0);
    }

    TIFFClose(tif);
}

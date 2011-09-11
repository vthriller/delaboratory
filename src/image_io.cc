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

#include "image_io.h"
#include <tiffio.h>
#include <wx/wx.h>
#include "channel.h"

bool checkTIFF(const std::string& fileName)
{
    wxLogNull noerrormessages;

    TIFF* tif = TIFFOpen(fileName.c_str(), "r");
    if (!tif)
    {
        return false;
    }
    else
    {
        TIFFClose(tif);
        return true;
    }
}    

bool checkJPEG(const std::string& fileName)
{
    wxLogNull noerrormessages;

    wxImage image;
    return image.LoadFile(wxString::FromAscii(fileName.c_str()), wxBITMAP_TYPE_JPEG);
}   

deSize getTIFFSize(const std::string& fileName)
{
    TIFF* tif = TIFFOpen(fileName.c_str(), "r");
    deSize size(0,0);

    if (tif)
    {
        int w;
        int h;
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
        size = deSize(w,h);
    }

    return size;
}    

deSize getJPEGSize( const std::string& fileName)
{
    wxImage image;
    deSize size(0,0);

    if (image.LoadFile(wxString::FromAscii(fileName.c_str()), wxBITMAP_TYPE_JPEG))
    {
        int w = image.GetWidth();
        int h = image.GetHeight();
        size = deSize(w,h);
    }

    return size;
}    

void loadTIFF(const std::string& fileName, deBaseChannel& channelR, deBaseChannel& channelG, deBaseChannel& channelB, bool& icc)
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

    uint32* count;
    void** profile;
    if (TIFFGetField(tif, TIFFTAG_ICCPROFILE, &count, &profile))
    {
        icc = true;
    }        
    else
    {
        icc = false;
    }

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
            channelR.setValue(pos, r );
            channelG.setValue(pos, g );
            channelB.setValue(pos, b );
            pos++;
        }
    }
    _TIFFfree(buf);
    TIFFClose(tif);
}

void loadJPEG(const std::string& fileName, deBaseChannel& channelR, deBaseChannel& channelG, deBaseChannel& channelB)
{
    wxImage image;
    image.LoadFile(wxString::FromAscii(fileName.c_str()), wxBITMAP_TYPE_JPEG);
    int w = image.GetWidth();
    int h = image.GetHeight();

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
            channelR.setValue(pos, r );
            channelG.setValue(pos, g );
            channelB.setValue(pos, b );
            pos++;
        }
    }

}

void saveJPEG(const std::string& fileName, const deBaseChannel& channelR, const deBaseChannel& channelG, const deBaseChannel& channelB, deSize& size)
{
    wxImage* image;
    int w = size.getW();
    int h = size.getH();

    image = new wxImage(w, h);

    int pos = 0;
    int y;
    for (y = 0; y < h; y++)
    {
        int x;
        for (x = 0; x < w; x++)
        {
            deValue r = 255 * channelR.getValue(pos);
            deValue g = 255 * channelG.getValue(pos);
            deValue b = 255 * channelB.getValue(pos);
            image->SetRGB(x, y, r, g, b); 

            pos++;

        }
    }
    image->SaveFile(wxString::FromAscii(fileName.c_str()));
    delete image;
}

void saveTIFF(const std::string& fileName, const deBaseChannel& channelR, const deBaseChannel& channelG, const deBaseChannel& channelB, deSize& size)
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
            deValue d = 256 * 256 - 1 ;;
            deValue r = d * channelR.getValue(pos);
            deValue g = d * channelG.getValue(pos);
            deValue b = d * channelB.getValue(pos);
            bb[3*x+0] = r;
            bb[3*x+1] = g;
            bb[3*x+2] = b;;

            pos++;

        }
        TIFFWriteScanline (tif, buf, y, 0);
    }

    TIFFClose(tif);
}

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

#include "tiff.h"
#include <tiffio.h>
#include "static_image.h"
#include <wx/wx.h>
#include "logger.h"

TIFF* openTIFF(const std::string fileName, bool write)
{
    std::string mode = "";
    if (write)
    {
        logInfo("opening TIFF " + fileName + " in write mode");
        mode = "w";
    }
    else
    {
        logInfo("opening TIFF " + fileName + " in read mode");
        mode = "r";
    }

    TIFF* tif = NULL;

    const char* c = fileName.c_str();

#ifdef _WIN32
    wxString ws(c, wxConvUTF8);
    const wchar_t* wc = ws.wc_str(wxConvUTF8);
    tif = TIFFOpenW(wc, mode.c_str());
#else
    tif = TIFFOpen(c, mode.c_str());
#endif    
    logInfo("DONE opening TIFF");

    return tif;
}    


bool loadTIFF(const std::string& fileName, deStaticImage& image)
{
    TIFF* tif = openTIFF(fileName, false);
    if (!tif)
    {
        return false;
    }
    tdata_t buf;

    image.lock();

    int w;
    int h;
    uint16 bps;
    uint16 spp;
    uint16 photometric;

    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bps);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &spp);
    TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric);

    deSize size(w, h);
    image.setSize(size);

    image.setColorSpace(deColorSpaceRGB);
    if (photometric != PHOTOMETRIC_RGB)
    {
        logInfo("loading not RGB image, let's use LAB");
        image.setColorSpace(deColorSpaceLAB);
    }

    deValue* pixelsR = image.startWriteStatic(0);
    deValue* pixelsG = image.startWriteStatic(1);
    deValue* pixelsB = image.startWriteStatic(2);

    int pos = 0;
    int y;

    int ssize = TIFFScanlineSize(tif);
    buf = _TIFFmalloc(ssize);

    deValue d = -1;
    if (bps == 16)
    {
        logInfo("bps 16");
        d = (256 * 256) - 1;
    }
    else
    {
        logInfo("bps not 16");
        d = 256 - 1;
    }

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
                r = u1 / d;
                g = u2 / d;
                b = u3 / d;
            }

            if (photometric == PHOTOMETRIC_RGB)
            {
                pixelsR[pos] = r;
                pixelsG[pos] = g;
                pixelsB[pos] = b;
            }
            else
            {
                // LAB
                pixelsR[pos] = r;
                g += 0.5;
                if (g > 1)
                {
                    g -= 1.0;
                }
                b += 0.5;
                if (b > 1)
                {
                    b -= 1.0;
                }
                pixelsG[pos] = g;
                pixelsB[pos] = b;

            }

            pos++;
        }
    }

    image.finishWriteStatic(0);
    image.finishWriteStatic(1);
    image.finishWriteStatic(2);
    image.unlock();

    _TIFFfree(buf);
    TIFFClose(tif);

    return true;
}

bool saveTIFF(const std::string& fileName, const deValue* channelR, const deValue* channelG, const deValue* channelB, deSize size)
{
    int w = size.getW();
    int h = size.getH();

    if ((!channelR) || (!channelG) || (!channelB))
    {
        return false;
    }

    TIFF* tif = openTIFF(fileName, true);

    if (!tif)
    {
        return false;
    }

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
            deValue d = 256 * 256 - 1 ;
            deValue r = d * channelR[pos];
            deValue g = d * channelG[pos];
            deValue b = d * channelB[pos];
            bb[3*x+0] = r;
            bb[3*x+1] = g;
            bb[3*x+2] = b;;

            pos++;

        }
        TIFFWriteScanline (tif, buf, y, 0);
    }

    TIFFClose(tif);

    return true;

}


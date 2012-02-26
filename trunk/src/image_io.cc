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
#include "conversion_functions.h"
#include "logger.h"
#include "static_image.h"
#include "str.h"
#include "dcraw_support.h"
#include "rgb2xyz2lab.h"
#include "conversion_functions.h"

void saveJPEG(const std::string& fileName, const deChannel& channelR, const deChannel& channelG, const deChannel& channelB, deSize size)
{

    wxImage* image;
    int w = size.getW();
    int h = size.getH();

    image = new wxImage(w, h);

    image->SetOption(wxIMAGE_OPTION_QUALITY,98);

    channelR.lockRead();
    channelG.lockRead();
    channelB.lockRead();

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

    channelR.unlockRead();
    channelG.unlockRead();
    channelB.unlockRead();

    const char* c = fileName.c_str();
    wxString s(c, wxConvUTF8);
    image->SaveFile(s);
    delete image;
}

void saveTIFF(const std::string& fileName, const deChannel& channelR, const deChannel& channelG, const deChannel& channelB, deSize size)
{
    logMessage("save TIFF " + fileName);
    int w = size.getW();
    int h = size.getH();

    TIFF* tif = TIFFOpen(fileName.c_str(), "w");

    if (!tif)
    {
        logMessage("ERROR writing " + fileName);
        return;
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

    channelR.lockRead();
    channelG.lockRead();
    channelB.lockRead();

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

    channelR.unlockRead();
    channelG.unlockRead();
    channelB.unlockRead();

    TIFFClose(tif);

    logMessage("saved TIFF " + fileName);
}

bool loadJPEG(const std::string& fileName, deStaticImage& image)
{
    wxLogNull noerrormessages;

    logMessage("loadJPEG " + fileName);

    const char* c = fileName.c_str();
    wxString s(c, wxConvUTF8);
    wxImage fileImage;
    bool result = fileImage.LoadFile(s, wxBITMAP_TYPE_JPEG);
    if (!result)
    {
        return false;
    }
    int w = fileImage.GetWidth();
    int h = fileImage.GetHeight();

    deSize size(w, h);
    image.setSize(size);

    deChannel* channelRR = image.getChannel(0);
    if (!channelRR)
    {
        return false;
    }
    deChannel& channelR = *channelRR;

    deChannel* channelGG = image.getChannel(1);
    if (!channelGG)
    {
        return false;
    }
    deChannel& channelG = *channelGG;

    deChannel* channelBB = image.getChannel(2);
    if (!channelBB)
    {
        return false;
    }
    deChannel& channelB = *channelBB;

    int pos = 0;

    image.setColorSpace(deColorSpaceRGB);

    channelR.lockWrite();
    channelG.lockWrite();
    channelB.lockWrite();

    unsigned char* data = fileImage.GetData();

    int p = 0;
    int y;
    for (y =0; y < h; y++)
    {   
        int x;
        for (x = 0; x < w; x++)
        {

            deValue r = data[p] / 255.0; 
            p++;
            deValue g = data[p] / 255.0; 
            p++;
            deValue b = data[p] / 255.0; 
            p++;
            channelR.setValue(pos, r );
            channelG.setValue(pos, g );
            channelB.setValue(pos, b );
            pos++;
        }
    }

    channelR.unlockWrite();
    channelG.unlockWrite();
    channelB.unlockWrite();

    logMessage("loadJPEG " + fileName + " done");

    return true;
}



bool loadTIFF(const std::string& fileName, deStaticImage& image)
{
    wxLogNull noerrormessages;

    logMessage("load TIFF " + fileName);

    TIFF* tif = TIFFOpen(fileName.c_str(), "r");
    if (!tif)
    {
        return false;
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

    logMessage("found TIFF " + str(w) + "x" + str(h) + " bps: " + str(bps) + " spp: " + str(spp));

    deSize size(w, h);
    image.setSize(size);

    deChannel* channelRR = image.getChannel(0);
    if (!channelRR)
    {
        return false;
    }
    deChannel& channelR = *channelRR;

    deChannel* channelGG = image.getChannel(1);
    if (!channelGG)
    {
        return false;
    }
    deChannel& channelG = *channelGG;

    deChannel* channelBB = image.getChannel(2);
    if (!channelBB)
    {
        return false;
    }
    deChannel& channelB = *channelBB;

    image.setColorSpace(deColorSpaceRGB);

    channelR.lockWrite();
    channelG.lockWrite();
    channelB.lockWrite();

    deValue* pixelsR = channelR.getPixels();
    deValue* pixelsG = channelG.getPixels();
    deValue* pixelsB = channelB.getPixels();

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

            pixelsR[pos] = r;
            pixelsG[pos] = g;
            pixelsB[pos] = b;

            pos++;
        }
    }

    channelR.unlockWrite();
    channelG.unlockWrite();
    channelB.unlockWrite();

    _TIFFfree(buf);
    TIFFClose(tif);
    logMessage("loadTIFF " + fileName + " done");

    return true;
}


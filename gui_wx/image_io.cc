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
#include "logger.h"
#include "static_image.h"
#include "str.h"
#include "dcraw_support.h"
#include "channel_manager.h"
#include "image.h"
#include "conversion_processor.h"
#include "tiff.h"

/*
void saveJPEG(const std::string& fileName, const deChannel& channelR, const deChannel& channelG, const deChannel& channelB, deSize size)
{
    wxImage* image;
    int w = size.getW();
    int h = size.getH();

    image = new wxImage(w, h);

    image->SetOption(wxIMAGE_OPTION_QUALITY,98);

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

    const char* c = fileName.c_str();
    wxString s(c, wxConvUTF8);
    image->SaveFile(s);
    delete image;
}
    */

bool loadJPEG(const std::string& fileName, deStaticImage& image)
{

    logInfo("loadJPEG " + fileName);

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

    image.lock();

    deSize size(w, h);
    image.setSize(size);
    image.setColorSpace(deColorSpaceRGB);

    deValue* pixels0 = image.startWriteStatic(0);
    deValue* pixels1 = image.startWriteStatic(1);
    deValue* pixels2 = image.startWriteStatic(2);

    int pos = 0;

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
            pixels0[pos] = r;
            pixels1[pos] = g;
            pixels2[pos] = b;
            pos++;
        }
    }

    logInfo("loadJPEG " + fileName + " done");

    image.finishWriteStatic(0);
    image.finishWriteStatic(1);
    image.finishWriteStatic(2);

    image.unlock();
    return true;
}

bool saveImage(const std::string& fileName, const deImage& image, const std::string& type, deChannelManager& previewChannelManager)
{
    bool result = false;

    if (image.getColorSpace() == deColorSpaceRGB)
    {
        if (type == "tiff")
        {
            const deValue* vr = image.startRead(0);
            const deValue* vg = image.startRead(1);
            const deValue* vb = image.startRead(2);
            result = saveTIFF(fileName, vr, vg, vb, image.getChannelSize());
            image.finishRead(0);
            image.finishRead(1);
            image.finishRead(2);
        }            
        if (type == "jpeg")
        {
        /*
            deChannel* r = previewChannelManager.getChannel(image.getChannelIndex(0));
            deChannel* g = previewChannelManager.getChannel(image.getChannelIndex(1));
            deChannel* b = previewChannelManager.getChannel(image.getChannelIndex(2));
            saveJPEG(fileName, *r, *g, *b, image.getChannelSize());
            */
        }            
    }
    else
    {
        deImage finalImage(deColorSpaceRGB, previewChannelManager);
        finalImage.allocateChannels();

        deConversionProcessor p;
        p.convertImageNew(image, finalImage);

/*
        deChannel* r = previewChannelManager.getChannel(finalImage.getChannelIndex(0));
        deChannel* g = previewChannelManager.getChannel(finalImage.getChannelIndex(1));
        deChannel* b = previewChannelManager.getChannel(finalImage.getChannelIndex(2));
        */
        if (type == "tiff")
        {
            const deValue* vr = finalImage.startRead(0);
            const deValue* vg = finalImage.startRead(1);
            const deValue* vb = finalImage.startRead(2);
            result = saveTIFF(fileName, vr, vg, vb, image.getChannelSize());
            finalImage.finishRead(0);
            finalImage.finishRead(1);
            finalImage.finishRead(2);
        }            
        if (type == "jpeg")
        {
            //saveJPEG(fileName, *r, *g, *b, image.getChannelSize());
        }            
    }

    return result;
}

bool loadImage(const std::string& fileName, deStaticImage& image, deColorSpace colorSpace)
{
    wxLogNull noerrormessages;

    if (colorSpace != deColorSpaceRGB)
    {
        return false;
    }

    if (loadTIFF(fileName, image))
    {
        return true;
    }
    if (loadJPEG(fileName, image))
    {
        return true;
    }
    return false;
}


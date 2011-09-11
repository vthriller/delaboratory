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
#include "image_io.h"
#include <cassert>

deSourceImage::deSourceImage()
:dePreview(deColorSpaceRGB)
{
}

void deSourceImage::load(const std::string& fileName)
{
    bool tiff = checkTIFF(fileName);
    bool jpeg = checkJPEG(fileName);

    if ((!tiff) && (!jpeg))
    {
        return;
    }

    if ((tiff) && (jpeg))
    {
        // you got to be kidding
        assert(false);
    }

    if (tiff)
    {
        size = getTIFFSize(fileName);
    }

    if (jpeg)
    {
        size = getJPEGSize(fileName);
    }

    createChannels(size);

    deBaseChannel* channelR = getChannel(DE_CHANNEL_RED);
    deBaseChannel* channelG = getChannel(DE_CHANNEL_GREEN);
    deBaseChannel* channelB = getChannel(DE_CHANNEL_BLUE);

    if (tiff)
    {
        bool icc;
        loadTIFF(fileName, *channelR, *channelG, *channelB, icc);
        if (icc)
        {
            wxMessageBox( _T("Warning! this TIFF file contains ICC profile which is ignored by delaboratory\n\ndelaboratory expects sRGB - colors may be not accurate\n\nThis problem happens (for instance) when tiff is created by dcraw\nyou can fix it by calling tifficc command, by default it converts tiff to sRGB"), _T("ICC profile ignored"), wxOK | wxICON_INFORMATION, NULL );
        }
    }

    if (jpeg)
    {
        loadJPEG(fileName, *channelR, *channelG, *channelB);
    }



}

deSourceImage::~deSourceImage()
{
}

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

#include "dcraw_support.h"
#include <wx/wx.h>
#include <wx/progdlg.h>
#include <wx/process.h>
#include "logger.h"
#include "str.h"
#include "channel.h"
#include "size.h"
#include "static_image.h"
#include "rgb2xyz2lab.h"

#define DCRAW_EXECUTABLE "dcraw"

std::string getDcrawVersion()
{
    const char* com = DCRAW_EXECUTABLE;
    wxString s(com, wxConvUTF8);

    wxProcess* process = wxProcess::Open(s);

    wxInputStream* input = process->GetInputStream();

    std::string vs = "";

    int counter = 0;

    char c = ' ';
    char c2 = ' ';

    do
    {
        c2 = c;
        c = input->GetC();
        counter++;
        if (counter > 40)
        {
            return "";
        }
    }
    while ((c2 != ' ') || (c != 'v'));

    do
    {
        c = input->GetC();
        if (c != '\n')
        {
            vs += c;
        }
    }
    while (c != '\n');

    return vs;
}

deRawLoader::deRawLoader(const std::string& f, deStaticImage& _image, deColorSpace _colorSpace, bool _half)
:filename(f), image(_image), colorSpace(_colorSpace), half(_half)
{
    std::string options = "-w -c -6 -o 4 -W";
    if (half)
    {
        options += " -h";
    }
    else
    {
        options += " -q 3";
    }

    std::string command = std::string(DCRAW_EXECUTABLE) + " " + options + " \"" + f + "\" >abc ";
    logMessage("calling: " + command);

    const char* c = command.c_str();
    wxString s(c, wxConvUTF8);

    process = wxProcess::Open(s);

    input = process->GetInputStream();
}

deRawLoader::~deRawLoader()
{
}


bool deRawLoader::load(bool& failure)
{
    if (!input->CanRead())
    {
        return false;
    }

    logMessage("deRawLoader::load() - CanRead");

    char c1 = input->GetC();
    char c2 = input->GetC();

    if (c1 != 'P') 
    {
        logMessage("first character not P");
        failure = true;
        return false;
    }
    if (c2 != '6') 
    {
        logMessage("second character not 6");
        failure = true;
        return false;
    }

    logMessage("P6 loaded");

    char c;

    std::string ws = "";
    do
    {
        c = input->GetC();
        if (c != ' ')
        ws += c;
    }
    while (c != ' ');

    int w = getInt(ws);
    if (w <= 0)
    {
        failure = true;
        return false;
    }

    std::string hs = "";
    do
    {
        c = input->GetC();
        if (c != '\n')
        hs += c;
    }
    while (c != '\n');

    int h = getInt(hs);
    if (h <= 0)
    {
        failure = true;
        return false;
    }

    std::string ms = "";
    do
    {
        c = input->GetC();
        if (c != '\n')
        ms += c;
    }
    while (c != '\n');


    int max = getInt(ms);
    if (max <= 256)
    {
        failure = true;
        return false;
    }

    logMessage("w: " + str(w) + " h: " + str(h) + " max: " + str(max));

    if (half)
    {
        w *= 2;
        h *= 2;
        logMessage("after half w: " + str(w) + " h: " + str(h) + " max: " + str(max));
    }

    image.lock();


    deSize size(w, h);
    image.setSize(size);

    deChannel* channelRR = image.getChannel(0);
    if (!channelRR)
    {
        image.unlock();
        failure = true;
        return false;
    }
    deChannel& channelR = *channelRR;

    deChannel* channelGG = image.getChannel(1);
    if (!channelGG)
    {
        image.unlock();
        failure = true;
        return false;
    }
    deChannel& channelG = *channelGG;

    deChannel* channelBB = image.getChannel(2);
    if (!channelBB)
    {
        image.unlock();
        failure = true;
        return false;
    }
    deChannel& channelB = *channelBB;

    channelR.lockWrite();
    channelG.lockWrite();
    channelB.lockWrite();

    deValue* pixels0 = channelR.getPixels();
    deValue* pixels1 = channelG.getPixels();
    deValue* pixels2 = channelB.getPixels();

    deValue scale = 1.0 / max;

    int pos = 0;

    unsigned char cc1;
    unsigned char cc2;

    char* buffer;

    int bufsize=500000;

    logMessage("allocating buffer of size " + str(bufsize));

    buffer = new char[bufsize];

    logMessage("buffer allocated");

    bool error = false;

    int n = w * h;

    if (half)
    {
        n /= 4;
    }

    int offset = 0;

    int steps = 0;

    int maxRead = 0;

    image.setColorSpace(colorSpace);

    int tx = 0;
    int ty = 0;

    logMessage("start loading PPM data");

    while ((pos < n) && (!input->Eof()))
    {
        steps++;

        input->Read(buffer + offset, bufsize - offset);

        int r = input->LastRead();
        if (r > maxRead)
        {
            maxRead = r;
        }

        r += offset;

        int p = 0;
        while (p + 6 <= r)
        {
            deValue r;
            deValue g;
            deValue b;

            c = buffer[p];
            p++;
            cc1 = (unsigned char)(c);
            c = buffer[p];
            p++;
            cc2 = (unsigned char)(c);
            r = (256 * cc1 + cc2) * scale;

            c = buffer[p];
            p++;
            cc1 = (unsigned char)(c);
            c = buffer[p];
            p++;
            cc2 = (unsigned char)(c);
            g = (256 * cc1 + cc2) * scale;

            c = buffer[p];
            p++;
            cc1 = (unsigned char)(c);
            c = buffer[p];
            p++;
            cc2 = (unsigned char)(c);
            b = (256 * cc1 + cc2) * scale;

            if (half)
            {
                pixels0[(2 * ty + 0) * w + 2 * tx + 0] = r;
                pixels0[(2 * ty + 1) * w + 2 * tx + 0] = r;
                pixels0[(2 * ty + 0) * w + 2 * tx + 1] = r;
                pixels0[(2 * ty + 1) * w + 2 * tx + 1] = r;

                pixels1[(2 * ty + 0) * w + 2 * tx + 0] = g;
                pixels1[(2 * ty + 1) * w + 2 * tx + 0] = g;
                pixels1[(2 * ty + 0) * w + 2 * tx + 1] = g;
                pixels1[(2 * ty + 1) * w + 2 * tx + 1] = g;

                pixels2[(2 * ty + 0) * w + 2 * tx + 0] = b;
                pixels2[(2 * ty + 1) * w + 2 * tx + 0] = b;
                pixels2[(2 * ty + 0) * w + 2 * tx + 1] = b;
                pixels2[(2 * ty + 1) * w + 2 * tx + 1] = b;

                tx++;
                if (tx >= w / 2)
                {
                    tx = 0;
                    ty++;
                }
            }
            else
            {
                pixels0[pos] = r;
                pixels1[pos] = g;
                pixels2[pos] = b;
            }

            pos++;
        }

        int left = r - p;
        int i;
        for (i = 0; i < left; i++)
        {
            buffer[i] = buffer[p + i];
        }
        offset = left;

    }

    logMessage("pos: " + str(pos) + " n: " + str(n) + " steps: " + str(steps) + " maxRead: " + str(maxRead));
    if (input->Eof())
    {
        logMessage("input stream EOF");
    }

    logMessage("deallocating buffer");
    delete [] buffer;

    channelR.unlockWrite();
    channelG.unlockWrite();
    channelB.unlockWrite();

    image.unlock();
    
    if (error)
    {
        failure = true;
        return false;
    }

    logMessage("loading ppm done");
    return true;
}

bool deRawLoader::getStatus()
{
    if (!process)
    {
        return false;
    }
    if (!input)
    {
        return false;
    }
    return true;
}

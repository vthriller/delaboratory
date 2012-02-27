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
    const wxString command = wxString::FromAscii(DCRAW_EXECUTABLE);

    wxProcess* process = wxProcess::Open(command);

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

bool execDcrawProcess(const std::string& f, deStaticImage& image, deColorSpace colorSpace)
{
    wxProgressDialog* progressDialog = new wxProgressDialog(_T("dcraw processing"), _T("dcraw processing"), 100, NULL, wxPD_AUTO_HIDE);

    std::string command = std::string(DCRAW_EXECUTABLE) + " -w -c -6 -o 4 -W " + f + " >abc ";
    logMessage("calling: " + command);

    wxProcess* process = wxProcess::Open(wxString::FromAscii(command.c_str()));

    wxInputStream* input = process->GetInputStream();

    delete progressDialog;

    char c1 = input->GetC();
    char c2 = input->GetC();

    if (c1 != 'P') 
    {
        return false;
    }
    if (c2 != '6') 
    {
        return false;
    }

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
        return false;
    }


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

    bool error = false;

    int n = w * h;

    int offset = 0;

    int steps = 0;

    int maxRead = 0;

    bool lab = false;

    if (colorSpace == deColorSpaceLAB)
    {
        lab = true;
    }

    image.setColorSpace(colorSpace);

    while ((pos < n) && (!input->Eof()))
    {
        steps++;

//        logMessage("pos: " + str(pos) + " n: " + str(n) + " offset: " + str(offset));
        input->Read(buffer + offset, bufsize - offset);

        int r = input->LastRead();
        if (r > maxRead)
        {
            maxRead = r;
        }

//        logMessage("read " + str(r) + " bytes into buffer");
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

            if (lab)
            {
                deValue v1;
                deValue v2;
                deValue v3;
                deValue vv1;
                deValue vv2;
                deValue vv3;
                prophoto2xyz(r, g, b, vv1, vv2, vv3);
                xyz2lab(vv1, vv2, vv3, v1, v2, v3);

                pixels0[pos] = v1;
                pixels1[pos] = v2;
                pixels2[pos] = v3;
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
    
    if (error)
    {
        return false;
    }

    logMessage("loading ppm done");
    return true;
}

/*
bool execDcraw(const std::string& f, const std::string& tmp)
{
    wxProgressDialog* progressDialog = new wxProgressDialog(_T("dcraw processing"), _T("dcraw processing"), 100, NULL, wxPD_AUTO_HIDE);

    std::string c = std::string(DCRAW_EXECUTABLE) + " -w -c -6 -o 4 -W " + f + " >" + tmp;

    delete progressDialog;

    logMessage("calling: " + c);

    system(c.c_str());

    return true;
}
*/

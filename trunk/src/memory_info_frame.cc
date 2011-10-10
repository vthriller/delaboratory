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

#include "memory_info_frame.h"
#include "project.h"
#include <sstream>

int generateInfo(deChannelManager& channelManager, wxWindow* parent, wxSizer* sizer, wxString name)
{
    wxSizer* sizerB = new wxStaticBoxSizer(wxVERTICAL, parent,  name);
    sizer->Add(sizerB);

    int size = sizeof(deValue) * channelManager.getChannelSize().getN();
    int n = channelManager.getNumberOfAllocatedChannels();
    int m = n * size / (1024 * 1024);

    {
        std::ostringstream oss;
        oss << "channel size: " << size;
        wxStaticText* l1 = new wxStaticText(parent, wxID_ANY, wxString::FromAscii(oss.str().c_str()));
        sizerB->Add(l1);
    }
    {
        std::ostringstream oss;
        oss << "allocated channels: " << n;
        wxStaticText* l1 = new wxStaticText(parent, wxID_ANY, wxString::FromAscii(oss.str().c_str()));
        sizerB->Add(l1);
    }
    {
        std::ostringstream oss;
        oss << "memory usage: " << m << " MB";
        wxStaticText* l1 = new wxStaticText(parent, wxID_ANY, wxString::FromAscii(oss.str().c_str()));
        sizerB->Add(l1);
    }

    return m;
}

deMemoryInfoFrame::deMemoryInfoFrame(wxWindow *parent, deProject& _project)
:deHelpFrame(parent), project(_project)
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    int m = 0;

    { 
        deChannelManager& channelManager = project.getPreviewChannelManager();
        m+= generateInfo(channelManager, this, sizer, _T("preview channels"));
    }        

    { 
        deChannelManager& channelManager = project.getSourceChannelManager();
        m+= generateInfo(channelManager, this, sizer, _T("source channels"));
    }        

    {
        std::ostringstream oss;
        oss << "total memory usage: " << m << " MB";
        wxStaticText* l1 = new wxStaticText(this, wxID_ANY, wxString::FromAscii(oss.str().c_str()));
        sizer->Add(l1);
    }

    /*

    std::vector<deColorSpace> colorSpaces;
    getSupportedColorSpaces(colorSpaces);

    int width = 256;
    int barSize = 40;

    std::vector<deColorSpace>::iterator i;
    for (i = colorSpaces.begin(); i != colorSpaces.end(); i++)
    {
        deColorSpace c = *i;

        wxSizer* sizerB = new wxStaticBoxSizer(wxHORIZONTAL, this,  wxString::FromAscii(getColorSpaceName(c).c_str()));
        sizer->Add(sizerB);

        int n = getColorSpaceSize(c);
        int j;
        for (j = 0; j < n; j++)
        {
            wxSizer* sizerBC= new wxStaticBoxSizer(wxHORIZONTAL, this,  wxString::FromAscii(getChannelName(c, j).c_str()));
            sizerB->Add(sizerBC);

            deGradientPanel* gradient = new deGradientPanel(this, wxSize(width, barSize), c, j, -1);
            sizerBC->Add(gradient, 0, wxCENTER);
        }
    }*/

    Fit();

}

deMemoryInfoFrame::~deMemoryInfoFrame()
{
}


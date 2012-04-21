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
#include "str.h"
#include "channel_manager.h"

int generateInfo(deChannelManager& channelManager, wxWindow* parent, wxSizer* sizer, wxString name)
{
    wxSizer* sizerB = new wxStaticBoxSizer(wxVERTICAL, parent,  name);
    sizer->Add(sizerB);

    int size = sizeof(deValue) * channelManager.getChannelSizeFromChannelManager().getN();
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

void deMemoryInfoFrame::update()
{
    int m1;
    {
        deChannelManager& channelManager = project.getPreviewChannelManager();
        int size = sizeof(deValue) * channelManager.getChannelSizeFromChannelManager().getN();
        int n = channelManager.getNumberOfAllocatedChannels();
        m1 = n * size / (1024 * 1024);

        previewChannels->SetLabel(wxString::FromAscii(str(n).c_str()));
        previewSize->SetLabel(wxString::FromAscii(str(size).c_str()));
        std::string s = str(m1) + "MB";
        previewMemory->SetLabel(wxString::FromAscii(s.c_str()));
    }        

    Layout();
    Fit();
}

deMemoryInfoFrame::deMemoryInfoFrame(wxWindow *parent, deProject& _project)
:deHelpFrame(parent, "memory info"), project(_project)
{
    wxSizer* sizerB = new wxStaticBoxSizer(wxVERTICAL, this,  _T("memory info"));
    SetSizer(sizerB);
    wxSizer* sizer = new wxFlexGridSizer(2);
    sizerB->Add(sizer);

    {
        wxStaticText* l = new wxStaticText(this, wxID_ANY, _T("allocated preview channels:"));
        sizer->Add(l);
        previewChannels = new wxStaticText(this, wxID_ANY, _T(""));
        sizer->Add(previewChannels);
    }        
    {
        wxStaticText* l = new wxStaticText(this, wxID_ANY, _T("preview channel size:"));
        sizer->Add(l);
        previewSize = new wxStaticText(this, wxID_ANY, _T(""));
        sizer->Add(previewSize);
    }        
    {
        wxStaticText* l = new wxStaticText(this, wxID_ANY, _T("memory used by preview channels:"));
        sizer->Add(l);
        previewMemory = new wxStaticText(this, wxID_ANY, _T(""));
        sizer->Add(previewMemory);
    }   

    update();


}

deMemoryInfoFrame::~deMemoryInfoFrame()
{
    project.closeMemoryInfoFrame();
}


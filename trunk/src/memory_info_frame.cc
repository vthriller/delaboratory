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

void deMemoryInfoFrame::update()
{
    int m1;
    int m2;
    {
        deChannelManager& channelManager = project.getPreviewChannelManager();
        int size = sizeof(deValue) * channelManager.getChannelSize().getN();
        int n = channelManager.getNumberOfAllocatedChannels();
        m1 = n * size / (1024 * 1024);

        previewChannels->SetLabel(wxString::FromAscii(str(n).c_str()));
        previewSize->SetLabel(wxString::FromAscii(str(size).c_str()));
        std::string s = str(m1) + "MB";
        previewMemory->SetLabel(wxString::FromAscii(s.c_str()));
    }        
    /*
    {
        deChannelManager& channelManager = project.getSourceChannelManager();
        int size = sizeof(deValue) * channelManager.getChannelSize().getN();
        int n = channelManager.getNumberOfAllocatedChannels();
        m2 = n * size / (1024 * 1024);

        sourceChannels->SetLabel(wxString::FromAscii(str(n).c_str()));
        sourceSize->SetLabel(wxString::FromAscii(str(size).c_str()));
        std::string s = str(m2) + "MB";
        sourceMemory->SetLabel(wxString::FromAscii(s.c_str()));
    }        

    int m = m1 + m2;
    std::string s = str(m) + "MB";
    totalMemory->SetLabel(wxString::FromAscii(s.c_str()));
    */

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
    /*
    {
        wxStaticText* l = new wxStaticText(this, wxID_ANY, _T("allocated source channels:"));
        sizer->Add(l);
        sourceChannels = new wxStaticText(this, wxID_ANY, _T(""));
        sizer->Add(sourceChannels);
    }        
    {
        wxStaticText* l = new wxStaticText(this, wxID_ANY, _T("source channel size:"));
        sizer->Add(l);
        sourceSize = new wxStaticText(this, wxID_ANY, _T(""));
        sizer->Add(sourceSize);
    }        
    {
        wxStaticText* l = new wxStaticText(this, wxID_ANY, _T("memory used by source channels:"));
        sizer->Add(l);
        sourceMemory = new wxStaticText(this, wxID_ANY, _T(""));
        sizer->Add(sourceMemory);
    }        
    {
        wxStaticText* l = new wxStaticText(this, wxID_ANY, _T("total memory used:"));
        sizer->Add(l);
        totalMemory = new wxStaticText(this, wxID_ANY, _T(""));
        sizer->Add(totalMemory);
    } 
    */

    update();


}

deMemoryInfoFrame::~deMemoryInfoFrame()
{
    project.closeMemoryInfoFrame();
}


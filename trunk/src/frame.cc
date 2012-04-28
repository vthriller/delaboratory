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

#include "frame.h"
#include "window_wx.h"
#include "panel_wx.h"

deFrameOld::deFrameOld(wxWindow *parent, const std::string& name)
:wxFrame(parent, wxID_ANY, wxString::FromAscii(name.c_str()), wxDefaultPosition, wxDefaultSize, wxCAPTION | wxCLOSE_BOX | wxFRAME_FLOAT_ON_PARENT)
{
}

deFrameOld::~deFrameOld()
{
}

class deFrameImpl:public wxFrame
{
    private:
        wxSizer* sizer;
        deWindowWX window;
        deFrame* frame;
    public:
        deFrameImpl(deWindow& parent, const std::string& name, deFrame* _frame)
        :wxFrame((dynamic_cast<deWindowWX&>(parent)).getWindow(), wxID_ANY, wxString::FromAscii(name.c_str()), wxDefaultPosition, wxDefaultSize, wxCAPTION | wxCLOSE_BOX | wxFRAME_FLOAT_ON_PARENT), window(this), frame(_frame)
        {
            sizer = new wxBoxSizer(wxVERTICAL);
            SetSizer(sizer);
        }

        virtual ~deFrameImpl()
        {
            frame->clearImpl();
            delete frame;
        }

        void addWidget(wxWindow* widget)
        {
            sizer->Add(widget);
        }

        deWindow& getWindow()
        {
            return window;
        }

        void fit()
        {
            Fit();
        }
};

deFrame::deFrame(deWindow& parent, const std::string& name)
{
    impl = new deFrameImpl(parent, name, this);
}

deFrame::~deFrame()
{
    if (impl)
    {
        delete impl;
    }        
}

void deFrame::clearImpl()
{
    impl = NULL;
}

void deFrame::show()
{
    if (impl)
    {
        impl->Show();
    }
}

void deFrame::addWidget(deWindow& window)
{
    deWindowWX& w = dynamic_cast<deWindowWX&>(window);
    if (impl)
    {
        impl->addWidget(w.getWindow());
    }
}

deWindow& deFrame::getWindow()
{
    return impl->getWindow();
}

void deFrame::fit()
{
    impl->fit();
}

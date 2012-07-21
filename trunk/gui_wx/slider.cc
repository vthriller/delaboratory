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

#include "slider.h"
#include <sstream>
#include "layer_processor.h"
#include "panel_wx.h"
#include "window_wx.h"
#include "logger.h"
#include "str.h"

class deSliderImpl:public dePanelWX
{
    private:
        deSlider& parent;
        wxSlider* slider;
        wxStaticText* labelValue;
        deValue min;
        deValue max;
        deValue width;
    public:
        deSliderImpl(deSlider& _parent, deWindow& _parentWindow, const std::string& _name, deValue _min, deValue _max, int _width, int widthn, int widthl)
        :dePanelWX(_parentWindow), parent(_parent), min(_min), max(_max), width(_width)
        {
            if (width == 0)
            {
                logError("slider with width 0 created");
            }
            wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
            SetSizer(sizer);

            if (_name.size() > 0)
            {
                wxStaticText* label = new wxStaticText(this, wxID_ANY, wxString::FromAscii(_name.c_str()), wxDefaultPosition, wxSize(widthn, 30));
                sizer->Add(label, 0, wxCENTER);
            }

            slider = new wxSlider(this, wxID_ANY, _width, 0,  _width, wxDefaultPosition, wxSize(_width, -1), wxSL_HORIZONTAL);
            sizer->Add(slider, 0);

            labelValue = new wxStaticText(this, wxID_ANY, _T("inv"), wxDefaultPosition, wxSize(widthl, 30));
            sizer->Add(labelValue, 0, wxCENTER);

            Connect(wxEVT_SCROLL_THUMBTRACK, wxCommandEventHandler(deSliderImpl::moveSlider));
            Connect(wxEVT_SCROLL_CHANGED, wxCommandEventHandler(deSliderImpl::finishMoveSlider));
        }

        virtual ~deSliderImpl()
        {
#ifdef DEBUG_LOG
            logInfo("~deSliderImpl");
#endif            
        }

        void moveSlider(wxCommandEvent &event)
        {
            updateValueFromSlider(false);
        }

        void finishMoveSlider(wxCommandEvent &event)
        {
            updateValueFromSlider(true);
        }

        void updateValueFromSlider(bool finished)
        {
            deValue v = min + slider->GetValue() * ((max - min) / width);
            setLabelValue(v);
            parent.onValueChange(v, finished);
        }

        void setLabelValue(deValue v)
        {
            std::ostringstream oss;
            oss.str();
            oss << v;
            if (labelValue)
            {
                std::string s = oss.str();
                labelValue->SetLabel(wxString::FromAscii(oss.str().c_str()));
            }
        }

        void setValue(deValue v)
        {
            setLabelValue(v);
            setSlider(v);
        }

        void setSlider(deValue v)
        {
            if (width == 0)
            {
                return;
            }

            if (max == min)
            {
                return;
            }

            deValue sl = (v - min) / ((max-min) / width);

            slider->SetValue(sl);
        }

};

deSlider::deSlider(deWindow& window, const std::string& _name, deValue _min, deValue _max, int _width, int widthn, int widthl)
{
#ifdef DEBUG_LOG
    logInfo("deSlider constructor");
#endif    
    deWindowWX* w = dynamic_cast<deWindowWX*>(&window);
    if (w)
    {
        impl = new deSliderImpl(*this, window, _name, _min, _max, _width, widthn, widthl);
    }
    else
    {
        impl = NULL;
    }
}

deSlider::~deSlider()
{
#ifdef DEBUG_LOG
    logInfo("deSlider destructor");
#endif    
    if (impl)
    {
        delete impl;
    }        
}

void deSlider::setValue(deValue v)
{
    if (impl)
    {
        impl->setValue(v);
    }        
}

deWindow& deSlider::getWindow()
{
    return impl->getWindow();
}

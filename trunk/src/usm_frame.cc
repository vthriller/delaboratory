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

#include "usm_frame.h"
#include "usm_layer.h"
#include <iostream>


class deBlurRadiusSlider3:public deSlider
{
    private:
        deUSMLayer& layer;

    public:
        deBlurRadiusSlider3(wxWindow *parent, int range, deUSMLayer& _layer)
        :deSlider(parent, "blur radius", range, 0.0, 1.0, 0.0), layer(_layer)
        {
            set();
        }

        virtual ~deBlurRadiusSlider3()
        {
        }

        virtual void onValueChange(deValue value, bool finished)
        {
            if (finished)
            {
                layer.setBlurRadius(value);
            }                
        }

        void set()
        {
            setValue(layer.getBlurRadius());
        }
};        

class deUSMAmountSlider3:public deSlider
{
    private:
        deUSMLayer& layer;

    public:
        deUSMAmountSlider3(wxWindow *parent, int range, deUSMLayer& _layer)
        :deSlider(parent, "amount", range, 0.0, 5, 0.0), layer(_layer)
        {
            set();
        }
        
        void set()
        {
            setValue(layer.getAmount());
        }

        virtual ~deUSMAmountSlider3()
        {
        }

        virtual void onValueChange(deValue value, bool finished)
        {
            if (finished)
            {
                layer.setAmount(value);
            }                
        }
};        

class deUSMThresholdSlider3:public deSlider
{
    private:
        deUSMLayer& layer;

    public:
        deUSMThresholdSlider3(wxWindow *parent, int range, deUSMLayer& _layer)
        :deSlider(parent, "threshold", range, 0.0, 0.5, 0.0), layer(_layer)
        {
            set();
        }

        void set()
        {
            setValue(layer.getThreshold());
        }

        virtual ~deUSMThresholdSlider3()
        {
        }

        virtual void onValueChange(deValue value, bool finished)
        {
            if (finished)
            {
                layer.setThreshold(value);
            }                
        }
};        

deUSMFrame::deUSMFrame(wxWindow *parent, deActionLayer& _layer)
:deActionFrame(parent, _layer)
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    deUSMLayer& usmLayer = dynamic_cast<deUSMLayer&>(_layer);

    int range = 400;

    radius = new deBlurRadiusSlider3(this, range, usmLayer);
    sizer->Add(radius);

    amount = new deUSMAmountSlider3(this, range, usmLayer);
    sizer->Add(amount);

    threshold = new deUSMThresholdSlider3(this, range, usmLayer);
    sizer->Add(threshold);

    wxSizer* sizerB = new wxStaticBoxSizer(wxHORIZONTAL, this, _T(""));
    sizer->Add(sizerB, 0);

    reset = new wxButton(this, wxID_ANY, _T("reset"), wxDefaultPosition, wxSize(100,25));
    sizerB->Add(reset, 0);

    sharp = new wxButton(this, wxID_ANY, _T("sharp"), wxDefaultPosition, wxSize(100,25));
    sizerB->Add(sharp, 0);

    hiraloam1 = new wxButton(this, wxID_ANY, _T("hiraloam 1"), wxDefaultPosition, wxSize(100,25));
    sizerB->Add(hiraloam1, 0);

    hiraloam2 = new wxButton(this, wxID_ANY, _T("hiraloam 2"), wxDefaultPosition, wxSize(100,25));
    sizerB->Add(hiraloam2, 0);

    Fit();

    Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deUSMFrame::click));
}

deUSMFrame::~deUSMFrame()
{
}

void deUSMFrame::click(wxCommandEvent &event)
{
    int id = event.GetId();
    deUSMLayer& usmLayer = dynamic_cast<deUSMLayer&>(layer);

    if (reset->GetId() == id)
    {
        usmLayer.reset();
    }      

    if (sharp->GetId() == id)
    {
        usmLayer.sharp();
    }      

    if (hiraloam1->GetId() == id)
    {
        usmLayer.hiraloam1();
    }      

    if (hiraloam2->GetId() == id)
    {
        usmLayer.hiraloam2();
    }      

    dynamic_cast<deBlurRadiusSlider3*>(radius)->set();
    dynamic_cast<deUSMAmountSlider3*>(amount)->set();
    dynamic_cast<deUSMThresholdSlider3*>(threshold)->set();
}   

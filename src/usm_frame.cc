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
        :deSlider(parent, "blur radius", range, 0.0, 0.02, 0.0), layer(_layer)
        {
            setValue(layer.getBlurRadius());
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
};        

class deUSMAmountSlider3:public deSlider
{
    private:
        deUSMLayer& layer;

    public:
        deUSMAmountSlider3(wxWindow *parent, int range, deUSMLayer& _layer)
        :deSlider(parent, "amount", range, 0.0, 10, 0.0), layer(_layer)
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

    radius = new deBlurRadiusSlider3(this, 200, usmLayer);
    sizer->Add(radius);

    amount = new deUSMAmountSlider3(this, 200, usmLayer);
    sizer->Add(amount);

    threshold = new deUSMThresholdSlider3(this, 200, usmLayer);
    sizer->Add(threshold);


    Fit();
}

deUSMFrame::~deUSMFrame()
{
}


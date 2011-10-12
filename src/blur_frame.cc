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

#include "blur_frame.h"
#include "blur_layer.h"
#include <iostream>

class deBlurRadiusSlider2:public deSlider
{
    private:
        deBlurLayer& layer;

    public:
        deBlurRadiusSlider2(wxWindow *parent, int range, deBlurLayer& _layer)
        :deSlider(parent, "blur radius", range, 0.0, 0.05, 0.0), layer(_layer)
        {
            setValue(layer.getBlurRadius());
        }

        virtual ~deBlurRadiusSlider2()
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

deBlurFrame::deBlurFrame(wxWindow *parent, deActionLayer& _layer)
:deActionFrame(parent, _layer)
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    deBlurLayer& blurLayer = dynamic_cast<deBlurLayer&>(_layer);

    radius = new deBlurRadiusSlider2(this, 100, blurLayer);
    sizer->Add(radius);

    Fit();
}

deBlurFrame::~deBlurFrame()
{
}


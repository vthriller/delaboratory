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
#include "property_value_slider.h"
#include "property_choice_ui.h"

deBlurFrame::deBlurFrame(wxWindow *parent, deActionLayer& _layer)
:deActionFrame(parent, _layer)
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    deBlurLayer& blurLayer = dynamic_cast<deBlurLayer&>(_layer);

    int range = 100;

    radius = new dePropertyValueSlider(this, range, blurLayer.getPropertyRadius(), blurLayer);
    sizer->Add(radius);

    threshold = new dePropertyValueSlider(this, range, blurLayer.getPropertyThreshold(), blurLayer);
    sizer->Add(threshold);

    blurType = new dePropertyChoiceUI(this, blurLayer.getPropertyType(), blurLayer);
    sizer->Add(blurType);

    onUpdateProperties();

    Fit();

}

deBlurFrame::~deBlurFrame()
{
}

void deBlurFrame::onUpdateProperties()
{
    deBlurLayer& blurLayer = dynamic_cast<deBlurLayer&>(layer);
    if (blurLayer.getPropertyType().get() == "surface")
    {
        threshold->Enable();
    }
    else
    {
        threshold->Disable();
    }
}
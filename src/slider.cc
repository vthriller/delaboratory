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

void deSlider::updateValueFromSlider(bool finished)
{
    deValue v = valueMin + slider->GetValue() * ((valueMax-valueMin) / sliderRange);
    if (integerMode)
    {
        int i = v;
        v = i;
    }        
    setEdit(v);
    onValueChange(v, finished);
}

void deSlider::setValue(deValue v)
{
    setEdit(v);
    setSlider(v);
}

void deSlider::setEdit(deValue v)
{
    std::ostringstream oss;
    oss.str();
    oss << v;
    if (labelValue)
    {
        std::string s = oss.str();
        labelValue->SetLabel(wxString::FromAscii(oss.str().c_str()));
    }
    else
    {   
        layerProcessor.log("ERROR no labelValue in deSlider");
    }
}

void deSlider::setSlider(deValue v)
{
    if (sliderRange == 0)
    {
        layerProcessor.log("ERROR slider range is 0 !!!");
        return;
    }

    if (valueMax == valueMin)
    {
        layerProcessor.log("ERROR valueMax == valueMin !!!");
        return;
    }

    deValue sl = (v - valueMin) / ((valueMax-valueMin) / sliderRange);

    if (!slider)
    {
        layerProcessor.log("ERROR no slider in deSlider");
        return;
    }

    slider->SetValue(sl);
}

void deSlider::moveSlider(wxCommandEvent &event)
{
    updateValueFromSlider(false);
}

void deSlider::finishMoveSlider(wxCommandEvent &event)
{
    updateValueFromSlider(true);
}

deSlider::deSlider(wxWindow *parent, const std::string& labelString, int _sliderRange, deValue _valueMin, deValue _valueMax, deValue defaultValue, deLayerProcessor& _layerProcessor)
:wxPanel(parent), sliderRange(_sliderRange), valueMin(_valueMin), valueMax(_valueMax), layerProcessor(_layerProcessor)
{
    integerMode = false;
    sizer = new wxBoxSizer(wxHORIZONTAL);

    label = new wxStaticText(this, wxID_ANY, wxString::FromAscii(labelString.c_str()), wxDefaultPosition, wxSize(80, 30));
    sizer->Add(label, 0, wxCENTER);

    slider = new wxSlider(this, wxID_ANY, sliderRange, 0,  sliderRange, wxDefaultPosition, wxSize(sliderRange, -1), wxSL_HORIZONTAL);
    sizer->Add(slider, 0);

    labelValue = new wxStaticText(this, wxID_ANY, _T("inv"), wxDefaultPosition, wxSize(40, 30));
    sizer->Add(labelValue, 0, wxCENTER);


    SetSizer(sizer);

    Connect(wxEVT_SCROLL_THUMBTRACK, wxCommandEventHandler(deSlider::moveSlider));
    Connect(wxEVT_SCROLL_CHANGED, wxCommandEventHandler(deSlider::finishMoveSlider));

}

deSlider::~deSlider()
{
}

void deSlider::setIntegerMode()
{
    integerMode = true;
}

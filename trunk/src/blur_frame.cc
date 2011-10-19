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
        :deSlider(parent, "radius", range, 0.0, 0.05, 0.0), layer(_layer)
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

class deBlurThresholdSlider2:public deSlider
{
    private:
        deBlurLayer& layer;

    public:
        deBlurThresholdSlider2(wxWindow *parent, int range, deBlurLayer& _layer)
        :deSlider(parent, "threshold", range, 0.0, 1.0, 0.0), layer(_layer)
        {
            setValue(layer.getBlurThreshold());
        }

        virtual ~deBlurThresholdSlider2()
        {
        }

        virtual void onValueChange(deValue value, bool finished)
        {
            if (finished)
            {
                layer.setBlurThreshold(value);
            }                
        }
};        

deBlurFrame::deBlurFrame(wxWindow *parent, deActionLayer& _layer)
:deActionFrame(parent, _layer)
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    deBlurLayer& blurLayer = dynamic_cast<deBlurLayer&>(_layer);

    {
        getSupportedBlurTypes(blurTypes);

        deBlurType currentBlurType = blurLayer.getBlurType();

        wxString* blurTypeStrings = new wxString [blurTypes.size()];
        unsigned int i;
        int selectedBlurType;
        for (i = 0; i < blurTypes.size(); i++)
        {
            blurTypeStrings[i] = wxString::FromAscii(getBlurTypeName(blurTypes[i]).c_str());
            if (currentBlurType == blurTypes[i])
            {
                selectedBlurType = i;
            }
        }        

        choice =  new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(200, -1), blurTypes.size(), blurTypeStrings);
        choice->SetSelection(selectedBlurType);

        sizer->Add(choice);
    }

    radius = new deBlurRadiusSlider2(this, 100, blurLayer);
    sizer->Add(radius);

    threshold = new deBlurThresholdSlider2(this, 100, blurLayer);
    sizer->Add(threshold);

    Fit();

    Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(deBlurFrame::choose));
}

deBlurFrame::~deBlurFrame()
{
}

void deBlurFrame::choose(wxCommandEvent &event)
{
    deBlurLayer& l = dynamic_cast<deBlurLayer&>(layer);

    int i = event.GetInt();
    deBlurType& type = blurTypes[i];
    l.setBlurType(type);
}

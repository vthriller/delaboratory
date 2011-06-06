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

#include "nd_frame.h"
#include "nd_layer.h"
#include "slider.h"
#include "preview_stack.h"
#include "nd_type_choice.h"

class deNDSlider:public deSlider
{
    private:
        deND& nd;
        dePreviewStack& stack;
        int layerNumber;
        bool isY;
        bool isPower;
    public:
        deNDSlider(wxWindow *parent, const std::string& labelString, int _sliderRange, deValue _valueMin, deValue _valueMax, deND& _nd, dePreviewStack& _stack, int _layerNumber, bool _isY, bool _isPower)
        :deSlider(parent, labelString, _sliderRange, _valueMin, _valueMax), nd(_nd), stack(_stack), layerNumber(_layerNumber), isY(_isY), isPower(_isPower)
        {
            deValue v;
            if (isY)
            {
                v = nd.getYCenter();
            }
            else
            if (isPower)
            {
                v = nd.getPower();
            }
            else
            {
                v = nd.getXCenter();
            }
            setValue(v);
        }

        virtual ~deNDSlider()
        {
        }
        
        virtual void onValueChange(deValue value)
        {
            if (isY)
            {
                nd.setYCenter(value);
            }
            else
            if (isPower)
            {
                nd.setPower(value);
            }
            else
            {
                nd.setXCenter(value);
            }
            stack.updatePreviews(layerNumber);
            stack.refreshView();
        }            
};


deNDFrame::deNDFrame(wxWindow* parent, deNDLayer& _layer, dePreviewStack& _stack, int _layerNumber)
:deActionFrame(parent, _layer, _T("blur")),
 layer(_layer), stack(_stack), layerNumber(_layerNumber)
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    int sliderRange = 200;

    deValue min = -1.0;
    deValue max = 1.0;

    deNDType currentNDType = layer.getND().getType();
    ndTypeChoice = makeNDTypeChoice(this, ndTypes, currentNDType);
    sizer->Add(ndTypeChoice, 0);

    xCenterSlider = new deNDSlider(this, "x center", sliderRange, min, max, layer.getND(), stack, layerNumber, false, false);
    sizer->Add(xCenterSlider, 0);
    yCenterSlider = new deNDSlider(this, "y center", sliderRange, min, max, layer.getND(), stack, layerNumber, true, false);
    sizer->Add(yCenterSlider, 0);
    powerSlider = new deNDSlider(this, "power", sliderRange, min, 3*max, layer.getND(), stack, layerNumber, false, true);
    sizer->Add(powerSlider, 0);

    SetSizer(sizer);

    Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(deNDFrame::choose));

}

deNDFrame::~deNDFrame()
{
}

void deNDFrame::choose(wxCommandEvent &event)
{
    int t = ndTypeChoice->GetCurrentSelection();
    layer.getND().setType(ndTypes[t]);
    stack.updatePreviews(layerNumber);
    stack.refreshView();
}

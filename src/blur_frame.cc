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
#include "slider.h"
#include "preview_stack.h"

class deBlurSlider:public deSlider
{
    private:
        deBlurLayer& layer;
        dePreviewStack& stack;
        int layerNumber;
    public:
        deBlurSlider(wxWindow *parent, const std::string& labelString, int _sliderRange, deValue _valueMin, deValue _valueMax, deBlurLayer& _layer, dePreviewStack& _stack, int _layerNumber)
        :deSlider(parent, labelString, _sliderRange, _valueMin, _valueMax), layer(_layer), stack(_stack), layerNumber(_layerNumber)
        {
            deValue v = layer.getBlurRadius();
            setValue(v);
        }

        virtual ~deBlurSlider()
        {
        }
        
        virtual void onValueChange(deValue value)
        {
            layer.setBlurRadius(value);
            stack.updatePreviews(layerNumber);
            stack.refreshView();
        }            
};


deBlurFrame::deBlurFrame(wxWindow* parent, deBlurLayer& _layer, dePreviewStack& _stack, int _layerNumber)
:deActionFrame(parent, _layer, _T("blur")),
 layer(_layer), stack(_stack), layerNumber(_layerNumber)
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    int sliderRange = 200;

    blurSlider = new deBlurSlider(this, "blur", sliderRange, 0.0, 0.1, layer, stack, layerNumber);
    sizer->Add(blurSlider, 0);

    wxStaticText* labelBlurDirection = new wxStaticText(this, wxID_ANY, _T("blur direction:") );
    sizer->Add(labelBlurDirection);

    deBlurDirection currentBlurDirection = layer.getBlurDirection();

    wxString blurDirectionStrings[2];
    blurDirectionStrings[0] = _T("horizontal");
    blurDirectionStrings[1] = _T("vertical");

    blurDirectionChoice =  new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(200, -1), 2, blurDirectionStrings);
    if (currentBlurDirection == deBlurHorizontal)
    {
        blurDirectionChoice->SetSelection(0);
    }
    else
    {
        blurDirectionChoice->SetSelection(1);
    }

    sizer->Add(blurDirectionChoice, 0);

    deColorSpace colorSpace = layer.getColorSpace();
    int n = getColorSpaceSize(colorSpace);
    int i;
    channels.reserve(n);
    for (i = 0; i < n; i++)
    {
        wxCheckBox* cb = new wxCheckBox(this, wxID_ANY, wxString::FromAscii(getChannelName(colorSpace, i).c_str()));
        cb->SetValue(layer.isChannelEnabled(i));
        sizer->Add(cb, 0);
        channels[i] = cb;
    }

    SetSizer(sizer);

    Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(deBlurFrame::choose));
    Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(deBlurFrame::check));
}

deBlurFrame::~deBlurFrame()
{
}

void deBlurFrame::choose(wxCommandEvent &event)
{
    deBlurDirection direction;
    int c = blurDirectionChoice->GetSelection();
    if (c == 0)
    {
        direction = deBlurHorizontal;
    }
    else
    {
        direction = deBlurVertical;
    }
    layer.setBlurDirection(direction);
    stack.updatePreviews(layerNumber);
    stack.refreshView();
}

void deBlurFrame::check(wxCommandEvent &event)
{
    layer.clearEnabledChannels();
    deColorSpace colorSpace = layer.getColorSpace();
    int n = getColorSpaceSize(colorSpace);
    int i;
    for (i = 0; i < n; i++)
    {
        if (channels[i]->IsChecked())
        {
            layer.enableChannel(i);
        }
    }
    stack.updatePreviews(layerNumber);
    stack.refreshView();

}

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

#include "blend_frame.h"
#include "slider.h"
#include "blend_layer.h"
#include "preview_stack.h"
#include "preview.h"
#include "blend_mode.h"
#include "blend_mode_choice.h"
#include "channel_choice.h"

class deAlphaSlider:public deSlider
{
    private:
        deBlendLayer& layer;
        dePreviewStack& stack;
        int layerNumber;
    public:
        deAlphaSlider(wxWindow *parent, const std::string& labelString, int _sliderRange, deValue _valueMin, deValue _valueMax, deBlendLayer& _layer, dePreviewStack& _stack, int _layerNumber)
        :deSlider(parent, labelString, _sliderRange, _valueMin, _valueMax), layer(_layer), stack(_stack), layerNumber(_layerNumber)
        {
            deValue v = layer.getAlpha();
            setValue(v);
        }

        virtual ~deAlphaSlider()
        {
        }
        
        virtual void onValueChange(deValue value)
        {
            layer.changeAlpha(value);
            stack.updatePreviews(layerNumber);
            stack.refreshView();
        }            
};

deBlendFrame::deBlendFrame(wxWindow* parent, deBlendLayer& _layer, dePreviewStack& _stack, int _layerNumber)
:deActionFrame(parent, _layer, _T("blend")),
 layer(_layer), stack(_stack), layerNumber(_layerNumber)
{
    // FIXME constant
    int sliderRange = 200;

    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    alphaSlider = new deAlphaSlider(this, "alpha", sliderRange, 0.0, 1.0, layer, stack, layerNumber);
    sizer->Add(alphaSlider, 0);

    wxStaticText* labelBlendMode = new wxStaticText(this, wxID_ANY, _T("blend mode:") );
    sizer->Add(labelBlendMode);

    deBlendMode currentBlendMode = layer.getBlendMode();
    blendModeChoice = makeBlendModeChoice(this, blendModes, currentBlendMode);
    sizer->Add(blendModeChoice, 0);

    singleOverlayChannelCheckBox = new wxCheckBox(this, wxID_ANY, _T("single overlay channel"));
    sizer->Add(singleOverlayChannelCheckBox, 0);

    dePreview* overlayPreview = stack.getPreview(layer.getOverlayLayerID());
    deColorSpace overlayColorSpace = overlayPreview->getColorSpace();
    deColorSpace layerColorSpace = layer.getColorSpace();

    overlayChannelChoice = makeChannelChoice(this, overlayColorSpace);
    sizer->Add(overlayChannelChoice, 0);

    singleDestinationChannelCheckBox = new wxCheckBox(this, wxID_ANY, _T("single destination channel"));
    sizer->Add(singleDestinationChannelCheckBox, 0);

    destinationChannelChoice = makeChannelChoice(this, layerColorSpace);
    sizer->Add(destinationChannelChoice, 0);

    SetSizer(sizer);

    Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(deBlendFrame::choose));
    Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(deBlendFrame::check));

    update();
}

deBlendFrame::~deBlendFrame()
{
}

void deBlendFrame::choose(wxCommandEvent &event)
{
    updateSelections();
    int b = blendModeChoice->GetCurrentSelection();
    layer.setBlendMode(blendModes[b]);
    stack.updatePreviews(layerNumber);
    stack.refreshView();
}

void deBlendFrame::check(wxCommandEvent &event)
{
    layer.setSingleOverlayChannel(singleOverlayChannelCheckBox->IsChecked());
    layer.setSingleDestinationChannel(singleDestinationChannelCheckBox->IsChecked());
    updateSelections();
    update();
    stack.updatePreviews(layerNumber);
    stack.refreshView();
}

void deBlendFrame::updateSelections()
{
    int oc = overlayChannelChoice->GetCurrentSelection();
    layer.setOverlayChannel(oc);
    int dc = destinationChannelChoice->GetCurrentSelection();
    layer.setDestinationChannel(dc);
}

void deBlendFrame::update()
{
    if (layer.isSingleOverlayChannel())
    {
        overlayChannelChoice->Enable();
    }
    else
    {
        overlayChannelChoice->Disable();
    }
    if (layer.isSingleDestinationChannel())
    {
        destinationChannelChoice->Enable();
    }
    else
    {
        destinationChannelChoice->Disable();
    }
}

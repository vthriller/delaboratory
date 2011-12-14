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
#include "layer.h"
#include "action_layer.h"
#include "view_manager.h"
#include "layer_processor.h"

class deAlphaSlider:public deSlider
{
    private:
        deActionLayer& layer;
        deLayerProcessor& layerProcessor;

    public:
        deAlphaSlider(wxWindow *parent, int range, deActionLayer& _layer, deLayerProcessor& _layerProcessor)
        :deSlider(parent, "opacity", range, 0.0, 1.0, 1.0), layer(_layer),
        layerProcessor(_layerProcessor)
        {
            setValue(layer.getOpacity());
        }

        virtual ~deAlphaSlider()
        {
        }

        virtual void onValueChange(deValue value, bool finished)
        {
            if (finished)
            {
                layer.setOpacity(value);

                int index = layer.getIndex();
                layerProcessor.markUpdateBlendAllChannels(index);
            }                
        }
};        

class deBlurRadiusSlider:public deSlider
{
    private:
        deActionLayer& layer;
        deLayerProcessor& layerProcessor;

    public:
        deBlurRadiusSlider(wxWindow *parent, int range, deActionLayer& _layer, deLayerProcessor& _layerProcessor)
        :deSlider(parent, "blur radius", range, 0.0, 0.2, 0.0), layer(_layer),
         layerProcessor(_layerProcessor)
        {
            setValue(layer.getBlendBlurRadius());
        }

        virtual ~deBlurRadiusSlider()
        {
        }

        virtual void onValueChange(deValue value, bool finished)
        {
            if (finished)
            {
                layer.setBlendBlurRadius(value);

                int index = layer.getIndex();
                layerProcessor.markUpdateBlendAllChannels(index);
            }                
        }
};        

class deBlendMaskMinSlider:public deSlider
{
    private:
        deActionLayer& layer;
        deLayerProcessor& layerProcessor;

    public:
        deBlendMaskMinSlider(wxWindow *parent, int range, deActionLayer& _layer, deLayerProcessor& _layerProcessor)
        :deSlider(parent, "min", range, 0.0, 1.0, 0.0), layer(_layer),
        layerProcessor(_layerProcessor)
        {
            setValue(layer.getBlendMaskMin());
        }

        virtual ~deBlendMaskMinSlider()
        {
        }

        virtual void onValueChange(deValue value, bool finished)
        {
            if (finished)
            {
                layer.setBlendMaskMin(value);

                int index = layer.getIndex();
                layerProcessor.markUpdateBlendAllChannels(index);
            }                
        }
};        

class deBlendMaskMaxSlider:public deSlider
{
    private:
        deActionLayer& layer;
        deLayerProcessor& layerProcessor;

    public:
        deBlendMaskMaxSlider(wxWindow *parent, int range, deActionLayer& _layer, deLayerProcessor& _layerProcessor)
        :deSlider(parent, "max", range, 0.0, 1.0, 0.0), layer(_layer),
        layerProcessor(_layerProcessor)
        {
            setValue(layer.getBlendMaskMax());
        }

        virtual ~deBlendMaskMaxSlider()
        {
        }

        virtual void onValueChange(deValue value, bool finished)
        {
            if (finished)
            {
                layer.setBlendMaskMax(value);

                int index = layer.getIndex();
                layerProcessor.markUpdateBlendAllChannels(index);
            }                
        }
};        

deBlendFrame::deBlendFrame(wxWindow *parent, deActionLayer& _layer, deLayerProcessor& _layerProcessor)
:deLayerFrame(parent, _layer, _layer.getName()),
 layerProcessor(_layerProcessor)
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    deColorSpace colorSpace = layer.getColorSpace();
    if (colorSpace == deColorSpaceRGB)
    {
        applyAllowed = true;
    }
    else
    {
        applyAllowed = false;
    }

    layer.setBlendFrame(this);

    getSupportedBlendModes(blendModes);

    deBlendMode currentBlendMode = layer.getBlendMode();

    wxString* blendModeStrings = new wxString [blendModes.size()];
    unsigned int i;
    int selectedBlendMode = 0;
    for (i = 0; i < blendModes.size(); i++)
    {
        blendModeStrings[i] = wxString::FromAscii(getBlendModeName(blendModes[i]).c_str());
        if (currentBlendMode == blendModes[i])
        {
            selectedBlendMode = i;
        }
    }        

    choice =  new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(200, -1), blendModes.size(), blendModeStrings);
    choice->SetSelection(selectedBlendMode);

    delete [] blendModeStrings;

    sizer->Add(choice, 0);

    alphaSlider = new deAlphaSlider(this, 100, layer, layerProcessor);
    sizer->Add(alphaSlider, 0);

    wxSizer* sizerLC = new wxStaticBoxSizer(wxVERTICAL, this,  _T("luminance / color"));
    sizer->Add(sizerLC, 0);

    b1 = new wxRadioButton(this, wxID_ANY, _T("apply luminance and color"));
    sizerLC->Add(b1, 0);

    if (applyAllowed)
    {
        b2 = new wxRadioButton(this, wxID_ANY, _T("apply luminance"));
        sizerLC->Add(b2, 0);
        b3 = new wxRadioButton(this, wxID_ANY, _T("apply color"));
        sizerLC->Add(b3, 0);
        deApplyMode applyMode = layer.getApplyMode();
        if (applyMode == deApplyLuminance)
        {
            b2->SetValue(1);
        }
        if (applyMode == deApplyColor)
        {
            b3->SetValue(1);
        }
    }
    else
    {
        b2 = NULL;
        b3 = NULL;
    }

    wxSizer* sizerC = new wxStaticBoxSizer(wxVERTICAL, this,  _T("channels"));
    sizer->Add(sizerC, 0);

    unsigned int nc = getColorSpaceSize(colorSpace);

    for (i = 0; i < nc; i++)
    {
        wxCheckBox* cb = new wxCheckBox(this, wxID_ANY, wxString::FromAscii(getChannelName(colorSpace, i).c_str()));
        cb->SetValue(layer.isChannelEnabled(i));
        sizerC->Add(cb, 0);
        channels.push_back(cb);
    }

    wxSizer* sizerM = new wxStaticBoxSizer(wxVERTICAL, this,  _T("mask"));
    sizer->Add(sizerM, 0);

    wxSizer* sizerMC = new wxBoxSizer(wxHORIZONTAL);
    sizerM->Add(sizerMC);

    maskEnable = new wxCheckBox(this, wxID_ANY, _T("enable"));
    sizerMC->Add(maskEnable, 0);
    if (layer.isBlendMaskEnabled())
    {
        maskEnable->SetValue(1);
    }

    maskShow = new wxCheckBox(this, wxID_ANY, _T("show"));
    sizerMC->Add(maskShow, 0);

    deViewManager& viewManager = layer.getViewManager();

    if ((layer.isBlendMaskVisible()) && (viewManager.maskVisible()))
    {
        maskShow->SetValue(1);
    }

    unsigned int n = layer.getIndex();

    wxString* layerStrings = new wxString [n];
    for (i = 0; i < n; i++)
    {
        layerStrings[i] = wxString::Format(_T("%i"), i);
    }        

    maskLayerChoice =  new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(200, -1), n, layerStrings);

    delete [] layerStrings;

    maskLayerChoice->SetSelection(layer.getBlendMaskLayer());

    sizerM->Add(maskLayerChoice, 0);

    unsigned int s = layer.getBlendMaskChannel();

    for (i = 0; i < 4; i++)
    {
        long style = 0;
        if (i == 0)
        {
            style = wxRB_GROUP;
        }
        wxRadioButton* b = new wxRadioButton(this, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, style);
        sizerM->Add(b, 0);
        maskChannels[i] = b;
        if (i == s)
        {
            maskChannels[i]->SetValue(1);
        }
    }

    blurRadiusSlider = new deBlurRadiusSlider(this, 100, layer, layerProcessor);
    sizerM->Add(blurRadiusSlider, 0);

    blendMaskMinSlider = new deBlendMaskMinSlider(this, 100, layer, layerProcessor);
    sizerM->Add(blendMaskMinSlider, 0);

    blendMaskMaxSlider = new deBlendMaskMaxSlider(this, 100, layer, layerProcessor);
    sizerM->Add(blendMaskMaxSlider, 0);

    setChannels();

    showHide();

    Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(deBlendFrame::choose));
    Connect(wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(deBlendFrame::select));
    Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(deBlendFrame::check));

    Fit();
}

deBlendFrame::~deBlendFrame()
{
    deActionLayer& l = dynamic_cast<deActionLayer&>(layer);
    deViewManager& viewManager = layer.getViewManager();
    viewManager.hideThisMask(l.getAllocatedBlendMaskChannel());
    l.hideBlendMask();
    layer.closeBlendFrame();

    layerProcessor.repaintImageInLayerProcessor(true);
}

void deBlendFrame::choose(wxCommandEvent &event)
{
    deActionLayer& l = dynamic_cast<deActionLayer&>(layer);

    int id = event.GetId();
    if (id == choice->GetId())
    {
        int i = event.GetInt();
        deBlendMode& mode = blendModes[i];
        l.setBlendMode(mode);
        int index = layer.getIndex();
        layerProcessor.markUpdateBlendAllChannels(index);
    }
    else
    {
        updateMask();
    }
}

void deBlendFrame::check(wxCommandEvent &event)
{
    int id = event.GetId();
    deColorSpace colorSpace = layer.getColorSpace();
    int n = getColorSpaceSize(colorSpace);
    int i;
    for (i = 0; i < n; i++)
    {
        if (channels[i]->GetId() == id)
        {
            if (channels[i]->IsChecked())
            {
                layer.enableChannel(i);
            }
            else
            {
                layer.disableChannel(i);
            }

            int index = layer.getIndex();
            layerProcessor.markUpdateBlendAllChannels(index);

            return;
        }
    }

    updateMask();
}

void deBlendFrame::select(wxCommandEvent &event)
{
    deActionLayer& l = dynamic_cast<deActionLayer&>(layer);
    int i = event.GetId();

    if (applyAllowed)
    {
        if (i == b1->GetId())
        {
            l.setApplyMode(deApplyLuminanceAndColor);

            int index = layer.getIndex();
            layerProcessor.markUpdateBlendAllChannels(index);

            return;
        }            
        else if (i == b2->GetId())
        {
            l.setApplyMode(deApplyLuminance);

            int index = layer.getIndex();
            layerProcessor.markUpdateBlendAllChannels(index);

            return;
        }            
        else if (i == b3->GetId())
        {
            l.setApplyMode(deApplyColor);

            int index = layer.getIndex();
            layerProcessor.markUpdateBlendAllChannels(index);

            return;
        }
    }

    updateMask();

}

void deBlendFrame::setChannels()
{
    deActionLayer& l = dynamic_cast<deActionLayer&>(layer);
    deColorSpace colorSpace = l.getBlendMaskLayerColorSpace();
    int n = getColorSpaceSize(colorSpace);
    int i;
    for (i = 0; i < 4; i++)
    {
        if (i < n)
        {
            std::string name = getChannelName(colorSpace, i);
            maskChannels[i]->SetLabel(wxString::FromAscii(name.c_str()));
            maskChannels[i]->Show();
        }
        else
        {
            maskChannels[i]->Hide();
        }
    }
    Layout();
}

int deBlendFrame::getBlendChannel()
{
    int i;
    for (i = 0; i < 4; i++)
    {
        if (maskChannels[i]->GetValue() == 1)
        {
            return i;
        }
    }
    assert(false);
    return -1;
}

void deBlendFrame::updateMask()
{
    deActionLayer& l = dynamic_cast<deActionLayer&>(layer);
    deViewManager& viewManager = l.getViewManager();

    int layer = maskLayerChoice->GetSelection();
    int channel = getBlendChannel();

    if (maskShow->IsChecked())
    {
        l.setBlendMask(layer, channel);
        l.showBlendMask();
    }
    else
    {
        l.hideBlendMask();
    }

    if (maskEnable->IsChecked())
    {
        l.setBlendMask(layer, channel);
    }
    else
    {
        l.disableBlendMask();
    }

    l.onBlendSet();

    if (maskShow->IsChecked())
    {
        viewManager.showMask(l.getAllocatedBlendMaskChannel());
    }
    else
    {
        viewManager.hideMask();
    }

    setChannels();
    showHide();

    int index = l.getIndex();
    layerProcessor.markUpdateBlendAllChannels(index);
}

void deBlendFrame::showHide()
{
    int i;
    if (layer.isBlendMaskEnabled())
    {
        maskLayerChoice->Enable();
        for (i = 0; i < 4; i++)
        {
            maskChannels[i]->Enable();
        }
        blurRadiusSlider->Enable();
        blendMaskMinSlider->Enable();
        blendMaskMaxSlider->Enable();
    }
    else
    {
        maskLayerChoice->Disable();
        for (i = 0; i < 4; i++)
        {
            maskChannels[i]->Disable();
        }
        blurRadiusSlider->Disable();
        blendMaskMinSlider->Disable();
        blendMaskMaxSlider->Disable();
    }
}

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

#include "mixer_bw_editor.h"
#include "conversion_bw_layer.h"
#include "property_value_slider.h"
#include "layer_processor.h"
#include "layer_frame_manager.h"

deMixerBWEditor::deMixerBWEditor(wxWindow *parent, deConversionBWLayer& _layer, deLayerProcessor& _layerProcessor, deLayerFrameManager& _frameManager)
:deLayerFrame(parent, _layer, "conversion BW", _frameManager), layer( _layer), layerProcessor(_layerProcessor)
{
    frameManager.addActionFrame(this);

    deColorSpace sourceColorSpace = layer.getSourceColorSpace();

    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    int range = 300;

    wxSizer* sizerM = new wxStaticBoxSizer(wxVERTICAL, this,  _T("monochrome mixer"));
    sizer->Add(sizerM);

    add0 = new dePropertyValueSlider(this, range, layer.getAdd0(), layer, layerProcessor);
    sizerM->Add(add0);

    add1 = new dePropertyValueSlider(this, range, layer.getAdd1(), layer, layerProcessor);
    sizerM->Add(add1);

    add2 = new dePropertyValueSlider(this, range, layer.getAdd2(), layer, layerProcessor);
    sizerM->Add(add2);

    wxSizer* sizerBM = new wxStaticBoxSizer(wxHORIZONTAL, this,  _T(""));
    sizerM->Add(sizerBM);

    resetM = new wxButton(this, wxID_ANY, _T("reset"), wxDefaultPosition, wxSize(60,25));
    sizerBM->Add(resetM, 0);

    preset0M = new wxButton(this, wxID_ANY, wxString::FromAscii(getChannelName(sourceColorSpace, 0).c_str()), wxDefaultPosition, wxSize(60,25));
    sizerBM->Add(preset0M, 0);

    preset1M = new wxButton(this, wxID_ANY, wxString::FromAscii(getChannelName(sourceColorSpace, 1).c_str()), wxDefaultPosition, wxSize(60,25));
    sizerBM->Add(preset1M, 0);

    preset2M = new wxButton(this, wxID_ANY, wxString::FromAscii(getChannelName(sourceColorSpace, 2).c_str()), wxDefaultPosition, wxSize(60,25));
    sizerBM->Add(preset2M, 0);

    wxSizer* sizerO = new wxStaticBoxSizer(wxVERTICAL, this,  _T("overlay"));
    sizer->Add(sizerO);

    overlay0 = new dePropertyValueSlider(this, range, layer.getOverlay0(), layer, layerProcessor);
    sizerO->Add(overlay0);

    overlay1 = new dePropertyValueSlider(this, range, layer.getOverlay1(), layer, layerProcessor);
    sizerO->Add(overlay1);

    overlay2 = new dePropertyValueSlider(this, range, layer.getOverlay2(), layer, layerProcessor);
    sizerO->Add(overlay2);

    Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deMixerBWEditor::click));

    Layout();
    Fit();

}

deMixerBWEditor::~deMixerBWEditor()
{
    frameManager.removeActionFrame(this);
}

void deMixerBWEditor::click(wxCommandEvent &event)
{
    int id = event.GetId();

    if (resetM->GetId() == id)
    {
        layer.resetM();
    }      

    if (preset0M->GetId() == id)
    {
        layer.presetM(0);
    }      

    if (preset1M->GetId() == id)
    {
        layer.presetM(1);
    }      

    if (preset2M->GetId() == id)
    {
        layer.presetM(2);
    }      

    add0->setFromProperty();
    add1->setFromProperty();
    add2->setFromProperty();

    int index = layer.getIndex();
    layerProcessor.markUpdateAllChannels(index);

}

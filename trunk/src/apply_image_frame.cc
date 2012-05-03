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

#include "apply_image_frame.h"
#include "apply_image_layer.h"
#include "property_choice_ui.h"
#include "property_boolean_ui.h"
#include "layer_processor.h"
#include "color_space_utils.h"

deApplyImageFrame::deApplyImageFrame(wxWindow *parent, deLayer& _layer, deLayerProcessor& _layerProcessor, deLayerFrameManager& _frameManager, int _layerIndex)
:deActionFrame(parent, _layer, _frameManager, _layerIndex), layerProcessor(_layerProcessor)
{
    deApplyImageLayer& applyImageLayer = dynamic_cast<deApplyImageLayer&>(layer);

    wxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(mainSizer);

    appliedLayer = new dePropertyChoiceUIOld(this, applyImageLayer.getAppliedLayer(), applyImageLayer, layerProcessor, layerIndex);
    mainSizer->Add(appliedLayer);

    applySingleChannel = new dePropertyBooleanUIOld(this, applyImageLayer.getApplySingleChannel(), applyImageLayer, layerProcessor, layerIndex);
    mainSizer->Add(applySingleChannel);

    int i;
    for (i = 0; i < MAX_COLOR_SPACE_SIZE; i++)
    {
        int style = 0;
        if (i == 0)
        {
            style = wxRB_GROUP;
        }
        wxRadioButton* b = new wxRadioButton(this, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, style);
        mainSizer->Add(b, 0);
        channels[i] = b;
    }

    setChannels();

    Fit();

    Connect(wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(deApplyImageFrame::select));
}

deApplyImageFrame::~deApplyImageFrame()
{
}

void deApplyImageFrame::select(wxCommandEvent &event)
{
    deApplyImageLayer& applyImageLayer = dynamic_cast<deApplyImageLayer&>(layer);

    int i = event.GetId();
    int j;
    for (j = 0; j < MAX_COLOR_SPACE_SIZE; j++)
    {
        if (channels[j]->GetId() == i)
        {
            applyImageLayer.setAppliedChannel(j);
        }
    }

    layerProcessor.markUpdateAllChannels(layerIndex);
}

void deApplyImageFrame::setChannels()
{
    deApplyImageLayer& applyImageLayer = dynamic_cast<deApplyImageLayer&>(layer);

    deColorSpace colorSpace = applyImageLayer.getAppliedColorSpace();
    int n = getColorSpaceSize(colorSpace);
    int s = applyImageLayer.getAppliedChannel();
    int i;
    for (i = 0; i < MAX_COLOR_SPACE_SIZE; i++)
    {
        if (i < n)
        {
            std::string name = getChannelName(colorSpace, i);
            channels[i]->SetLabel(wxString::FromAscii(name.c_str()));
            channels[i]->Show();
        }
        else
        {
            channels[i]->Hide();
        }
        if (applyImageLayer.isSingleChannel())
        {
            channels[i]->Enable();
        }
        else
        {
            channels[i]->Disable();
        }
        if (i == s)
        {
            channels[i]->SetValue(1);
        }
    }
}

void deApplyImageFrame::onUpdateProperties()
{
    setChannels();
}


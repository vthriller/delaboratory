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
#include "view_manager.h"
#include "layer_processor.h"
#include "layer_frame_manager.h"
#include "color_space_utils.h"
#include "opacity_slider.h"

deBlendFrame::deBlendFrame(wxWindow *parent, deLayerWithBlending& _layer, deLayerProcessor& _layerProcessor, deLayerFrameManager& _frameManager, int _layerIndex)
:deLayerFrameOld(parent, _layer, "blend frame", _frameManager, _layerIndex),
 layerProcessor(_layerProcessor), blendModes(getSupportedBlendModes())
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    deColorSpace colorSpace = layer.getColorSpace();
    frameManager.addBlendFrame(this);

    deBlendMode currentBlendMode = _layer.getBlendMode();

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

    deSliderOld* alphaSlider = new deOpacitySlider(this, 100, _layer, layerProcessor, layerIndex);
    sizer->Add(alphaSlider, 0);

    wxSizer* sizerC = new wxStaticBoxSizer(wxVERTICAL, this,  _T("channels"));
    sizer->Add(sizerC, 0);

    unsigned int nc = getColorSpaceSize(colorSpace);

    for (i = 0; i < nc; i++)
    {
        wxCheckBox* cb = new wxCheckBox(this, wxID_ANY, wxString::FromAscii(getChannelName(colorSpace, i).c_str()));
        cb->SetValue(_layer.isChannelEnabled(i));
        sizerC->Add(cb, 0);
        channels.push_back(cb);
    }

    Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(deBlendFrame::choose));
    Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(deBlendFrame::check));

    Fit();
}

deBlendFrame::~deBlendFrame()
{
    layerProcessor.onChangeViewMode();

    frameManager.removeBlendFrame(this);
}


void deBlendFrame::choose(wxCommandEvent &event)
{
    deLayerWithBlending& l = dynamic_cast<deLayerWithBlending&>(layer);

    int id = event.GetId();
    if (id == choice->GetId())
    {
        int i = event.GetInt();
        deBlendMode& mode = blendModes[i];
        l.setBlendMode(mode);
        layerProcessor.markUpdateBlendAllChannels(layerIndex);
    }
    else
    {
    }
}

void deBlendFrame::check(wxCommandEvent &event)
{
    deLayerWithBlending& l = dynamic_cast<deLayerWithBlending&>(layer);
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
                l.enableChannel(i);
            }
            else
            {
                l.disableChannel(i);
            }

            layerProcessor.markUpdateSingleChannel(layerIndex, i);

            return;
        }
    }

}



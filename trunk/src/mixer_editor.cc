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

#include "mixer_editor.h"
#include "color_space.h"
#include "mixer_layer.h"
#include "mixer_editor_channel.h"
#include "layer_processor.h"
#include "str.h"

deMixerEditor::deMixerEditor(wxWindow *parent, deActionLayer& _layer, deLayerProcessor& _layerProcessor, deLayerFrameManager& _frameManager)
:deActionFrame(parent, _layer, _frameManager)
{

    deMixerLayer& mixerLayer = dynamic_cast<deMixerLayer&>(_layer);

    deColorSpace colorSpace = layer.getColorSpace();

    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    int n = getColorSpaceSize(colorSpace);

    int i;
    for (i = 0; i < n; i++)
    {
        deMixerEditorChannel* mixerEditorChannel = new deMixerEditorChannel(this, mixerLayer, i, _layerProcessor);
        sizer->Add(mixerEditorChannel);
        channels.push_back(mixerEditorChannel);
    }

    wxSizer* sizerB = new wxStaticBoxSizer(wxHORIZONTAL, this, _T(""));
    sizer->Add(sizerB, 0);

    reset = new wxButton(this, wxID_ANY, _T("reset"), wxDefaultPosition);
    sizerB->Add(reset, 0);
    random1 = new wxButton(this, wxID_ANY, _T("light random"), wxDefaultPosition);
    sizerB->Add(random1, 0);
    random2 = new wxButton(this, wxID_ANY, _T("medium random"), wxDefaultPosition);
    sizerB->Add(random2, 0);
    random3 = new wxButton(this, wxID_ANY, _T("heavy random"), wxDefaultPosition);
    sizerB->Add(random3, 0);

    sizer->Layout();

    Fit();

    Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deMixerEditor::click));

}

deMixerEditor::~deMixerEditor()
{
}

void deMixerEditor::onImageClick(deValue x, deValue y)
{
}

void deMixerEditor::click(wxCommandEvent &event)
{
    int id = event.GetId();

    std::vector<deMixerEditorChannel*>::iterator i;

    if (reset->GetId() == id)
    {
        for (i = channels.begin(); i != channels.end(); i++)
        {
            (*i)->preset(0);
        }
    }      

    if (random1->GetId() == id)
    {
        for (i = channels.begin(); i != channels.end(); i++)
        {
            (*i)->random(0.3);
        }
    }      

    if (random2->GetId() == id)
    {
        for (i = channels.begin(); i != channels.end(); i++)
        {
            (*i)->random(0.6);
        }
    }      

    if (random3->GetId() == id)
    {
        for (i = channels.begin(); i != channels.end(); i++)
        {
            (*i)->random(0.9);
        }
    }      
}   

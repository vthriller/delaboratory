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
    sizer->Layout();
    Fit();
}

deMixerEditor::~deMixerEditor()
{
}

void deMixerEditor::onImageClick(deValue x, deValue y)
{
}

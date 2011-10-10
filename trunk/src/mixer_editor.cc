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

deMixerEditor::deMixerEditor(wxWindow *parent, deActionLayer& _layer)
:deActionFrame(parent, _layer)
{
    deMixerLayer& mixerLayer = dynamic_cast<deMixerLayer&>(_layer);

    deColorSpace colorSpace = layer.getColorSpace();

    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    int n = getColorSpaceSize(colorSpace);

    int i;
    for (i = 0; i < n; i++)
    {
        deMixerEditorChannel* mixerEditorChannel = new deMixerEditorChannel(this, mixerLayer, i);
        sizer->Add(mixerEditorChannel);
        channels.push_back(mixerEditorChannel);
    }
    /*
    wxString* channelStrings = new wxString [n];
    int i;
    for (i = 0; i < n; i++)
    {
        channelStrings[i] = wxString::FromAscii(getChannelName(colorSpace,i).c_str());
    }        

    channelChoice =  new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(200, -1), n, channelStrings);
    channelChoice->SetSelection(0);

    delete [] channelStrings;

    sizer->Add(channelChoice);

    curvesPanel = new deCurvesPanel(this, curvesLayer );

    sizer->Add(curvesPanel);

    Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(deMixerEditor::choose));
    */
    sizer->Layout();
    Fit();
}

deMixerEditor::~deMixerEditor()
{
}

/*
void deMixerEditor::choose(wxCommandEvent &event)
{
    int b = channelChoice->GetCurrentSelection();
    curvesPanel->changeChannel(b);
}

void deMixerEditor::onImageClick(deValue x, deValue y)
{
    curvesPanel->onImageClick(x, y);
}
*/

void deMixerEditor::onImageClick(deValue x, deValue y)
{
}

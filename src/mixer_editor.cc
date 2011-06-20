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
#include "mixer.h"
#include "mixer_layer.h"
#include <iostream>
#include <sstream>
#include "layer_stack.h"
#include "preview.h"
#include "mixer_slider.h"
#include "mixer_editor_channel.h"

deMixerEditor::deMixerEditor(wxWindow *_parent, dePropertyMixer& _property)
:wxPanel(_parent),
parent(_parent),
property(_property)
{
    sizer = new wxBoxSizer(wxVERTICAL);
    build();
    SetSizer(sizer);
    Fit();
}    

void deMixerEditor::rebuild()
{
    destroy();
    build();
    Layout();
}

void deMixerEditor::build()
{
    deMixer* mixer = property.getMixer();
    if (!mixer)
    {
        return;
    }

    int d = getColorSpaceSize(mixer->getDestinationColorSpace());

    int i;
    for (i = 0; i < d; i++)
    {
        deMixerEditorChannel* editorChannel = new deMixerEditorChannel(this, property, i);
        panels.push_back(editorChannel);
        sizer->Add(editorChannel, 1, wxEXPAND);
    }
}

deMixerEditor::~deMixerEditor()
{
    destroy();
}

void deMixerEditor::destroy()
{
    while (panels.size() > 0)
    {
        std::list<wxPanel*>::iterator i = panels.begin();
        wxPanel* panel = *i;
        sizer->Detach(panel);
        panels.erase(i);
        delete panel;
    }
}


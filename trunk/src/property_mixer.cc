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

#include "property_mixer.h"
#include "mixer_editor.h"
#include "mixer.h"
#include "mixer_layer.h"

dePropertyMixer::dePropertyMixer(deLayer& _parent)
:deProperty(_parent)
{
    mixer = NULL;
    mixerEditor = NULL;
}

dePropertyMixer::~dePropertyMixer()
{
    if (mixer)
    {
        delete mixer;
    }        
}

void dePropertyMixer::recreateMixer()
{   
    deColorSpace s = parent.getSourceLayer()->getColorSpace();
    deColorSpace d = parent.getColorSpace();

    if (mixer)
    {
        delete mixer;
    }
    mixer = new deMixer(s, d);
    if (mixerEditor)
    {
        mixerEditor->rebuild();
    }        
}

void dePropertyMixer::addPanelContent(wxPanel* panel, wxSizer* sizer)
{
    mixerEditor = new deMixerEditor(panel, *this);
    sizer->Add(mixerEditor);
}    

void dePropertyMixer::onColorSpaceChange()
{
    recreateMixer();
}

void dePropertyMixer::saveContent(xmlNodePtr node)
{
    if (mixer)
    {
        mixer->save(node);
    }        
}

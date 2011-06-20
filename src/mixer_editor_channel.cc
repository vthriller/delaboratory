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

#include "mixer_editor_channel.h"
#include "mixer.h"
#include "mixer_slider.h"
#include "property_mixer.h"

deMixerEditorChannel::deMixerEditorChannel(wxWindow *parent, dePropertyMixer& _property, int _channel)
:wxPanel(parent), property(_property), channel(_channel)
{

    deMixer* mixer = property.getMixer();
    if (!mixer)
    {
        return;
    }

    int s = getColorSpaceSize(mixer->getSourceColorSpace());

    // FIXME constant
    sliderRange = 300;

    deColorSpace sourceColorSpace = mixer->getSourceColorSpace();
    deColorSpace colorSpace = mixer->getDestinationColorSpace();

    sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);
    
    std::string dc = getChannelName(colorSpace, channel);
    wxStaticText* destLabel = new wxStaticText(this, wxID_ANY, wxString::FromAscii(dc.c_str()), wxDefaultPosition, wxSize(100, -1));
    sizer->Add(destLabel, 1, wxEXPAND);

    int j;
    for (j = 0; j < s; j++)
    {
        std::string sc = getChannelName(sourceColorSpace, j);
        deMixerSlider* slider = new deMixerSlider(this, sc, sliderRange, mixer->getRangeMin(), mixer->getRangeMax(), j, channel, property);
        sliders.push_back(slider);
        sizer->Add(slider, 1, wxEXPAND);
    }
}

deMixerEditorChannel::~deMixerEditorChannel()
{
    while (sliders.size() > 0)
    {
        std::vector<deMixerSlider*>::iterator i = sliders.begin();
        delete *i;
        sliders.erase(i);
    }
}


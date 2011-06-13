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

deMixerEditor::deMixerEditor(wxWindow *parent, deMixerLayer& _mixerLayer, dePreviewStack& stack, int layerNumber)
:wxPanel(parent),
mixerLayer(_mixerLayer)
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    deMixer* mixer = mixerLayer.getMixer();
    if (!mixer)
    {
        return;
    }

    int s = getColorSpaceSize(mixer->getSourceColorSpace());
    int d = getColorSpaceSize(mixer->getDestinationColorSpace());

    // FIXME constant
    sliderRange = 300;

    deColorSpace sourceColorSpace = mixer->getSourceColorSpace();
    deColorSpace colorSpace = mixerLayer.getColorSpace();

    int i;
    for (i = 0; i < d; i++)
    {
        wxSizer* destSizer = new wxBoxSizer(wxVERTICAL);
        
        std::string dc = getChannelName(colorSpace, i);
        wxStaticText* destLabel = new wxStaticText(this, wxID_ANY, wxString::FromAscii(dc.c_str()), wxDefaultPosition, wxSize(100, -1));
        destSizer->Add(destLabel, 1, wxEXPAND);

        int j;
        for (j = 0; j < s; j++)
        {
            std::string sc = getChannelName(sourceColorSpace, j);
            deMixerSlider* slider = new deMixerSlider(this, sc, sliderRange, mixer->getRangeMin(), mixer->getRangeMax(), *mixer, j, i, stack, layerNumber);
            sliders.push_back(slider);
            destSizer->Add(slider, 1, wxEXPAND);
        }
        
        sizer->Add(destSizer, 1, wxEXPAND);
    }

    SetSizer(sizer);

    Fit();
}

deMixerEditor::~deMixerEditor()
{
    while (sliders.size() > 0)
    {
        std::vector<deMixerSlider*>::iterator i = sliders.begin();
        delete *i;
        sliders.erase(i);
    }
}


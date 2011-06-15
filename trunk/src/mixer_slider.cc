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

#include "mixer_slider.h"
#include "mixer.h"
#include "preview_stack.h"
#include "property_mixer.h"

deMixerSlider::deMixerSlider(wxWindow *parent, const std::string& labelString, int _sliderRange, deValue _valueMin, deValue _valueMax, deMixer& _mixer, int _s, int _d, dePropertyMixer& _property)
:deSlider(parent, labelString, _sliderRange, _valueMin, _valueMax), mixer(_mixer), s(_s), d(_d), property(_property)
{
    deValue v = mixer.getValue(s, d);
    setValue(v);
}

deMixerSlider::~deMixerSlider()
{
}

void deMixerSlider::onValueChange(deValue value)
{
    mixer.setValue(s, d, value);
    property.onUpdate();
}

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

#ifndef _DE_MIXER_SLIDER_H
#define _DE_MIXER_SLIDER_H

#include "slider.h"
class deMixer;
class dePreviewStack;

class deMixerSlider:public deSlider
{
    private:
        deMixer& mixer;
        int s;
        int d;
        dePreviewStack& stack;
        int layerNumber;
    public:
        deMixerSlider(wxWindow *parent, const std::string& labelString, int _sliderRange, deValue _valueMin, deValue _valueMax, deMixer& _mixer, int _s, int _d, dePreviewStack& _stack, int _layerNumber);
        virtual ~deMixerSlider();
        
        virtual void onValueChange(deValue value);

};

#endif

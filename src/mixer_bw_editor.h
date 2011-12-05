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

#ifndef _DE_MIXER_BW_EDITOR_H
#define _DE_MIXER_BW_EDITOR_H

#include "frame.h"
class deConversionBWLayer;
class dePropertyValueSlider;

class deMixerBWEditor:public deFrame
{
    private:
        deConversionBWLayer& layer;

        wxButton* resetM;
        wxButton* preset0M;
        wxButton* preset1M;
        wxButton* preset2M;

        dePropertyValueSlider* add0;
        dePropertyValueSlider* add1;
        dePropertyValueSlider* add2;
        dePropertyValueSlider* add3;
        dePropertyValueSlider* overlay0;
        dePropertyValueSlider* overlay1;
        dePropertyValueSlider* overlay2;
        dePropertyValueSlider* overlay3;

        void click(wxCommandEvent &event);

    public:
        deMixerBWEditor(wxWindow *parent, deConversionBWLayer& _layer);
        virtual ~deMixerBWEditor();

};


#endif

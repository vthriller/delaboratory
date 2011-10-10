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

#ifndef _DE_MIXER_EDITOR_CHANNEL_H
#define _DE_MIXER_EDITOR_CHANNEL_H

#include <wx/wx.h>
class deMixerLayer;
#include "slider.h"
#include <vector>

class deMixerEditorChannel:public wxPanel
{
    private:
        std::vector<deSlider*> sliders;
        deMixerLayer& layer;
        int index;

        wxButton* reset;
        wxButton* app1;
        wxButton* app2;
        wxButton* mix1;
        wxButton* mix2;
        wxButton* mix3;
        wxButton* mix4;

        void click(wxCommandEvent &event);
        void preset(deValue a);
        void preset2(deValue a, deValue b, deValue c);

    public:
        deMixerEditorChannel(wxWindow *parent, deMixerLayer& _layer, int _index);
        virtual ~deMixerEditorChannel();



};


#endif

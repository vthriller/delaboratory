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

#ifndef _DE_MIXER_EDITOR_H
#define _DE_MIXER_EDITOR_H

#include "action_frame.h"
class deMixerEditorChannel;
#include <vector>
class deLayerProcessor;

class deMixerEditor:public deActionFrame
{
    private:
        std::vector<deMixerEditorChannel*> channels;
        wxButton* reset;
        wxButton* random1;
        wxButton* random2;
        wxButton* random3;

        void click(wxCommandEvent &event);

    public:
        deMixerEditor(wxWindow *parent, deLayer& _layer, deLayerProcessor& _layerProcessor, deLayerFrameManager& _frameManager, int _layerIndex);
        virtual ~deMixerEditor();


};


#endif

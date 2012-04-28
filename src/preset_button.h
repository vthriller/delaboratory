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

#ifndef _DE_PRESET_BUTTON_H
#define _DE_PRESET_BUTTON_H

class deLayerProcessor;
class deWindow;
class deBaseLayer;
class deLayerFrame;
#include "button.h"

class dePresetButton:public deButton
{
    private:
        const std::string preset;
        deLayerProcessor& layerProcessor;
        deBaseLayer& layer;
        deLayerFrame& frame;

        int layerIndex;

    public:
        dePresetButton(deWindow& window, deBaseLayer& _layer, const std::string& name, deLayerProcessor& _layerProcessor, int _layerIndex, deLayerFrame& _frame);
        virtual ~dePresetButton();

        virtual void onClick();
};        

#endif

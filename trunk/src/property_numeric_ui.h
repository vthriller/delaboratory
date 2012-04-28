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

#ifndef _DE_PROPERTY_VALUE_SLIDER_H
#define _DE_PROPERTY_VALUE_SLIDER_H

class dePropertyNumeric;
class deLayerProcessor;
class deWindow;
#include "slider.h"

class dePropertyNumericUI:public deSlider
{
    private:
        dePropertyNumeric& property;
        deLayerProcessor& layerProcessor;

        int layerIndex;

    public:
        dePropertyNumericUI(deWindow& window, dePropertyNumeric& _property, deLayerProcessor& _layerProcessor, int _layerIndex, int _width, int width, int widthl);
        virtual ~dePropertyNumericUI();

        virtual void onValueChange(deValue value, bool finished);

        void setFromProperty();

};        

#endif

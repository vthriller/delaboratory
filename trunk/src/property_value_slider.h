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

#include "slider.h"
class deLayer;
class dePropertyValue;
class deLayerProcessor;

class dePropertyValueSlider:public deSlider
{
    private:
        dePropertyValue& property;
        deLayer& layer;
        deLayerProcessor& layerProcessor;

        int channel;

        int layerIndex;

    public:
        dePropertyValueSlider(wxWindow *parent, int _sliderRange, dePropertyValue& _property, deLayer& _layer, deLayerProcessor& _layerProcessor, int _layerIndex);
        virtual ~dePropertyValueSlider();

        virtual void onValueChange(deValue value, bool finished);

        void setFromProperty();
};        

#endif

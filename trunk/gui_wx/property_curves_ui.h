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

#ifndef _DE_PROPERTY_CURVES_UI_H
#define _DE_PROPERTY_CURVES_UI_H

class dePropertyCurves;
class deLayerProcessor;
class deWindow;
class dePropertyCurvesUIImpl;
class deBaseLayerWithSource;
#include "value.h"

class dePropertyCurvesUI
{
    private:
        dePropertyCurves& property;
        deLayerProcessor& layerProcessor;

        int layerIndex;

        deBaseLayerWithSource& layer;

        dePropertyCurvesUIImpl* impl;

    public:
        dePropertyCurvesUI(deWindow& window, dePropertyCurves& _property, deLayerProcessor& _layerProcessor, int _layerIndex, deBaseLayerWithSource& _layer, int width);
        virtual ~dePropertyCurvesUI();

        deWindow& getWindow();

        void setFromProperty();

        bool onImageClick(deValue x, deValue y);
        bool onKey(int key);
};        

#endif

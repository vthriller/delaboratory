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

#ifndef _DE_PROPERTY_BOOLEAN_UI_H
#define _DE_PROPERTY_BOOLEAN_UI_H

#include "check_box.h"
class deLayer;
class dePropertyBoolean;
class deLayerProcessor;

class dePropertyBooleanUI:public deCheckBox
{
    private:
        dePropertyBoolean& property;
        deLayer& layer;
        deLayerProcessor& layerProcessor;
        int layerIndex;

    public:
        dePropertyBooleanUI(wxWindow *parent, dePropertyBoolean& _property, deLayer& _layer, deLayerProcessor& _layerProcessor, int _layerIndex);
        virtual ~dePropertyBooleanUI();

        virtual void onCheck(bool c);

        void setFromProperty();

};        

#endif

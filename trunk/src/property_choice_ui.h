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

#ifndef _DE_PROPERTY_CHOICE_UI_H
#define _DE_PROPERTY_CHOICE_UI_H

#include "choice.h"
class deLayerOld;
class dePropertyChoice;
class deLayerProcessor;

class dePropertyChoiceUIOld:public deChoiceOld
{
    private:
        dePropertyChoice& property;
        deLayerOld& layer;
        deLayerProcessor& layerProcessor;
        int layerIndex;

    public:
        dePropertyChoiceUIOld(wxWindow *parent, dePropertyChoice& _property, deLayerOld& _layer, deLayerProcessor& _layerProcessor, int _layerIndex);
        virtual ~dePropertyChoiceUIOld();

        virtual void onChoose(int c);

        void setFromProperty();

};        

class dePropertyChoiceUI:public deChoice
{
    private:
        dePropertyChoice& property;
        deLayerProcessor& layerProcessor;

        int layerIndex;

    public:
        dePropertyChoiceUI(deWindow& window, dePropertyChoice& _property, deLayerProcessor& _layerProcessor, int _layerIndex);
        virtual ~dePropertyChoiceUI();

        virtual void onChoose(int index);

        void setFromProperty();

};        


#endif

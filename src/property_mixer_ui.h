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

#ifndef _DE_PROPERTY_MIXER_UI_H
#define _DE_PROPERTY_MIXER_UI_H

class dePropertyMixer;
class deLayerProcessor;
class deWindow;
class dePropertyMixerUIImpl;
class deBaseLayerWithSource;

class dePropertyMixerUI
{
    private:
        dePropertyMixer& property;
        deLayerProcessor& layerProcessor;

        int layerIndex;

        deBaseLayerWithSource& layer;

        dePropertyMixerUIImpl* impl;

    public:
        dePropertyMixerUI(deWindow& window, dePropertyMixer& _property, deLayerProcessor& _layerProcessor, int _layerIndex, deBaseLayerWithSource& _layer, int width);
        virtual ~dePropertyMixerUI();

        deWindow& getWindow();

        void setFromProperty();
};        

#endif

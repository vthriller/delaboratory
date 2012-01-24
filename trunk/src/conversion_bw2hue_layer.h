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

#ifndef _DE_CONVERSION_BW2HUE_LAYER_H
#define _DE_CONVERSION_BW2HUE_LAYER_H

#include "conversion_layer.h"
#include "mixer.h"
#include "property_value.h"
class deViewManager;

class deConversionBW2HueLayer:public deConversionLayer
{
    private:
        dePropertyValue hue;
        dePropertyValue saturation;

        virtual bool hasAction() const {return true;};

        virtual std::string getType() const {return "conversion_bw2hue";};
    public:
        deConversionBW2HueLayer(int _index, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deColorSpace _colorSpace);
        virtual ~deConversionBW2HueLayer();

        virtual void updateImage();

        dePropertyValue& getPropertyHue() {return hue;};
        dePropertyValue& getPropertySaturation() {return saturation;};

        virtual std::string getActionName() {return "setup";};

        void reset();

};

#endif

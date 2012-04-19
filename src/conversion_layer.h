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

#ifndef _DE_CONVERSION_LAYER_H
#define _DE_CONVERSION_LAYER_H

#include "base_layer_with_source.h"
#include "image.h"
class deLayerStack;
class deChannelManager;

class deConversionLayer:public deBaseLayerWithSource
{
    private:
        virtual std::string getType() const {return "conversion";};

    public:
        deConversionLayer(deColorSpace _colorSpace, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager);
        virtual ~deConversionLayer();

        virtual bool updateImage();

        virtual void updateChannelUsage(std::map<int, int>& channelUsage, int layerIndex) const;

        virtual void load(xmlNodePtr root) {};
        virtual void save(xmlNodePtr root) {saveCommon(root);};


};

#endif

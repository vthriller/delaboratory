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

#ifndef _DE_GRADIENT_LAYER_H
#define _DE_GRADIENT_LAYER_H

#include "layer_with_blending.h"
class deViewManager;

class deGradientLayer:public deLayerWithBlending
{
    private:
        virtual std::string getType() const {return "gradient";};
        deViewManager& viewManager;

    public:
        deGradientLayer(deColorSpace _colorSpace, deChannelManager& _channelManager, int _sourceLayer, deLayerStack& _layerStack, deViewManager& _viewManager);
        virtual ~deGradientLayer();

        virtual bool isChannelNeutral(int index);
        virtual bool updateMainImageSingleChannel(int channel);

        virtual std::string getActionName() {return "gradient";};

        virtual bool onImageClick(deValue x, deValue y);

};

#endif

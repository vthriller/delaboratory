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

#ifndef _DE_CONVERSION_BW_LAYER_H
#define _DE_CONVERSION_BW_LAYER_H

#include "conversion_layer.h"
#include "mixer.h"
class deViewManager;

class deConversionBWLayer:public deConversionLayer
{
    private:
        deMixer mixer;

        // from project
        deViewManager& viewManager;

        virtual bool hasAction() const {return true;};
    public:
        deConversionBWLayer(int _index, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager, int n);
        virtual ~deConversionBWLayer();

        virtual void createActionFrame(wxWindow* parent);

        virtual void updateImage();

        deColorSpace getSourceColorSpace() const;

        void setWeight(int s, deValue value);
        deValue getWeight(int s);

        void updateAndRepaint();

        virtual std::string getActionName() {return "mixer";};

};

#endif

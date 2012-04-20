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

#ifndef _DE_HIGH_PASS_LAYER_H
#define _DE_HIGH_PASS_LAYER_H

#include "layer.h"
#include "property_value.h"

class deChannel;

class deHighPassLayer:public deLayer
{
    private:
        int blurRadiusIndex;
        deViewManager& viewManager;

    protected:
        virtual std::string getType() const {return "high_pass";};
        virtual std::string getLabel() const {return "high pass";};

    public:
        deHighPassLayer(deColorSpace _colorSpace, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager);
        virtual ~deHighPassLayer();

        virtual bool isChannelNeutral(int index);


        virtual void load(xmlNodePtr root);
        virtual void save(xmlNodePtr root);

        virtual std::string getActionName() {return "hp";};

        void reset();

        bool processHP(const deChannel& sourceChannel, deChannel& channel);
        virtual bool updateMainImageSingleChannel(int i);

};

#endif

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

#ifndef _DE_DODGE_BURN_LAYER_H
#define _DE_DODGE_BURN_LAYER_H

#include "layer.h"
#include "property_value.h"
#include "property_boolean.h"

class dePresetLayer;
class deChannel;

class deDodgeBurnLayer:public deLayerOld
{
    private:
        dePropertyBoolean alternate;
        int blurRadiusIndex;
        int blur2RadiusIndex;
        int dodgeAmountIndex;
        int dodgeMinIndex;
        int dodgeMaxIndex;
        int burnAmountIndex;
        int burnMinIndex;
        int burnMaxIndex;
        deViewManager& viewManager;

    protected:
        virtual std::string getType() const {return "dodge_burn";};
        virtual std::string getLabel() const {return "dodge / burn";};

    public:
        deDodgeBurnLayer(deColorSpace _colorSpace, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager);
        virtual ~deDodgeBurnLayer();

        virtual bool isChannelNeutral(int index);


        virtual void load(xmlNodePtr root);
        virtual void save(xmlNodePtr root);

        virtual std::string getActionName() {return "d/b";};

        dePropertyBoolean& getPropertyAlternate() {return alternate;};

        bool processDB(const deChannel& sourceChannel, deChannel& channel);
        virtual bool updateMainImageSingleChannel(int i);


};

#endif

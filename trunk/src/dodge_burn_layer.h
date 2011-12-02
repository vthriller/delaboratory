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

#include "action_layer.h"
#include "property_value.h"

class deDodgeBurnLayer:public deActionLayer
{
    private:
        dePropertyValue blurRadius;
        dePropertyValue dodgeAmount;
        dePropertyValue dodgeMin;
        dePropertyValue dodgeMax;
        dePropertyValue burnAmount;
        dePropertyValue burnMin;
        dePropertyValue burnMax;

    protected:
        virtual bool singleChannelProcessing() const {return true;};
        virtual std::string getType() const {return "dodge_burn";};

    public:
        deDodgeBurnLayer(deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deLayerProcessor& _layerProcessor, deChannelManager& _channelManager, deViewManager& _viewManager, const std::string& _name);
        virtual ~deDodgeBurnLayer();

        virtual bool isChannelNeutral(int index);

        virtual void processAction(int i, const deChannel& sourceChannel, deChannel& channel, deSize size);

        virtual void load(xmlNodePtr root);
        virtual void save(xmlNodePtr root);

        virtual std::string getActionName() {return "setup";};

        dePropertyValue& getPropertyRadius() {return blurRadius;};
        dePropertyValue& getPropertyDodgeAmount() {return dodgeAmount;};

        void reset();

};

#endif

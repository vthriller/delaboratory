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

#ifndef _DE_APPLY_IMAGE_LAYER_H
#define _DE_APPLY_IMAGE_LAYER_H

#include "action_layer.h"
#include "property_choice.h"
#include "property_boolean.h"

class deApplyImageLayer:public deActionLayer
{
    private:
        dePropertyChoice appliedLayer;
        dePropertyBoolean applySingleChannel;
        int appliedChannel;

        virtual bool simpleActionProcessing() const {return true;};

    protected:
        virtual bool singleChannelProcessing() const {return true;};
        virtual std::string getType() const {return "apply_image";};

    public:
        deApplyImageLayer(deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deLayerProcessor& _layerProcessor, deChannelManager& _channelManager, deViewManager& _viewManager, const std::string& _name);
        virtual ~deApplyImageLayer();

        virtual bool isChannelNeutral(int index);

        virtual void processAction(int i);

        bool isSingleChannel() const;

        void setAppliedChannel(int c);

        dePropertyChoice& getAppliedLayer() {return appliedLayer;};
        dePropertyBoolean& getApplySingleChannel() {return applySingleChannel;};

        int getAppliedChannel() const {return appliedChannel;};

        deColorSpace getAppliedColorSpace();

        virtual void load(xmlNodePtr root);
        virtual void save(xmlNodePtr root);

        virtual std::string getActionName() {return "setup";};


};

#endif

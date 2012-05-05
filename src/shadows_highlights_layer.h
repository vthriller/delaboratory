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

#ifndef _DE_SHADOWS_HIGHLIGHTS_LAYER_H
#define _DE_SHADOWS_HIGHLIGHTS_LAYER_H

#include "layer.h"
#include "property_value.h"

class deChannel;

class deShadowsHighlightsLayer:public deLayerOld
{
    private:
        int blurRadiusIndex;
        int shadowsHighlightsAmountIndex;
        int darkenAmountIndex;
        int lightenAmountIndex;
        deViewManager& viewManager;

    protected:
        virtual std::string getType() const {return "shadows_highlights";};
        virtual std::string getLabel() const {return "shadows / highlights";};

    public:
        deShadowsHighlightsLayer(deColorSpace _colorSpace, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager);
        virtual ~deShadowsHighlightsLayer();

        virtual bool isChannelNeutral(int index);


        virtual void load(xmlNodePtr root);
        virtual void save(xmlNodePtr root);

        virtual std::string getActionName() {return "s/h";};

        void reset();

        bool processSH(const deChannel& sourceChannel, deChannel& channel);
        virtual bool updateMainImageSingleChannel(int i);


};

#endif

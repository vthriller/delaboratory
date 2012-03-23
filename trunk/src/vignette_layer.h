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

#ifndef _DE_VIGNETTE_LAYER_H
#define _DE_VIGNETTE_LAYER_H

#include "action_layer.h"
#include "property_value.h"

class deVignetteLayer:public deActionLayer
{
    private:
        int vignettes;

        int radius1XIndex;
        int radius1YIndex;
        int center1XIndex;
        int center1YIndex;

        int radius2XIndex;
        int radius2YIndex;
        int center2XIndex;
        int center2YIndex;

        int lightIndex;
        int darknessIndex;
        int spotIndex;

    protected:
        virtual bool singleChannelProcessing() const {return true;};
        virtual std::string getType() const {return "vignette";};

    public:
        deVignetteLayer(deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager, const std::string& _name, int _vignettes);
        virtual ~deVignetteLayer();

        virtual bool isChannelNeutral(int index);

        virtual bool processAction(int i, const deChannel& sourceChannel, deChannel& channel, deSize size);

        virtual void load(xmlNodePtr root);
        virtual void save(xmlNodePtr root);

        virtual std::string getActionName() {return "vign";};

        void reset();

        virtual bool randomize();

        bool setCenter(deValue x, deValue y);

};

class deVignetteLayer1:public deVignetteLayer
{
    public:
        deVignetteLayer1(deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager, const std::string& _name);
        virtual ~deVignetteLayer1();

        virtual std::string getType() const {return "vignette1";};
};

class deVignetteLayer2:public deVignetteLayer
{
    public:
        deVignetteLayer2(deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager, const std::string& _name);
        virtual ~deVignetteLayer2();

        virtual std::string getType() const {return "vignette2";};
};

#endif

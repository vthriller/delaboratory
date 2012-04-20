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

#ifndef _DE_MIXER_LAYER_H
#define _DE_MIXER_LAYER_H

#include "layer.h"
#include "mixer.h"

class deMixerLayer:public deLayer
{
    private:
        std::vector<deMixer*> mixers;

    protected:
        virtual std::string getType() const {return "mixer";};
        virtual std::string getLabel() const {return "channel mixer";};

    public:
        deMixerLayer(deColorSpace _colorSpace, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager);
        virtual ~deMixerLayer();

        deMixer* getMixer(int index);

        virtual bool isChannelNeutral(int index);

        void setWeight(int s, int d, deValue value);
        deValue getWeight(int s, int d);

        virtual void load(xmlNodePtr root);
        virtual void save(xmlNodePtr root);

        virtual std::string getActionName() {return "mixer";};

        virtual bool updateMainImageSingleChannel(int i);




};

#endif

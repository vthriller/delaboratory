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

#include "action_layer.h"
#include "mixer.h"

class deMixerLayer:public deActionLayer
{
    private:
        std::vector<deMixer*> mixers;

    protected:
        virtual bool singleChannelProcessing() const {return false;};
        virtual std::string getType() const {return "mixer";};
        virtual std::string getLabel() const {return "channel mixer";};

    public:
        deMixerLayer(deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager);
        virtual ~deMixerLayer();

        deMixer* getMixer(int index);

        virtual bool isChannelNeutral(int index);

        virtual bool processAction4(int i, const deChannel* s1, const deChannel* s2, const deChannel* s3, const deChannel* s4, deChannel& channel, int channelSize);

        void setWeight(int s, int d, deValue value);
        deValue getWeight(int s, int d);

        virtual void load(xmlNodePtr root);
        virtual void save(xmlNodePtr root);

        virtual std::string getActionName() {return "mixer";};




};

#endif

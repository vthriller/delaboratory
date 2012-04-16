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

#ifndef _DE_EQUALIZER_LAYER_H
#define _DE_EQUALIZER_LAYER_H

#include "action_layer.h"
class deEqualizer;

class deEqualizerLayer:public deActionLayer
{
    private:
        std::vector<deEqualizer*> equalizers;
        int bands;

    protected:
        virtual bool singleChannelProcessing() const {return false;};

    public:
        deEqualizerLayer(deColorSpace _colorSpace, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager, int _bands);
        virtual ~deEqualizerLayer();

        virtual bool isChannelNeutral(int index);

        virtual bool processAction4(int i, const deChannel* s1, const deChannel* s2, const deChannel* s3, const deChannel* s4, deChannel& channel, int channelSize);

        virtual void load(xmlNodePtr root);
        virtual void save(xmlNodePtr root);


        int getBands() const {return bands;};
        deEqualizer* getEqualizer(int index);

        void reset();
};

class deEqualizerLayer8:public deEqualizerLayer
{
    public:
        deEqualizerLayer8(deColorSpace _colorSpace, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager);
        virtual ~deEqualizerLayer8();

        virtual std::string getActionName() {return "eq8";};
        virtual std::string getType() const {return "equalizer8";};
        virtual std::string getLabel() const {return "equalizer8";};
};

class deEqualizerLayer16:public deEqualizerLayer
{
    public:
        deEqualizerLayer16(deColorSpace _colorSpace, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager);
        virtual ~deEqualizerLayer16();

        virtual std::string getActionName() {return "eq16";};
        virtual std::string getType() const {return "equalizer16";};
        virtual std::string getLabel() const {return "equalizer16";};
};


#endif

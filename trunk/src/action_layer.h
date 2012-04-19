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

#ifndef _DE_ACTION_LAYER_H
#define _DE_ACTION_LAYER_H

#include "layer.h"
#include "blend_mode.h"
#include "size.h"
#include "image.h"
#include "channel.h"
class deChannelManager;
class deViewManager;
#include <set>

class deActionLayer:public deLayer
{
    private:

    protected:
        deViewManager& viewManager;

    private:        

        virtual bool processAction(int i, const deChannel& sourceChannel, deChannel& channel, deSize size) {return false;};

    public:
        deActionLayer(deColorSpace _colorSpace, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager);
        virtual ~deActionLayer();

        deChannel* getSourceChannel(int index);
        deSize getChannelSize() const;

        virtual void loadBlend(xmlNodePtr root);
        virtual void saveBlend(xmlNodePtr root);

        void setHistogramChannel(int channel);

        virtual bool updateMainImageSingleChannel(int i);


};

#endif

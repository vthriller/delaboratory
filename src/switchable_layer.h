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

#ifndef _DE_SWITCHABLE_LAYER_H
#define _DE_SWITCHABLE_LAYER_H

#include "base_layer_with_source.h"
#include <set>

class deSwitchableLayer:public deBaseLayerWithSource
{
    private:
        bool enabled;

        std::set<int> channels;

    protected:
        void disableNotLuminance();
        void disableNotForSharpen();

    public:
        deSwitchableLayer(deColorSpace _colorSpace, deChannelManager& _channelManager, int _sourceLayer, deLayerStack& _layerStack);
        virtual ~deSwitchableLayer();

        bool isEnabled() const;
        void setEnabled(bool e);

        bool isChannelEnabled(int index) const;
        void enableChannel(int index);
        void disableChannel(int index);




};

#endif

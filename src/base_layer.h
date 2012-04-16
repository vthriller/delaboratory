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

#ifndef _DE_BASE_LAYER_H
#define _DE_BASE_LAYER_H

#include "color_space.h"
#include "mutex.h"
#include <libxml/parser.h>
#include <string>
#include <map>
class deImage;

enum deLayerProcessType
{   
    deLayerProcessInvalid,
    deLayerProcessFull,
    deLayerProcessSingleChannel,
    deLayerProcessBlend
};

class deBaseLayer
{
    private:
        deBaseLayer(const deBaseLayer& layer);
        deBaseLayer& operator = (const deBaseLayer& layer);

        virtual bool updateImage() = 0;

        deMutex mutex;

    protected:
        deColorSpace colorSpace;

    public:
        deBaseLayer(deColorSpace _colorSpace);
        virtual ~deBaseLayer();

        deColorSpace getColorSpace() const;

        virtual void load(xmlNodePtr root) = 0;
        virtual void save(xmlNodePtr root) = 0;
        void saveCommon(xmlNodePtr node);

        void lockLayer();
        void unlockLayer();

        virtual std::string getType() const = 0;

        virtual const deImage& getLayerImage() const = 0;

        void process(deLayerProcessType type, int channel);

        bool processFull();
        virtual void processBlend() {};
        virtual void processChannel(int channel) {};

        bool updateImageThreadCall();

        virtual void updateChannelUsage(std::map<int, int>& channelUsage, int layerIndex) const = 0;

        virtual void onUpdateProperties() {};


};

#endif

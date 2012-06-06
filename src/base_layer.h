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
#include <string>
#include <vector>
#include "image.h"
class deChannelManager;
class deViewManager;
class deProperty;
class dePreset;
class dePropertyNumeric;
class dePropertyChoice;
class dePropertyBoolean;

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

        deMutex mutex;

        virtual bool setChannelInMainImage(int channel) {return true;};
        virtual bool updateMainImageSingleChannel(int channel) {return false;};

    protected:
        deColorSpace colorSpace;
        deImage mainLayerImage;
        bool errorOnUpdate;

        virtual bool updateBlendAllChannels() {return false;};
        bool updateMainImageAllChannels();
        virtual bool updateMainImageNotThreadedWay() {return false;};
        virtual bool updateImage();

    public:
        deBaseLayer(deColorSpace _colorSpace, deChannelManager& _channelManager);
        virtual ~deBaseLayer();

        deColorSpace getColorSpace() const;

        void lockLayer();
        void unlockLayer();

        void processLayer(deLayerProcessType type, int channel);

        bool processFull();
        virtual void processSingleChannel(int channel);

        bool updateImageThreadCall();

        virtual std::string getType() const = 0;

        virtual const deImage& getLayerImage() const;

        virtual void updateChannelUsage(std::map<int, int>& channelUsage, int layerIndex) const;

        void setErrorOnUpdateFromThread();

        virtual std::string getWarning() const {return "OK";};

        virtual bool onImageClick(deValue x, deValue y) {return false;};
        virtual void beforeSetUIFromLayer() {};

        bool processMainImageSingleChannel(int channel);

};

#endif

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

#ifndef _DE_SOURCE_IMAGE_LAYER_H
#define _DE_SOURCE_IMAGE_LAYER_H

#include "layer.h"
class deProject;
#include "image.h"
class deChannelManager;
class deViewManager;
class deStaticImage;

class deSourceImageLayer:public deBaseLayer
{
    private:
        deChannelManager& previewChannelManager;
        deViewManager& viewManager;

        deImage image;
        deStaticImage& sourceImage;

        virtual std::string getType() const {return "original";};

        virtual bool updateImage();

    public:
        deSourceImageLayer(deChannelManager& _previewChannelManager, deViewManager& _viewManager, deStaticImage& _sourceImage, deColorSpace _colorSpace);
        virtual ~deSourceImageLayer();

        virtual const deImage& getLayerImage() const;

        virtual void updateChannelUsage(std::map<int, int>& channelUsage, int layerIndex) const;

        virtual void load(xmlNodePtr root) {};
        virtual void save(xmlNodePtr root) {saveCommon(root);};

};

#endif

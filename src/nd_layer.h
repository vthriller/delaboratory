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

#ifndef _DE_ND_LAYER_H
#define _DE_ND_LAYER_H

#include "layer.h"
#include "property_nd_type.h"
#include "property_value.h"
#include "property_channels.h"
#include "property_blend_mode.h"
#include <set>

class deNDLayer:public deLayer
{
    private:
        dePropertyNDType ndType;
        dePropertyValue xCenter;
        dePropertyValue yCenter;
        dePropertyValue power;
        dePropertyBlendMode blendMode;
        dePropertyChannels channels;

    public:
        deNDLayer(deLayerStack& _stack, int _index, const std::string& _name);
        virtual ~deNDLayer();

        virtual bool canChangeSourceLayer() const {return true;};
        virtual void onChangeColorSpace();

        virtual dePreview* createPreview(dePreviewStack& previewStack);
        bool updatePreview(const dePreview* sourcePreview, dePreview* preview);

        virtual std::string getType() const {return "nd";};

        virtual void saveSpecific(xmlNodePtr node);
        virtual void loadSpecific(xmlNodePtr node);

};

#endif

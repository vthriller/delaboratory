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

#ifndef _DE_BLUR_LAYER_H
#define _DE_BLUR_LAYER_H

#include "layer.h"
#include "blur.h"
#include "property_value.h"
#include "property_blur_direction.h"
#include "property_channels.h"
#include <set>

class deBlurLayer:public deLayer
{
    private:
        dePropertyBlurDirection direction;
        dePropertyValue radius;
        dePropertyChannels channels;

    public:
        deBlurLayer(deLayerStack& _stack, int _index, const std::string& _name);
        virtual ~deBlurLayer();

        virtual bool canChangeSourceLayer() const {return true;};

        virtual dePreview* createPreview(dePreviewStack& previewStack);

        virtual deActionFrame* createActionFrame(wxWindow* parent, int layerNumber, deProject* project);
        virtual void onChangeColorSpace();

        deBlurDirection getBlurDirection() const;
        deValue getBlurRadius() const;
        void setBlurRadius(deValue _radius);
        void setBlurDirection(deBlurDirection _direction);

        void clearEnabledChannels();
        void enableChannel(int c);

        const std::set<int>& getEnabledChannels() const;
        virtual std::string getType() const {return "blur";};

        virtual void saveSpecific(xmlNodePtr node);
        virtual void loadSpecific(xmlNodePtr node);


};

#endif

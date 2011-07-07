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

#ifndef _DE_BLEND_LAYER_H
#define _DE_BLEND_LAYER_H

#include "layer.h"
#include "blend_mode.h"
#include "property_value.h"
#include "property_channels.h"
#include "property_blend_mode.h"

class deBlendLayer:public deLayer
{
    private:
        dePropertyValue alpha;
//        dePropertyLayerIndex maskLayer;
        dePropertyBlendMode blendMode;
        dePropertyChannels channels;
        dePropertyLayerIndex overlayLayer;
    public:
        deBlendLayer(deLayerStack& _stack, int _index, const std::string& _name);
        virtual ~deBlendLayer();

        virtual bool canChangeSourceLayer() const {return true;};
        virtual bool canChangeOverlayLayer() const {return true;};
        virtual bool canChangeColorSpace() const {return true;};

        deBlendMode getBlendMode() const;
        void setBlendMode(deBlendMode mode);

        void changeAlpha(deValue _alpha);
        deValue getAlpha() const {return alpha.getValue();};

        virtual dePreview* createPreview(dePreviewStack& previewStack);

        virtual deActionFrame* createActionFrame(wxWindow* parent, int layerNumber, deProject* project);

        virtual std::string getType() const {return "blend";};

        virtual void saveSpecific(xmlNodePtr node);
        virtual void loadSpecific(xmlNodePtr node);

        virtual void onChangeSourceLayer();
        virtual void onChangeColorSpace();

};

#endif

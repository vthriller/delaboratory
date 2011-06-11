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

class deBlendLayer:public deLayer
{
    private:
        deValue alpha;
        int overlayChannel;
        int destinationChannel;
        bool singleOverlayChannel;
        bool singleDestinationChannel;
        deBlendMode blendMode;
    public:
        deBlendLayer(deLayerStack& _stack, const std::string& _name);
        virtual ~deBlendLayer();

        virtual bool canChangeSourceLayer() const {return true;};
        virtual bool canChangeOverlayLayer() const {return true;};
        virtual bool canChangeColorSpace() const {return true;};

        void setOverlayChannel(int _channel);
        int getOverlayChannel() const;

        void setSingleOverlayChannel(bool _singleChannel);
        bool isSingleOverlayChannel() const;

        void setDestinationChannel(int _channel);
        int getDestinationChannel() const;

        void setSingleDestinationChannel(bool _singleChannel);
        bool isSingleDestinationChannel() const;

        deBlendMode getBlendMode() const;
        void setBlendMode(deBlendMode mode);

        void changeAlpha(deValue _alpha);
        deValue getAlpha() const {return alpha;};

        virtual dePreview* createPreview(dePreviewStack& previewStack);

        virtual deActionFrame* createActionFrame(wxWindow* parent, int layerNumber, deProject* project);

};

#endif

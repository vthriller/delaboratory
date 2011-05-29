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
        deBlendLayer(const std::string& _name);
        virtual ~deBlendLayer();

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

//        virtual void generatePreview();
        virtual dePreview* createPreview(dePreviewStack& previewStack);

        virtual void changeSourceLayer(int id, const deLayerStack& layerStack);
        //void changeOverlayLayer(int id);
        virtual void changeOverlayLayer(int id, const deLayerStack& layerStack);
        void changeAlpha(deValue _alpha);
        virtual void changeColorSpace(deColorSpace _colorSpace, const deLayerStack& layerStack);

        deValue getAlpha() const {return alpha;};

        virtual wxDialog* createDialog(wxWindow* parent, int layerNumber, deProject* project);
        virtual deActionFrame* createActionFrame(wxWindow* parent, int layerNumber, deProject* project);

};

#endif

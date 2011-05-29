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

#ifndef _DE_CONVERSION_LAYER_H
#define _DE_CONVERSION_LAYER_H

#include "layer.h"
#include "mixer.h"
#include "color_space.h"

class deConversionLayer:public deLayer
{
    private:

    public:
        deConversionLayer(const std::string& _name);
        virtual ~deConversionLayer();

//        virtual void generatePreview();
        virtual dePreview* createPreview(dePreviewStack& previewStack);
        virtual void changeSourceLayer(int id, const deLayerStack& layerStack);

        virtual wxDialog* createDialog(wxWindow* parent, int layerNumber, deProject* project);
        virtual deActionFrame* createActionFrame(wxWindow* parent, int layerNumber, deProject* project);

        int getSourceLayer() const {return sourceLayer;};
        
        virtual void changeColorSpace(deColorSpace _colorSpace, const deLayerStack& layerStack);
};

#endif

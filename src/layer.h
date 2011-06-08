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

#ifndef _DE_LAYER_H
#define _DE_LAYER_H

#include <string>
#include "color_space.h"
class dePreview;
class deLayerStack;
class dePreviewStack;
class wxFrame;
class wxWindow;
class deProject;
class deActionFrame;

class deLayer
{
    private:
        deLayer(const deLayer& layer);
        deLayer& operator = (const deLayer& layer);
        deActionFrame* actionFrame;

    protected:
        std::string name;
        deColorSpace colorSpace;
        int sourceLayer;
        int overlayLayer;

    public:
        deLayer(const std::string& _name);
        virtual ~deLayer();

        virtual dePreview* createPreview(dePreviewStack& previewStack) = 0;

        virtual bool canChangeSourceLayer() const {return false;};
        virtual bool canChangeOverlayLayer() const {return false;};
        virtual bool canChangeColorSpace() const {return false;};

        void changeSourceLayer(int id, const deLayerStack& layerStack);
        void changeOverlayLayer(int id, const deLayerStack& layerStack);
        void changeColorSpace(deColorSpace _colorSpace, const deLayerStack& layerStack);

        virtual void onChangeColorSpace(const deLayerStack& layerStack) {};
        virtual void onChangeSourceLayer(const deLayerStack& layerStack) {};
        virtual void onChangeOverlayLayer(const deLayerStack& layerStack) {};

        void updateColorSpace(const deLayerStack& layerStack);

        const deColorSpace& getColorSpace() const {return colorSpace;};

        void setName(const std::string& _name);
        const std::string& getName() const {return name;};

        wxDialog* createDialog(wxWindow* parent, int layerNumber, deProject* project);
        virtual deActionFrame* createActionFrame(wxWindow* parent, int layerNumber, deProject* project) = 0;
        void closeActionFrame();
        void setActionFrame(deActionFrame* frame);
        deActionFrame* getActionFrame();

        virtual const std::string getDescription() const {return "base layer";};

        int getSourceLayerID() const {return sourceLayer;};
        int getOverlayLayerID() const {return overlayLayer;};

};

#endif

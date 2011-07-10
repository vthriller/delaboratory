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

#include <list>
#include "property_name.h"
#include "property_color_space.h"
#include "property_layer_index.h"
class deLayerStack;
class deActionFrame;
class dePreview;
class deProject;

typedef std::list<deProperty*> deProperties;

class deLayer
{
    private:
        deLayer(const deLayer& layer);
        deLayer& operator = (const deLayer& layer);
        deActionFrame* actionFrame;

        deProperties properties;

    protected:
        deLayerStack& stack;
        int index;
        dePropertyName name;
        dePropertyColorSpace colorSpace;
        dePropertyLayerIndex sourceLayer;

    public:
        deLayer(deLayerStack& _stack, int _index, const std::string& _name);
        virtual ~deLayer();

        virtual dePreview* createPreview(dePreviewStack& previewStack) = 0;

        virtual bool canChangeSourceLayer() const {return false;};
        virtual bool canChangeOverlayLayer() const {return false;};
        virtual bool canChangeColorSpace() const {return false;};

        void changeSourceLayer(int id);
        void changeColorSpace(deColorSpace _colorSpace);

        virtual void onChangeColorSpace() {};
        virtual void onChangeSourceLayer() {};
        virtual void onChangeOverlayLayer() {};

        void notifyPropertiesOnColorSpaceChange();

        void updateColorSpace();

        const deColorSpace getColorSpace() const {return colorSpace.getColorSpace();};

        void setName(const std::string& _name);
        const std::string& getName() const {return name.getName();};

        deActionFrame* createLayerFrame(wxWindow* parent, int layerNumber, deProject* project);
        void closeActionFrame();
        void setActionFrame(deActionFrame* frame);
        deActionFrame* getActionFrame();

        virtual const std::string getDescription() const {return "base layer";};

        int getSourceLayerID() const {return sourceLayer.getIndex();};

        void registerProperty(deProperty& property);

        deLayer* getSourceLayer();
        
        deLayerStack& getStack() {return stack;};
        int getIndex() const {return index;};

        virtual std::string getType() const = 0;

        void save(xmlNodePtr node);
        void load(xmlNodePtr node);
        
        virtual void saveSpecific(xmlNodePtr node) {};
        virtual void loadSpecific(xmlNodePtr node) {};

        virtual void traceSampler(deSampler* sampler);

};

#endif

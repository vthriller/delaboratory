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

#ifndef _DE_LAYER_STACK_H
#define _DE_LAYER_STACK_H

#include <vector>
#include <libxml/parser.h>

class deLayer;
class deSampler;
class deLayerFactory;

class deLayerStack
{
    private:
        std::vector<deLayer*> layers;

        void loadLayer(xmlNodePtr node, deLayerFactory& factory);
        
    public:
        deLayerStack();
        virtual ~deLayerStack();

        deLayer* getLayer(int id) const;
        void addLayer(deLayer* layer);
        void removeTopLayer();

        int getSize() const;

        void traceSampler(deSampler* sampler);
        void updateColorSpaces();

        void save(xmlNodePtr node);
        void load(xmlNodePtr node, deLayerFactory& factory);

        void clear();
    
        void generateLayerUsage(std::vector<int>& layerUsage);
        
        void onKey(int key);

};

#endif

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
#include <map>
#include "color_space.h"
#include <vector>
#include "layer_with_blending.h"

class deProject;
class deFrame;
class deBlendFrame;
class deImage;
class dePropertyValue;
class deProperty;
class dePresetLayer;

class deLayer:public deLayerWithBlending
{
    private:
        deLayer(const deLayer& layer);
        deLayer& operator = (const deLayer& layer);

    protected:
        std::vector<deProperty*> properties;
        std::map<std::string, dePresetLayer*> presets;

        int registerPropertyValue(const std::string& _name);

        void saveValueProperties(xmlNodePtr node);
        void loadValueProperties(xmlNodePtr root);

    public:
        deLayer(deColorSpace _colorSpace, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager);
        virtual ~deLayer();

        virtual std::string getLabel() const;

        int getNumberOfProperties() const {return properties.size();};
        dePropertyValue* getPropertyValue(int index);
        dePropertyValue* getPropertyValue(const std::string& _name);

        bool applyPreset(const std::string& _name);
        void getPresets(std::vector<std::string>& result);


};

#endif

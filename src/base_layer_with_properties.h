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

#ifndef _DE_BASE_LAYER_WITH_PROPERTIES_H
#define _DE_BASE_LAYER_WITH_PROPERTIES_H

#include "base_layer.h"

class deBaseLayerWithProperties:public deBaseLayer
{
    private:
        deBaseLayerWithProperties(const deBaseLayerWithProperties& layer);
        deBaseLayerWithProperties& operator = (const deBaseLayerWithProperties& layer);

    protected:
        std::vector<deProperty*> properties;
        std::vector<dePreset*> presets;

        void createPropertyNumeric(const std::string& _name, deValue _min, deValue _max);
        void createPropertyChoice(const std::string& _name, const std::vector<std::string>& _choices);
        void createPropertyBoolean(const std::string& _name);

    public:
        deBaseLayerWithProperties(deColorSpace _colorSpace, deChannelManager& _channelManager);
        virtual ~deBaseLayerWithProperties();

        deProperty* getProperty(const std::string& _name);
        const deProperty* getProperty(const std::string& _name) const;

        dePropertyNumeric* getPropertyNumeric(const std::string& _name);
        const dePropertyNumeric* getPropertyNumeric(const std::string& _name) const;

        dePropertyChoice* getPropertyChoice(const std::string& _name);
        const dePropertyChoice* getPropertyChoice(const std::string& _name) const;

        dePropertyBoolean* getPropertyBoolean(const std::string& _name);
        const dePropertyBoolean* getPropertyBoolean(const std::string& _name) const;

        void getProperties(std::vector<std::string>& names);

        deValue getNumericValue(const std::string& name) const;

        void applyPreset(const std::string& name);
        void getPresets(std::vector<std::string>& names);
        dePreset* createPreset(const std::string& name);

        virtual void executeOperation(const std::string& o) {};

};

#endif

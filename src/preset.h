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

#ifndef _DE_PRESET_H
#define _DE_PRESET_H

#include <string>
#include "value.h"
#include "layer.h"
#include "property_value.h"

class dePreset
{
    private:
        dePreset(const dePreset&);
        dePreset& operator =(const dePreset&);

    protected:
        const std::string name;

    public:
        dePreset(const std::string& _name)
        :name(_name)
        {
        }

        virtual ~dePreset()
        {
        }

        std::string getName() {return name;};

};

class dePresetValue:public dePreset
{
    private:
        deValue value;

    public:
        dePresetValue(const std::string& _name, deValue _value)
        :dePreset(_name),
         value(_value)
        {
        }

        virtual ~dePresetValue()
        {
        }

        deValue getValue() {return value;};


};

class dePresetLayer
{
    private:
        deLayer& layer;
        std::vector<dePresetValue*> valuePresets;

        dePresetLayer(const dePresetLayer&);
        dePresetLayer& operator =(const dePresetLayer&);

    public:
        dePresetLayer(deLayer& _layer)
        :layer(_layer)
        {
        }

        virtual ~dePresetLayer()
        {
        }

        void addPresetValue(dePresetValue* preset)
        {
            valuePresets.push_back(preset);
        }

        bool apply()
        {
            std::vector<dePresetValue*>::iterator i;
            for (i = valuePresets.begin(); i != valuePresets.end(); i++)
            {
                dePresetValue* pv = *i;
                const std::string n = pv->getName();
                const deValue v = pv->getValue();

                dePropertyValue* property = layer.getPropertyValue(n);
                property->set(v);

            }

            return true;
        }
};


#endif

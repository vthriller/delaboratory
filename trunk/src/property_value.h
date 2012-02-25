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

#ifndef _DE_PROPERTY_VALUE_H
#define _DE_PROPERTY_VALUE_H

#include "property.h"
#include "value.h"

class dePropertyValue:public deProperty
{
    private:
        deValue value;

        deValue min;
        deValue max;
        deValue defaultValue;

        int channel;
    public:
        dePropertyValue(const std::string& _name);
        virtual ~dePropertyValue();

        void set(deValue _value);
        deValue get() const;

        virtual void save(xmlNodePtr root) const;
        virtual void load(xmlNodePtr root);

        deValue getMin() const;
        deValue getMax() const;
        deValue getDefault() const;

        void setMin(deValue _min);
        void setMax(deValue _max);

        void setChannel(int _channel);
        int getChannel() const {return channel;};
};

#endif

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

#ifndef _DE_PROPERTY_LEVELS_H
#define _DE_PROPERTY_LEVELS_H

#include "property.h"
#include "value.h"

class deLevels
{
    private:
        deValue min;
        deValue middle;
        deValue max;

        void sort();

    public:        
        deLevels();
        virtual ~deLevels();

        void setMin(deValue _value);
        deValue getMin() const;
        void setMiddle(deValue _value);
        deValue getMiddle() const;
        void setMax(deValue _value);
        deValue getMax() const;

        bool isNeutral() const;
};

class dePropertyLevels:public deProperty
{
    private:
        deLevels levels;
        int channel;

    public:
        dePropertyLevels(const std::string& _name, int _channel);
        virtual ~dePropertyLevels();

        int getChannel() const;

        const deLevels& getLevels() const {return levels;};
        deLevels& getLevels() {return levels;};

        virtual void save(xmlNodePtr root) const;
        virtual void load(xmlNodePtr root);
};

#endif

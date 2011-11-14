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

#ifndef _DE_PROPERTY_CHOICE_H
#define _DE_PROPERTY_CHOICE_H

#include "property.h"
#include <vector>

class dePropertyChoice:public deProperty
{
    private:
        std::string value;

        std::vector<std::string> choices;
    public:
        dePropertyChoice(const std::string& _name);
        virtual ~dePropertyChoice();

        void set(std::string _value);
        std::string get() const;

        virtual void save(xmlNodePtr root) const;
        virtual void load(xmlNodePtr root);

        std::vector<std::string>& getChoices() {return choices;};

        void setIndex(int index);
        int getIndex() const;
        
};

#endif

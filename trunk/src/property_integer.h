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

#ifndef _DE_PROPERTY_INTEGER_H
#define _DE_PROPERTY_INTEGER_H

#include "property.h"
#include "value.h"
#include <string>

class dePropertyInteger:public deProperty
{
    private:
        int value;
        int min;
        int max;
        std::string label;

    public:
        dePropertyInteger(deLayer& _parent, const std::string& label, int _min, int _max);
        virtual ~dePropertyInteger();

        int getValue() const {return value;};
        void setValue(int _value);

        virtual void addPanelContent(wxPanel* panel, wxSizer* sizer);

        const std::string getLabel() const {return label;};

        int getMin() const {return min;};
        int getMax() const {return max;};

        virtual void saveContent(xmlNodePtr node);
        virtual void load(xmlNodePtr node);

};

#endif

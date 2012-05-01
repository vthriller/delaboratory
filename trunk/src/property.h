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

#ifndef _DE_PROPERTY_H
#define _DE_PROPERTY_H

#include <string>
#include <libxml/parser.h>

class deProperty
{
    private:
        std::string label;
        bool blendOnly;

    protected:        
        std::string name;

    public:
        deProperty(const std::string& _name);
        virtual ~deProperty();

        virtual void save(xmlNodePtr root) const = 0;
        virtual void load(xmlNodePtr root) = 0;

        std::string getLabel() const;
        void setLabel(const std::string& _label);

        std::string getName() const;

        virtual bool updateBlendOnly() const {return blendOnly;};
        void setBlendOnly();
};

#endif

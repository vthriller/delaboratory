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

#ifndef _DE_ND_H
#define _DE_ND_H

#include "value.h"
#include "nd_type.h"

class deND
{
    private:
        deNDType type;
        deValue xCenter;
        deValue yCenter;
        deValue power;
        bool inverted;

    public:
        deND();
        virtual ~deND();

        deValue getValue(deValue x, deValue y) const;

        deValue getXCenter() const;
        deValue getYCenter() const;
        deValue getPower() const;
        deNDType getType() const;
        void setXCenter(deValue v);
        void setYCenter(deValue v);
        void setPower(deValue v);
        void setType(deNDType _type);

};

#endif

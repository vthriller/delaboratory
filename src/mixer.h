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

#ifndef _DE_MIXER_H
#define _DE_MIXER_H

#include "color_space.h"
#include "value.h"

class dePreview;

class deMixer
{
    private:
        deColorSpace sourceColorSpace;
        deColorSpace destinationColorSpace;

        deValue* values;

        deValue range;

    public:
        deMixer(deColorSpace _sourceColorSpace, deColorSpace _destinationColorSpace);
        virtual ~deMixer();

        void calc(const dePreview& source, dePreview& destination) const;

        void reset();

        deColorSpace getSourceColorSpace() const;
        deColorSpace getDestinationColorSpace() const;

        deValue getValue(int s, int d) const;
        void setValue(int s, int d, deValue value);

        deValue getRangeMin() const;
        deValue getRangeMax() const;
    
};

#endif

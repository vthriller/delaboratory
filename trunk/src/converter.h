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

#ifndef _DE_CONVERTER_H
#define _DE_CONVERTER_H

class dePreview;
#include "size.h"
#include "color_space.h"

class deConverter
{
    private:
        const dePreview* source;
        dePreview* destination;
        deSize size;
        int n;
        int index;
        int dataPosition;
        unsigned char* data;
        deColorSpace sourceColorSpace;
        deColorSpace destinationColorSpace;
        deValue values[8];

        void storeRGB();

        void (*conversionFunction)(deValue* values);

    public:
        deConverter();
        virtual ~deConverter();

        void setSource(const dePreview* _source);
        void setDestination(dePreview* _destination);

        void storeSingleChannel(int channel);
        void storeAllChannels();

        void setData(unsigned char* _data);

        bool getPixel(int pos, deColorSpace colorspace, deValue& v0, deValue& v1, deValue& v2, deValue& v3);

        bool setConversionFunction();

        bool convert();
};

#endif

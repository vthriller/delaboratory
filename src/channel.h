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

#ifndef _DE_CHANNEL_H
#define _DE_CHANNEL_H

#include "base_channel.h"

class deTrueChannel:public deBaseChannel
{
    private:
        deValue* pixels;
        deValue min;
        deValue max;

    public:
        deTrueChannel(const deSize& _size, deValue _min, deValue _max);
        virtual ~deTrueChannel();

        virtual bool copy(const deBaseChannel* channel);
        bool scale(const deTrueChannel* channel);

        virtual deValue getValue(int pos) const;
        virtual void setValue(int pos, deValue value);

        const deValue* getPixels() const;
};

class deChannel
{
    private:
        deValue* pixels;
    public:
        deChannel(int size)
        {
            pixels = new deValue [size];
        }

        virtual ~deChannel()
        {
            delete pixels;
        }

        deValue* getPixels()
        {
            return pixels;
        }
};

#endif

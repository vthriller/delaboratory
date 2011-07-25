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

#ifndef _DE_PREVIEW_H
#define _DE_PREVIEW_H

#include "size.h"
#include "color_space.h"

class deBaseChannel;

class dePreview
{
    private:
        dePreview(const dePreview& preview);

    protected:
        const deColorSpace colorSpace;
        deSize size;
        deBaseChannel* channels[4];

        void createChannels(const deSize& _size);
        void destroyChannels();

        dePreview(const deColorSpace& colorSpace);

    public:
        dePreview(const deColorSpace& colorSpace, const deSize& _size);
        ~dePreview();

        const deSize& getSize() const {return size;}; 

        void setDirty();
        void clearDirty();

        virtual deBaseChannel* getChannel(int id);
        virtual const deBaseChannel* getChannel(int id) const;

        const deColorSpace& getColorSpace() const;
};

#endif

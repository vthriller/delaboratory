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

#ifndef _DE_STATIC_IMAGE_H
#define _DE_STATIC_IMAGE_H

#include "color_space.h"
class deChannel;

class deStaticImage
{
    private:
        const deColorSpace colorSpace;
        std::vector<deChannel*> channels;
        
        deStaticImage(const deStaticImage& i);
        deStaticImage& operator = (const deStaticImage& i);


    public:
        deStaticImage(const deColorSpace& _colorSpace);

        virtual ~deStaticImage();

        deColorSpace getColorSpace() const;
};

#endif

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

#ifndef _DE_DCRAW_SUPPORT_H
#define _DE_DCRAW_SUPPORT_H

#include <string>
class deStaticImage;
#include "color_space.h"

std::string getDcrawVersion();
std::string getRawInfo(const std::string& f);
bool isRawValid(const std::string& f);
class wxProcess;
class wxInputStream;

class deRawLoader
{
    private:
        wxProcess* process;
        wxInputStream* input;
        const std::string& filename;
        deStaticImage& image;
        deColorSpace colorSpace;
        bool half;
    public:        
        
        deRawLoader(const std::string& f, deStaticImage& image, deColorSpace colorSpace, bool half);
        virtual ~deRawLoader();

        bool load(bool& failure);
        bool getStatus();

};

#endif

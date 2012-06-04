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

#ifndef _DE_LOAD_TIFF_H
#define _DE_LOAD_TIFF_H

class deStaticImage;
#include <string>
#include "value.h"
#include "size.h"

bool loadTIFF(const std::string& fileName, deStaticImage& image);
void saveTIFF(const std::string& fileName, const deValue* channelR, const deValue* channelG, const deValue* channelB, deSize size);

#endif

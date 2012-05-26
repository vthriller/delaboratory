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

#ifndef _DE_BLEND_COLOR_LUMINOSITY_H
#define _DE_BLEND_COLOR_LUMINOSITY_H

#include "value.h"

void blendColorRGB(const deValue* source0, const deValue* source1, const deValue* source2, const deValue* overlay0, const deValue* overlay1, const deValue* overlay2, deValue* destination0, deValue* destination1, deValue* destination2, int n, deValue o);

#endif

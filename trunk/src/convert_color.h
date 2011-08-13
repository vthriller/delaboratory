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

#ifndef _DE_COLOR_CONVERSIONS_H
#define _DE_COLOR_CONVERSIONS_H

#include "value.h"

void simpleCopy(deValue* values);
void rgb2cmy(deValue* values);
void cmy2cmyk(deValue* values);
void cmyk2cmy(deValue* values);
void cmy2rgb(deValue* values);
void rgb2cmyk(deValue* values);
void cmyk2rgb(deValue* values);
void rgb2xyz(deValue* values);
void xyz2rgb(deValue* values);
void lab2xyz(deValue* values);
void xyz2lab(deValue* values);
void lab2rgb(deValue* values);
void rgb2lab(deValue* values);
void rgb2hsl(deValue* values);
void hsl2rgb(deValue* values);
void rgb2hsv(deValue* values);
void hsv2rgb(deValue* values);
void rgb2bw(deValue* values);
void bw2rgb(deValue* values);
void lab2cmyk(deValue* values);
void cmyk2lab(deValue* values);
void lab2bw(deValue* values);
void bw2lab(deValue* values);
void lab2hsv(deValue* values);
void hsv2lab(deValue* values);
void lab2hsl(deValue* values);
void hsl2lab(deValue* values);


#endif

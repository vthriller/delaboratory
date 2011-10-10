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

#ifndef _DE_HSL_HSV_H
#define _DE_HSL_HSV_H

#include "value.h"

void rgb2hsl(deValue r, deValue g, deValue b, deValue& h, deValue& s, deValue& l);
void hsl2rgb(deValue h, deValue s, deValue l, deValue& r, deValue& g, deValue& b);
void rgb2hsv(deValue r, deValue g, deValue b, deValue& h, deValue& s, deValue& v);
void hsv2rgb(deValue h, deValue s, deValue v, deValue& r, deValue& g, deValue& b);

#endif

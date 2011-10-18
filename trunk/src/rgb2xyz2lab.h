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

#ifndef _DE_RGB_2_XYZ_2_LAB_H
#define _DE_RGB_2_XYZ_2_LAB_H

#include "value.h"

void rgb2xyz(deValue r, deValue g, deValue b, deValue &x, deValue &y, deValue& z);
void xyz2rgb(deValue x, deValue y, deValue z, deValue &r, deValue &g, deValue& b);
void xyz2lab(deValue x, deValue y, deValue z, deValue &l, deValue &a, deValue& b);
void lab2xyz(deValue l, deValue a, deValue b, deValue &x, deValue &y, deValue& z);
void lab2lch(deValue l, deValue a, deValue b, deValue &_l, deValue &_c, deValue& _h);
void lch2lab(deValue l, deValue c, deValue h, deValue &_l, deValue &_a, deValue& _b);

/* for benchmarks only */
void initLAB();

#endif

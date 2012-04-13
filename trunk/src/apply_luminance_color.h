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

#ifndef _DE_APPLY_LUMINANCE_COLOR_H
#define _DE_APPLY_LUMINANCE_COLOR_H

#include "value.h"

void applyLuminanceRGB(deValue r1, deValue g1, deValue b1, deValue r2, deValue g2, deValue b2, deValue &r, deValue &g, deValue &b);
void applyLuminanceProPhoto(deValue r1, deValue g1, deValue b1, deValue r2, deValue g2, deValue b2, deValue &r, deValue &g, deValue &b);
void applyLuminanceCMYK(deValue c1, deValue m1, deValue y1, deValue k1, deValue c2, deValue m2, deValue y2, deValue k2, deValue &c, deValue &m, deValue &y, deValue& k);

void applyColor(deValue r1, deValue g1, deValue b1, deValue r2, deValue g2, deValue b2, deValue &r, deValue &g, deValue &b);

#endif

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

#ifndef _DE_CONVERSION_FUNCTIONS_H
#define _DE_CONVERSION_FUNCTIONS_H

#include "value.h"
#include "color_space.h"

typedef void (*deConversion3x1)(deValue s1, deValue s2, deValue s3, deValue& d1);
typedef void (*deConversion1x3)(deValue s1, deValue& d1, deValue& d2, deValue& d3);
typedef void (*deConversion1x4)(deValue s1, deValue& d1, deValue& d2, deValue& d3, deValue& d4);
typedef void (*deConversion3x3)(deValue s1, deValue s2, deValue s3, deValue& d1, deValue& d2, deValue& d3);
typedef void (*deConversion4x3)(deValue s0, deValue s1, deValue s2, deValue s3, deValue& d1, deValue& d2, deValue& d3);
typedef void (*deConversion3x4)(deValue s1, deValue s2, deValue s3, deValue& d1, deValue& d2, deValue& d3, deValue& d4);

deConversion1x3 getConversion1x3(deColorSpace s, deColorSpace d);
deConversion1x4 getConversion1x4(deColorSpace s, deColorSpace d);
deConversion3x1 getConversion3x1(deColorSpace s, deColorSpace d);
deConversion3x3 getConversion3x3(deColorSpace s, deColorSpace d);
deConversion3x4 getConversion3x4(deColorSpace s, deColorSpace d);
deConversion4x3 getConversion4x3(deColorSpace s, deColorSpace d);

bool checkConversion(deColorSpace currentColorSpace, deColorSpace colorSpace);


#endif

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

#include "blur_type.h"

std::string getBlurTypeName(deBlurType type)
{
    switch (type)
    {
        case deBoxBlur:
            return "box blur";
        case deGaussianBlur:
            return "gaussian blur";
        case deSurfaceBlur:
            return "surface blur";
        default:
            return "unknown";
    }
}

deBlurType blurTypeFromString(const std::string& s)
{
    if (s == "box blur")
    {
        return deBoxBlur;
    }
    if (s == "gaussian blur")
    {
        return deGaussianBlur;
    }
    if (s == "surface blur")
    {
        return deSurfaceBlur;
    }

    return deBlurInvalid;
}

void getSupportedBlurTypes(std::vector<deBlurType>& result)
{
    result.push_back(deBoxBlur);
    result.push_back(deGaussianBlur);
    result.push_back(deSurfaceBlur);
}

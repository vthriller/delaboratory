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

#include "blend_mode.h"

std::string getBlendModeName(deBlendMode mode)
{
    switch (mode)
    {
        case deBlendNormal:
            return "normal";
        case deBlendMultiply:
            return "multiply";
        case deBlendScreen:
            return "screen";
        case deBlendOverlay:
            return "overlay";
        default:
            return "unknown";
    }
}

void getSupportedBlendModes(std::vector<deBlendMode>& result)
{
    result.push_back(deBlendNormal);
    result.push_back(deBlendMultiply);
    result.push_back(deBlendScreen);
    result.push_back(deBlendOverlay);
}

deValue calcBlendResult(deValue src, deValue v2, deBlendMode mode)
{
    switch (mode)
    {
        case deBlendNormal:
            return v2;
            break;
        case deBlendMultiply:                    
            return src*v2;
            break;
        case deBlendScreen:
            return 1 - (1-src)*(1-v2);
            break;
        case deBlendOverlay:                    
            if (src > 0.5)
            {
                return 1 - (1 - 2 * ( src - 0.5)) * (1 - v2);
            }
            else
            {
                return 2 * src * v2;
            }
            break;
        default:
            return 0;
    }                    
}        


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

#include "apply_mode.h"

/*
#include <cmath>

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
        case deBlendOverlayInvert:
            return "overlay invert";
        case deBlendAdd:
            return "add";
        case deBlendAddInvert:
            return "add invert";
        case deBlendSub:
            return "sub";
        case deBlendDifference:
            return "difference";
        case deBlendDarken:
            return "darken";
        case deBlendLighten:
            return "lighten";
        default:
            return "unknown";
    }
}

deBlendMode blendModeFromString(const std::string& s)
{
    if (s == "normal")
    {
        return deBlendNormal;
    }

    if (s == "multiply")
    {
        return deBlendMultiply;
    }

    if (s == "screen")
    {
        return deBlendScreen;
    }

    if (s == "overlay")
    {
        return deBlendOverlay;
    }

    if (s == "overlay invert")
    {
        return deBlendOverlayInvert;
    }

    if (s == "add")
    {
        return deBlendAdd;
    }

    if (s == "add invert")
    {
        return deBlendAddInvert;
    }

    if (s == "sub")
    {
        return deBlendSub;
    }

    if (s == "difference")
    {
        return deBlendDifference;
    }

    if (s == "darken")
    {
        return deBlendDarken;
    }

    if (s == "lighten")
    {
        return deBlendLighten;
    }
    
    return deBlendInvalid;

}

void getSupportedBlendModes(std::vector<deBlendMode>& result)
{
    result.push_back(deBlendNormal);
    result.push_back(deBlendMultiply);
    result.push_back(deBlendScreen);
    result.push_back(deBlendOverlay);
    result.push_back(deBlendOverlayInvert);
    result.push_back(deBlendAdd);
    result.push_back(deBlendAddInvert);
    result.push_back(deBlendSub);
    result.push_back(deBlendDifference);
    result.push_back(deBlendDarken);
    result.push_back(deBlendLighten);
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
        case deBlendOverlayInvert:                    
            if (src > 0.5)
            {
                return 1 - (1 - 2 * ( src - 0.5)) * v2;
            }
            else
            {
                return 2 * src * (1 - v2);
            }
            break;
        case deBlendAdd:                    
            return src + v2;
            break;
        case deBlendAddInvert:                    
            return 0.5 + src - v2;
            break;
        case deBlendSub:                    
            return src - v2;
            break;
        case deBlendDifference:                    
            return fabs(src - v2);
            break;
        case deBlendDarken:                    
            if (src < v2)
            {
                return src;
            }
            else
            {
                return v2;
            }                
            break;
        case deBlendLighten:                    
            if (src > v2)
            {
                return src;
            }
            else
            {
                return v2;
            }                
            break;
        default:
            return 0;
    }                    
}        
*/

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

#include "convert_pixel.h"
#include "image.h"
#include "conversion_functions.h"
#include "channel_manager.h"


bool convertPixel(const deImage& image, int p, deColorSpace colorSpace, deValue &v1, deValue& v2, deValue& v3, deValue& v4)
{
    deColorSpace sourceColorSpace = image.getColorSpace();
    int sn = getColorSpaceSize(sourceColorSpace);
    int tn = getColorSpaceSize(colorSpace);

    if ((sn == 3) && (tn == 3))
    {
        deConversion3x3 conversion = getConversion3x3(sourceColorSpace, colorSpace);

        if (conversion)
        {
            deValue s1;
            if (!image.getPixel(0, p, s1))
            {
                return false;
            }
            deValue s2;
            if (!image.getPixel(1, p, s2))
            {
                return false;
            }
            deValue s3;
            if (!image.getPixel(2, p, s3))
            {
                return false;
            }
            conversion(s1, s2, s3, v1, v2, v3);
            v4 = -1;
            return true;
        } 
    }        

    if ((sn == 3) && (tn == 4))
    {
        deConversion3x4 conversion = getConversion3x4(sourceColorSpace, colorSpace);

        if (conversion)
        {
            deValue s1;
            if (!image.getPixel(0, p, s1))
            {
                return false;
            }
            deValue s2;
            if (!image.getPixel(1, p, s2))
            {
                return false;
            }
            deValue s3;
            if (!image.getPixel(2, p, s3))
            {
                return false;
            }
            conversion(s1, s2, s3, v1, v2, v3, v4);
            return true;
        } 
    }        
}


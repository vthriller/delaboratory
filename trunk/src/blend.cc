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


#include "blend.h"
#include <iostream>
#include "preview.h"
#include "color_space.h"
#include "channel.h"

void blend(const dePreview& sourcePreview, const dePreview& overlayPreview, deValue alpha, dePreview& resultPreview, int overlayChannelID, int destinationChannelID, deBlendMode mode)
{
    deColorSpace rc = resultPreview.getColorSpace();

    const deSize& size = resultPreview.getSize();

    int n = size.getN();
    int nc = getColorSpaceSize(rc);

    int first = 0;
    int last = nc - 1;
    if (destinationChannelID >= 0)
    {
        first = destinationChannelID;
        last = destinationChannelID;
        int i;
        for (i = 0; i < nc; i++)
        {
            if (i != destinationChannelID)
            {
                const deChannel* sourceChannel = dynamic_cast<const deChannel*>(sourcePreview.getChannel(i));
                deChannel* resultChannel = dynamic_cast<deChannel*>(resultPreview.getChannel(i));
                resultChannel->copy(sourceChannel);
            }
        }
    }

    int i;
    for (i = first; i <= last; i++)
    {
        const deBaseChannel* sourceChannel = sourcePreview.getChannel(i);
        const deBaseChannel* overlayChannel;
        
        if (overlayChannelID < 0)
        {
            overlayChannel = overlayPreview.getChannel(i);
        }
        else
        {
            overlayChannel = overlayPreview.getChannel(overlayChannelID);
        }            
        deBaseChannel* resultChannel = resultPreview.getChannel(i);

        int j;
        for (j = 0; j < n; j++)
        {
            deValue src = sourceChannel->getValue(j);
            deValue v2 = overlayChannel->getValue(j);

            deValue dst;
            switch (mode)
            {
                case deBlendNormal:
                    dst = v2;
                    break;
                case deBlendMultiply:                    
                    dst = src*v2;
                    break;
                case deBlendScreen:
                    dst = 1 - (1-src)*(1-v2);
                    break;
                case deBlendOverlay:                    
                    if (src > 0.5)
                    {
                        dst = 1 - (1 - 2 * ( src - 0.5)) * (1 - v2);
                    }
                    else
                    {
                        dst = 2 * src * v2;
                    }
                    break;
                default:
                    dst = 0;
            }                    
//            deValue dst = 1 - (1-src)*(1-v2)

            deValue r = (1 - alpha) * src + alpha * dst;
            resultChannel->setValue(j, r);
        }
    }
}

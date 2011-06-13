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
#include "nd.h"

void blend(const deBaseChannel& sourceChannel, const deBaseChannel& overlayChannel, deBaseChannel& resultChannel, deValue alpha, deBlendMode mode)
{
    int w = sourceChannel.getSize().getW();
    int h = sourceChannel.getSize().getH();
    int pos = 0;
    int y;
    for (y = 0; y < h; y++)
    {
        int x;
        for (x = 0; x < w; x++)
        {
            deValue src = sourceChannel.getValue(pos);
            deValue v2 = overlayChannel.getValue(pos);

            deValue dst = calcBlendResult(src, v2, mode);

            deValue r = (1 - alpha) * src + alpha * dst;
            resultChannel.setValue(pos, r);
            pos ++;
        }
    }        
}        

void blend(const deBaseChannel& sourceChannel, const deND& nd, deBaseChannel& resultChannel, deValue alpha, deBlendMode mode)
{
    int w = sourceChannel.getSize().getW();
    int h = sourceChannel.getSize().getH();
    deValue stepX = 1.0 / w;
    deValue stepY = 1.0 / h;
    int pos = 0;
    int y;
    deValue yy = -0.5;
    for (y = 0; y < h; y++)
    {
        int x;
        deValue xx = -0.5;
        for (x = 0; x < w; x++)
        {
            deValue src = sourceChannel.getValue(pos);
            deValue v2 = nd.getValue(xx, yy);

            deValue dst = calcBlendResult(src, v2, mode);

            deValue r = (1 - alpha) * src + alpha * dst;
            resultChannel.setValue(pos, r);
            pos ++;
            xx += stepX;
        }
        yy += stepY;
    }        
}        

void blend(const dePreview& sourcePreview, const dePreview& overlayPreview, deValue alpha, dePreview& resultPreview, int overlayChannelID, const deChannels& enabledChannels, deBlendMode mode)
{

/*
    deColorSpace rc = resultPreview.getColorSpace();

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

        blend(*sourceChannel, *overlayChannel, *resultChannel, alpha, mode);
    }*/



    deColorSpace rc = resultPreview.getColorSpace();
    int nc = getColorSpaceSize(rc);

    //deValue alpha = 1.0;

    int i;
    for (i = 0; i < nc; i++)
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
        if (enabledChannels.count(i) == 1)
        {
            blend(*sourceChannel, *overlayChannel, *resultChannel, alpha, mode);
        }
        else
        {
            resultChannel->copy(sourceChannel);
        }

    }
}

void blend(const dePreview& sourcePreview, const deND& nd, dePreview& resultPreview, deBlendMode mode, const deChannels& enabledChannels)
{
    deColorSpace rc = resultPreview.getColorSpace();
    int nc = getColorSpaceSize(rc);

    deValue alpha = 1.0;

    int i;
    for (i = 0; i < nc; i++)
    {
        const deBaseChannel* sourceChannel = sourcePreview.getChannel(i);
        deBaseChannel* resultChannel = resultPreview.getChannel(i);
        if (enabledChannels.count(i) == 1)
        {
            blend(*sourceChannel, nd, *resultChannel, alpha, mode);
        }
        else
        {
            resultChannel->copy(sourceChannel);
        }

    }
}

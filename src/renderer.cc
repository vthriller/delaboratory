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

#include "renderer.h"
#include <wx/wx.h>
#include "project.h"
#include "image.h"
#include "layer.h"
#include <iostream>
#include "conversion_functions.h"
#include "layer_processor.h"
#include "str.h"
#include "logger.h"
#include "channel_manager.h"
#include "layer_stack.h"
#include "canvas_wx.h"

bool renderImage(const deImage& image, unsigned char* data, deChannelManager& channelManager)
{
    deColorSpace colorSpace = image.getColorSpace();

    deConversion3x3 conversion3x3 = getConversion3x3(colorSpace, deColorSpaceRGB);
    deConversion4x3 conversion4x3 = NULL;
    deConversion1x3 conversion1x3 = NULL;
    if (!conversion3x3)
    {
        conversion4x3 = getConversion4x3(colorSpace, deColorSpaceRGB);
        if (!conversion4x3)
        {
            conversion1x3 = getConversion1x3(colorSpace, deColorSpaceRGB);
            if (!conversion1x3)
            {
                logError("no conversion found");
                return false;
            }            
        }            
    }

    const deSize& s = channelManager.getChannelSize();

    deChannel* channel0 = channelManager.getChannel(image.getChannelIndex(0));
    deChannel* channel1 = channelManager.getChannel(image.getChannelIndex(1));
    deChannel* channel2 = channelManager.getChannel(image.getChannelIndex(2));
    deChannel* channel3 = channelManager.getChannel(image.getChannelIndex(3));

    if (conversion4x3)
    {
        if (!channel0)
        {
            return false;
        }
        if (!channel1)
        {
            return false;
        }
        if (!channel2)
        {
            return false;
        }
        if (!channel3)
        {
            return false;
        }
        channel0->lockRead();
        channel1->lockRead();
        channel2->lockRead();
        channel3->lockRead();
    }        
    else if (conversion1x3)
    {
        if (!channel0)
        {
            return false;
        }
        channel0->lockRead();
    }        
    else
    {
        if (!channel0)
        {
            logError("no channel 0");
            return false;
        }
        if (!channel1)
        {
            logError("no channel 1");
            return false;
        }
        if (!channel2)
        {
            logError("no channel 2");
            return false;
        }
        channel0->lockRead();
        channel1->lockRead();
        channel2->lockRead();
    }        

    deValue rr;
    deValue gg;
    deValue bb;

    int n = s.getN();
    int i;
    int pos = 0;

    if (conversion4x3)
    {
        const deValue* p0 = channel0->getPixels();
        const deValue* p1 = channel1->getPixels();
        const deValue* p2 = channel2->getPixels();
        const deValue* p3 = channel3->getPixels();
        for (i = 0; i < n; i++)
        {
            deValue s0 = p0[i];
            deValue s1 = p1[i];
            deValue s2 = p2[i];
            deValue s3 = p3[i];
            conversion4x3(s0, s1, s2, s3, rr, gg, bb);

            unsigned char r = 255 * rr;
            data[pos] = r;
            pos++;
            unsigned char g = 255 * gg;
            data[pos] = g;
            pos++;
            unsigned char b = 255 * bb;
            data[pos] = b;
            pos++;
        }

        channel0->unlockRead();
        channel1->unlockRead();
        channel2->unlockRead();
        channel3->unlockRead();

    }
    else if (conversion1x3)
    {
        const deValue* p0 = channel0->getPixels();
        for (i = 0; i < n; i++)
        {
            deValue s0 = p0[i];
            conversion1x3(s0, rr, gg, bb);

            unsigned char r = 255 * rr;
            data[pos] = r;
            pos++;
            unsigned char g = 255 * gg;
            data[pos] = g;
            pos++;
            unsigned char b = 255 * bb;
            data[pos] = b;
            pos++;
        }
        channel0->unlockRead();
    }
    else
    {
        const deValue* p0 = channel0->getPixels();
        const deValue* p1 = channel1->getPixels();
        const deValue* p2 = channel2->getPixels();
        for (i = 0; i < n; i++)
        {

            deValue s1 = p0[i];
            deValue s2 = p1[i];
            deValue s3 = p2[i];
            conversion3x3(s1, s2, s3, rr, gg, bb);

            unsigned char r = 255 * rr;
            data[pos] = r;
            pos++;
            unsigned char g = 255 * gg;
            data[pos] = g;
            pos++;
            unsigned char b = 255 * bb;
            data[pos] = b;
            pos++;

        }
        channel0->unlockRead();
        channel1->unlockRead();
        channel2->unlockRead();
    }

    return true;

}

void renderChannel(const deImage& image, int c, unsigned char* data, deChannelManager& channelManager, bool reversed)
{
    const deSize& s = channelManager.getChannelSize();

    deChannel* channel = channelManager.getChannel(image.getChannelIndex(c));

    if (!channel)
    {
        return;
    }

    channel->lockRead();

    const deValue* pixels = channel->getPixels();

    int n = s.getN();
    int i;
    int pos = 0;
    for (i = 0; i < n; i++)
    {
        deValue s = pixels[i];

        if (reversed)
        {
            s = 1.0 - s;
        }

        unsigned char ss = 255 * s;

        data[pos] = ss;
        pos++;
        data[pos] = ss;
        pos++;
        data[pos] = ss;
        pos++;
    }      

    channel->unlockRead();

}

deRenderer::deRenderer(deChannelManager& _channelManager)
:size(0,0),
 channelManager(_channelManager)
{
}

deRenderer::~deRenderer()
{
}

bool deRenderer::prepareImage(const deViewManager& viewManager, deLayerProcessor& layerProcessor, deLayerStack& layerStack)
{
    const deSize& s = channelManager.getChannelSize();

    if ((s.getW() == 0) || (s.getH() == 0))
    {
        return false;
    }

    lockWithLog(mutex, "renderer mutex");

    int viewV = viewManager.getView();
    int view = layerProcessor.getLastValidLayer();
    if (view > viewV)
    {
        view = viewV;
    }

    if (view < 0)
    {
        mutex.unlock();
        return false;
    }

    logMessage("renderer getLayer " +str(view));
    deBaseLayer* layer = layerStack.getLayer(view);
    if (!layer)
    {
        logError("no layer");
        logMessage("unlock renderer mutex");
        mutex.unlock();
        return false;
    }
    logMessage("renderer lock layer " +str(view));

    layer->lockLayer();

    logMessage("renderer get image from layer " +str(view));

    const deImage& layerImage = layer->getLayerImage();

    bool reversed = false;
    deColorSpace colorSpace = layerImage.getColorSpace();

    if ((colorSpace == deColorSpaceCMYK))
    {
        reversed = true;
    }

    logMessage("renderer before renderer");

    if (viewManager.isSingleChannel())
    {
        renderChannel(layerImage, viewManager.getChannel(), getCurrentImageData(), channelManager, reversed);
    }
    else
    {
        if (!renderImage(layerImage, getCurrentImageData(), channelManager))
        {
            std::cout << "failed renderImage" << std::endl;
            logMessage("render image FAILED");
        }
    }

    logMessage("renderer unlock layer " +str(view));

    layer->unlockLayer();

    logMessage("unlock renderer mutex");
    mutex.unlock();

    return true;
}

bool deRenderer::render(deCanvas& canvas)
{
    lockWithLog(mutex, "renderer mutex");

    bool result = renderedImage.render(canvas);

    if (!result)
    {
        canvas.clear();
    }

    logMessage("unlock renderer mutex");
    mutex.unlock();

    return result;
}

unsigned char* deRenderer::getCurrentImageData()
{
    const deSize& s = channelManager.getChannelSize();
    renderedImage.setSize(s);
    return renderedImage.getCurrentImageData();
}    

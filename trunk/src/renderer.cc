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
#include "layer_processor.h"
#include "str.h"
#include "logger.h"
#include "channel_manager.h"
#include "layer_stack.h"
#include "canvas_wx.h"
#include "conversion_processor.h"

void renderChannel(const deImage& image, int c, unsigned char* data, deChannelManager& channelManager, bool reversed)
{
    const deSize& s = image.getChannelSize();

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
    if (channelManager.isImageEmpty())
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
        deConversionProcessor p;
        //if (!p.renderImageToRGB(layerImage, getCurrentImageData(), channelManager))
        if (!p.renderImageToRGBNew(layerImage, getCurrentImageData()))
        {
            logMessage("render image FAILED");
            renderedImage.setError();
        }
        else
        {
            renderedImage.clearError();
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
    const deSize& s = channelManager.getChannelSizeFromChannelManager();
    renderedImage.setSize(s);
    return renderedImage.getCurrentImageData();
}    

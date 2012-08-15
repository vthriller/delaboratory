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
#include "project.h"
#include "image.h"
#include "base_layer.h"
#include <iostream>
#include "layer_processor.h"
#include "str.h"
#include "logger.h"
#include "channel_manager.h"
#include "layer_stack.h"
#include "conversion_processor.h"
#include "canvas.h"

void renderChannel(const deImageOld& image, int c, unsigned char* data, deChannelManager& channelManager, bool reversed)
{
    const deSize& s = image.getChannelSize();

    const deValue* pixels = image.startRead(c);

    if (!pixels)
    {
        logError("can't render channel - NULL pixels");
        return;
    }

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

    image.finishRead(c);

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
        logError("image is empty");
        return false;
    }

    mutex.lock();

    int viewV = viewManager.getView();
    int view = layerProcessor.getLastValidLayer();
    if (view > viewV)
    {
        view = viewV;
    }

    if (view < 0)
    {
        logError("view < 0");
        mutex.unlock();
        return false;
    }

    const deBaseLayer* layer = layerStack.startReadLayer(view);

    if (layer)
    {
        const deImageOld& layerImage = layer->getLayerImage();

        if (viewManager.isSingleChannel())
        {
            bool reversed = false;
            deColorSpace colorSpace = layerImage.getColorSpace();
            if (colorSpace == deColorSpaceCMYK)
            {
                reversed = true;
            }
            renderChannel(layerImage, viewManager.getChannel(), getCurrentImageData(), channelManager, reversed);
            renderedImage.clearError();
        }
        else
        {
            deConversionProcessor p;
            if (!p.renderImageToRGBNew(layerImage, getCurrentImageData()))
            {
                logError("render image FAILED");
                renderedImage.setError();
            }
            else
            {
                renderedImage.clearError();
            }
        }
    }        
    else
    {
        logError("no layer in renderer");
    }

    layerStack.finishReadLayer(view);

    mutex.unlock();

    return true;
}

bool deRenderer::render(deCanvas& canvas)
{
    mutex.lock();

    bool result = renderedImage.render(canvas);

    if (!result)
    {
        canvas.clear();
    }

    mutex.unlock();

    return result;
}

unsigned char* deRenderer::getCurrentImageData()
{
    const deSize& s = channelManager.getChannelSizeFromChannelManager();
    renderedImage.setSize(s);
    return renderedImage.getCurrentImageData();
}    

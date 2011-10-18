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

void renderImage(const deImage& image, unsigned char* data, deChannelManager& channelManager)
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
                return;
            }            
        }            
    }

    const deSize& s = channelManager.getChannelSize();

    deChannel* channel0 = channelManager.getChannel(image.getChannelIndex(0));
    deChannel* channel1 = channelManager.getChannel(image.getChannelIndex(1));
    deChannel* channel2 = channelManager.getChannel(image.getChannelIndex(2));
    deChannel* channel3 = channelManager.getChannel(image.getChannelIndex(3));

    if (!channel0)
    {
        return;
    }

    const deValue* pixels0 = channel0->getPixels();
    const deValue* pixels1 = NULL;
    if (channel1)
    {
        pixels1 = channel1->getPixels();
    }
    const deValue* pixels2 = NULL;
    if (channel2)
    {
        pixels2 = channel2->getPixels();
    }
    const deValue* pixels3 = NULL;
    if (channel3)
    {
        pixels3 = channel3->getPixels();
    }

    deValue rr;
    deValue gg;
    deValue bb;

    int n = s.getN();
    int i;
    int pos = 0;

    if (conversion4x3)
    {
        if (!pixels0)
        {
            return;
        }
        if (!pixels1)
        {
            return;
        }
        if (!pixels2)
        {
            return;
        }
        if (!pixels3)
        {
            return;
        }
        for (i = 0; i < n; i++)
        {
            deValue s0 = pixels0[i];
            deValue s1 = pixels1[i];
            deValue s2 = pixels2[i];
            deValue s3 = pixels3[i];
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
    }
    else if (conversion1x3)
    {
        if (!pixels0)
        {
            return;
        }
        for (i = 0; i < n; i++)
        {
            deValue s0 = pixels0[i];
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
    }
    else
    {
        if (!pixels0)
        {
            return;
        }
        if (!pixels1)
        {
            return;
        }
        if (!pixels2)
        {
            return;
        }
        for (i = 0; i < n; i++)
        {
            deValue s1 = pixels0[i];
            deValue s2 = pixels1[i];
            deValue s3 = pixels2[i];
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
    }

}

void renderChannel(const deImage& image, int c, unsigned char* data, deChannelManager& channelManager)
{
    const deSize& s = channelManager.getChannelSize();

    deChannel* channel = channelManager.getChannel(image.getChannelIndex(c));

    if (!channel)
    {
        return;
    }

    const deValue* pixels = channel->getPixels();

    int n = s.getN();
    int i;
    int pos = 0;
    for (i = 0; i < n; i++)
    {
        deValue s = pixels[i];

        unsigned char ss = 255 * s;

        data[pos] = ss;
        pos++;
        data[pos] = ss;
        pos++;
        data[pos] = ss;
        pos++;
    }      

}

void renderChannel(int c, unsigned char* data, deChannelManager& channelManager)
{
    const deSize& s = channelManager.getChannelSize();

    deChannel* channel = channelManager.getChannel(c);

    if (!channel)
    {
        return;
    }

    const deValue* pixels = channel->getPixels();

    int n = s.getN();
    int i;
    int pos = 0;
    for (i = 0; i < n; i++)
    {
        deValue s = pixels[i];

        unsigned char ss = 255 * s;

        data[pos] = ss;
        pos++;
        data[pos] = ss;
        pos++;
        data[pos] = ss;
        pos++;
    }      

}

deRenderer::deRenderer(deProject* _project)
:project(_project), size(0,0)
{
    image = NULL;
}

deRenderer::~deRenderer()
{
    if (image)
    {
        delete image;
    }        
}

bool deRenderer::render(wxDC& dc)
{
    deChannelManager& channelManager = project->getPreviewChannelManager();
    const deSize& s = channelManager.getChannelSize();
    if (s != size)
    {
        if (image)
        {
            delete image;
        }            
        int w = s.getW();
        int h = s.getH();
        image = new wxImage(w,h);
        size = s;
    }

    const deViewManager& viewManager = project->getViewManager();
    int view = viewManager.getView();

    deLayerStack& layerStack = project->getLayerStack();

    unsigned char* data = image->GetData();

    if (viewManager.maskVisible())
    {
        renderChannel(viewManager.getMaskChannel(), data, channelManager);
    }
    else
    {
        deLayer* layer = layerStack.getLayer(view);
        if (!layer)
        {
            return false;
        }

        const deImage& layerImage = layer->getImage();

        if (viewManager.isSingleChannel())
        {
            renderChannel(layerImage, viewManager.getChannel(), data, channelManager);
        }
        else
        {
            renderImage(layerImage, data, channelManager);
        }
    }

    wxBitmap bitmap(*image);
    dc.DrawBitmap(bitmap, 0, 0, false);

    return true;
}


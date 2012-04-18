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

#include "histogram_panel.h"
#include "project.h"
#include "color_space.h"
#include "channel.h"
#include <sstream>
#include "layer.h"
#include "image.h"
#include "layer_processor.h"
#include <wx/dcbuffer.h>
#include "channel_manager.h"
#include "layer_stack.h"
#include "str.h"
#include "canvas_wx.h"

BEGIN_EVENT_TABLE(deHistogramPanel, wxPanel)
EVT_PAINT(deHistogramPanel::paintEvent)
END_EVENT_TABLE()

deHistogramPanel::deHistogramPanel(wxWindow* parent, deProject* _project)
:wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(256, 200)), project(_project), histogram(256)
{
    channel = 0;
    generated = false;
    renderedImage.setSize(deSize(256, 256));
}

deHistogramPanel::~deHistogramPanel()
{
}

void deHistogramPanel::paintEvent(wxPaintEvent & evt)
{
    logMessage("paintEvent in deHistogramPanel");
    wxBufferedPaintDC dc(this);
    deCanvasWX canvas(dc);
	render(canvas);
}

void deHistogramPanel::paintHistogram()
{
    wxClientDC dc(this);
    wxBufferedDC bufferedDC(&dc);
    deCanvasWX canvas(bufferedDC);
	render(canvas);
}

void deHistogramPanel::render(deCanvas& canvas)
{
    if (generated)
    {
        renderedImage.render(canvas);
    }       
    else
    {
        canvas.clear();
    }
}

void deHistogramPanel::generateHistogram()
{
    project->getLayerProcessor().lock();

    generated = false;
    deLayerStack& layerStack = project->getLayerStack();
    deViewManager& viewManager = project->getViewManager();
    deChannelManager& channelManager = project->getPreviewChannelManager();

    int viewV = viewManager.getView();
    int view = project->getLayerProcessor().getLastValidLayer();
    if (view > viewV)
    {
        view = viewV;
    }

    deBaseLayer* layer = layerStack.getLayer(view);

    if (layer)
    {
        const deImage& image = layer->getLayerImage();
        int channelIndex = image.getChannelIndex(channel);

        logMessage("generate histogram for channel " + str(channelIndex));

        deChannel* c = channelManager.getChannel(channelIndex);

        if (!c)
        {
            logMessage("generate histogram - NULL channel");
        }            

        int n = channelManager.getChannelSize().getN();

        if (n <= 0)
        {
            logMessage("generate histogram - n: " + str(n));
        }

        if ((c) && (n > 0))
        {
            histogram.clear();
            histogram.calc(c, n);
        }            
    }        

    project->getLayerProcessor().unlock();

    if (layer)
    {
        int sizeW = 256;
        int sizeH = 200;
        unsigned char g1 = 50;
        unsigned char g2 = 120;

        generated = histogram.render(renderedImage.getCurrentImageData(), sizeW, sizeH, g1, g2);
    }        
}

void deHistogramPanel::setChannel(int _channel)
{
    channel = _channel;
}

int deHistogramPanel::getChannel() const
{
    return channel;
}

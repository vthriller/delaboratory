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

BEGIN_EVENT_TABLE(deHistogramPanel, wxPanel)
EVT_PAINT(deHistogramPanel::paintEvent)
END_EVENT_TABLE()

deHistogramPanel::deHistogramPanel(wxWindow* parent, deProject* _project)
:wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(256, 200)), project(_project), histogram(256), histogramImage(256, 200)
{
    channel = 0;
    generated = false;
    project->setHistogramPanel(this);
}

deHistogramPanel::~deHistogramPanel()
{
}

void deHistogramPanel::paintEvent(wxPaintEvent & evt)
{
    logMessage("paintEvent in deHistogramPanel");
    wxBufferedPaintDC dc(this);
	render(dc);
}

void deHistogramPanel::paintHistogram()
{
    wxClientDC dc(this);
    wxBufferedDC bufferedDC(&dc);
	render(bufferedDC);
}

void deHistogramPanel::render(wxDC& dc)
{
    if (generated)
    {
        wxBitmap* bitmap = new wxBitmap(histogramImage);
        dc.DrawBitmap(*bitmap, 0, 0, false);
    }       
    else
    {
        dc.Clear();
    }
}

void deHistogramPanel::generateHistogram()
{
    project->getLayerProcessor().lock();

    generated = false;
    deLayerStack& layerStack = project->getLayerStack();
    deViewManager& viewManager = project->getViewManager();
    deChannelManager& channelManager = project->getPreviewChannelManager();

    int view = viewManager.getView();
    deLayer* layer = layerStack.getLayer(view);
    if (layer)
    {
        const deImage& image = layer->getImage();
        int channelIndex = image.getChannelIndex(channel);
        deChannel* c = channelManager.getChannel(channelIndex);

        int n = channelManager.getChannelSize().getN();

        if ((c) && (n > 0))
        {
            c->lockRead();

            histogram.clear();
            histogram.calc(c, n);

            c->unlockRead();
        }            
    }        

    project->getLayerProcessor().unlock();

    if (layer)
    {
        int sizeW = 256;
        int sizeH = 200;
        unsigned char g1 = 50;
        unsigned char g2 = 120;

        generated = histogram.render(histogramImage.GetData(), sizeW, sizeH, g1, g2);
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

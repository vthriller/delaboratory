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
#include <sstream>
#include "base_layer.h"
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

deHistogramPanel::deHistogramPanel(wxWindow* parent, deProject* _project, int _width, int _margin)
:wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(_width, 200)), project(_project), histogram(_width - 2 * _margin), width(_width), margin(_margin)
{
    channel = 0;
    generated = false;
    renderedImage.setSize(deSize(width, width));
}

deHistogramPanel::~deHistogramPanel()
{
}

void deHistogramPanel::paintEvent(wxPaintEvent & evt)
{
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
    mutex.lock();

    if (generated)
    {
        renderedImage.render(canvas);
    }       
    else
    {
        canvas.clear();
    }

    mutex.unlock();
}

void deHistogramPanel::generateHistogram()
{
    mutex.lock();

#ifdef DEBUG_LOG
    logInfo("generate histogram...");
#endif    

    generated = false;

    deLayerStack& layerStack = project->getLayerStack();
    deViewManager& viewManager = project->getViewManager();

    int viewV = viewManager.getView();

    project->getLayerProcessor().lockLayerProcessor();

    int view = project->getLayerProcessor().getLastValidLayer();
    if (view > viewV)
    {
        view = viewV;
    }
    const deBaseLayer* layer = layerStack.startReadLayer(view);

    project->getLayerProcessor().unlockLayerProcessor();

    if (layer)
    {
        const deImage& image = layer->getLayerImage();
        int n = image.getChannelSize().getN();

        const deValue* values = image.startRead(channel);

        if ((n > 0) && (values))
        {
            histogram.clear();
            histogram.calc(values, n);
        }
        else
       {
            if (!values)
            {
                logError("can't generate histogram - NULL channel");
            }
            if (n <= 0)
            {
                logError("can't generate histogram - n: " + str(n));
            }
        }

        image.finishRead(channel);
    }        


    if (layer)
    {
        int sizeH = 200;
        unsigned char g1 = 50;
        unsigned char g2 = 120;

        generated = histogram.render(renderedImage.getCurrentImageData(), width, sizeH, g1, g2, margin);
    }        

    layerStack.finishReadLayer(view);

#ifdef DEBUG_LOG
    logInfo("generate histogram DONE");
#endif    

    mutex.unlock();
}

void deHistogramPanel::setChannel(int _channel)
{
    mutex.lock();
    channel = _channel;
    mutex.unlock();
}

int deHistogramPanel::getChannel() const
{
    return channel;
}

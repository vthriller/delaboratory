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

#include "property_levels_ui.h"
#include "property_levels.h"
#include "layer_processor.h"
#include "panel_wx.h"
#include "window_wx.h"
#include "histogram.h"
#include <wx/dcbuffer.h>
#include "canvas_wx.h"
#include "gradient_panel.h"
#include "channel_selector.h"
#include "base_layer_with_source.h"

class deLevelsChannelSelector:public deChannelSelector
{
    private:
        dePropertyLevelsUIImpl& ui;
    public:
        deLevelsChannelSelector(deWindow& window, deColorSpace colorSpace, dePropertyLevelsUIImpl& _ui)
        :deChannelSelector(window, colorSpace), ui(_ui)
        {
        }
        virtual ~deLevelsChannelSelector()
        {
        }
        virtual void onValueChange(int channel);
};


class deLevelsControl:public dePanelWX
{
    private:
        wxBitmap bitmap;
        dePropertyLevels& property;
        int width;
        int height;
        int margin;

        deLayerProcessor& layerProcessor;
        int layerIndex;

        int channel;

        enum 
        {
            deModeNothing,
            deModeMin,
            deModeMiddle,
            deModeMax
        } mode;            

        void click(wxMouseEvent &event)
        {
            deValue v = getValue(event.GetX());

            const deLevels& levels = property.getLevels(channel);

            deValue min = levels.getMin();
            deValue middle = levels.getMiddle();
            deValue max = levels.getMax();

            deValue dmin = fabs(v - min);
            deValue dmiddle = fabs(v - middle);
            deValue dmax = fabs(v - max);

            deValue d = 0.01;

            if (dmin < d)
            {
                mode = deModeMin;
            } else
            if (dmax < d)
            {
                mode = deModeMax;
            } else
            if (dmiddle < d)
            {
                mode = deModeMiddle;
            }
        }
        
        void release(wxMouseEvent &event)
        {
            mode = deModeNothing;
        }
        
        void move(wxMouseEvent &event)
        {
            deValue v = getValue(event.GetX());

            if ((v < 0) || (v > 1.0))
            {
                return;
            }

            deLevels& levels = property.getLevels(channel);

            switch (mode)
            {
                case deModeMin:
                {
                    levels.setMin(v);
                    break;
                }
                case deModeMiddle:
                {
                    levels.setMiddle(v);
                    break;
                }
                case deModeMax:
                {
                    levels.setMax(v);
                    break;
                }
                default:
                {
                    return;
                    break;
                }                    
            }

            paint();

            layerProcessor.markUpdateSingleChannel(layerIndex, channel);
        }

        void drawArrow(deCanvas& canvas, deValue v)
        {
            int p = margin + v * (width - 2 * margin);
            int m = 3;
            int p1 = p - m;
            if (p1 < 0)
            {
                p1 = 0;
            }
            int p2 = p + m;
            if (p2 >= width)
            {
                p2 = width - 1;
            }

            int g = 50;
            canvas.setBrush(g, g, g);
            canvas.drawTriangle(p, 0, p1, height, p2, height);
        }

        deValue getValue(int p)
        {
            deValue a = p - margin;
            deValue b = width - 2 * margin;
            return a / b;
        }


    public:
        deLevelsControl(deWindow& _parentWindow, dePropertyLevels& _property, int _width, int _height, int _margin, deLayerProcessor& _layerProcessor, int _layerIndex)
        :dePanelWX(_parentWindow, _width, _height), property(_property), width(_width), height(_height), margin(_margin), layerProcessor(_layerProcessor), layerIndex(_layerIndex)
        {
            SetMinSize(wxSize(width, height));

            channel = -1;

            mode = deModeNothing;

            bitmap.Create(_width, _height);

            Connect(wxEVT_PAINT, wxPaintEventHandler(deLevelsControl::paintEvent));
            Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(deLevelsControl::click));
            Connect(wxEVT_LEFT_UP, wxMouseEventHandler(deLevelsControl::release));
            Connect(wxEVT_MOTION, wxMouseEventHandler(deLevelsControl::move));

        }

        ~deLevelsControl()
        {
        }

        void setChannel(int _channel)
        {
            channel = _channel;
            paint();
        }

        void paint()
        {
            wxClientDC dc(this);

            render(dc);
        }

        void paintEvent(wxPaintEvent & evt)
        {
            wxPaintDC dc(this);

            render(dc);
        }

        void render(wxDC& dc_orig)
        {
            wxBufferedDC dc(&dc_orig, bitmap, wxBUFFER_CLIENT_AREA);

            deCanvasWX canvas(dc);

            int h = height / 5;

            canvas.clear();
            canvas.drawLine(0, h, width, h);

            const deLevels& levels = property.getLevels(channel);
            deValue min = levels.getMin();
            deValue middle = levels.getMiddle();
            deValue max = levels.getMax();

            unsigned char g = 50;
            canvas.setPen(g, g, g);
            drawArrow(canvas, min);
            drawArrow(canvas, middle);
            drawArrow(canvas, max);
        }
};

class deChannelHistogramPanel:public dePanelWX
{
    private:
        wxBitmap* backgroundBitmap;
        int width;
        int height;
        int margin;

    public:
        deChannelHistogramPanel(deWindow& _parentWindow, const deValue* c, int n, int _width, int _height, int _margin)
        :dePanelWX(_parentWindow, _width, _height), width(_width), height(_height), margin(_margin)
        {
            backgroundBitmap = NULL;

            generate(c, n);

            Connect(wxEVT_PAINT, wxPaintEventHandler(deChannelHistogramPanel::paintEvent));
        }

        virtual ~deChannelHistogramPanel()
        {
        }

        void paintEvent(wxPaintEvent & evt)
        {
            wxPaintDC dc(this);
            dc.DrawBitmap(*backgroundBitmap, 0, 0, false);
        }

        void paint()
        {
            wxClientDC dc(this);
            dc.DrawBitmap(*backgroundBitmap, 0, 0, false);
        }

        void generate(const deValue* c, int n)
        {
            if (backgroundBitmap)
            {
                delete backgroundBitmap;
                backgroundBitmap = NULL;
            }

            deHistogram histogram(width - 2 * margin);

            histogram.clear();
            histogram.calc(c, n);

            wxImage* image = new wxImage(width, height);
            unsigned char* data = image->GetData();

            unsigned char g1 = 255;
            unsigned char g2 = 200;

            histogram.render(data, width, height, g1, g2, margin);

            backgroundBitmap = new wxBitmap(*image);
            delete image;

            paint();
        }
};


class dePropertyLevelsUIImpl:public dePanelWX
{
    private:
        deBaseLayerWithSource& layer;
        dePropertyLevelsUI& parent;
        dePropertyLevels& property;

        deLevelsChannelSelector *channelSelector;
        deChannelHistogramPanel* histogramPanel;
        deLevelsControl* levelsControl;
        deGradientPanel0* gradient;

        int channel;

    public:
        dePropertyLevelsUIImpl(dePropertyLevelsUI& _parent, deWindow& _parentWindow, dePropertyLevels& _property, deBaseLayerWithSource& _layer, deLayerProcessor& layerProcessor, int layerIndex, int width)
        :dePanelWX(_parentWindow), layer(_layer), parent(_parent), property(_property)
        {
            int margin = 10;

            wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
            SetSizer(sizer);

            channel = -1;

            deColorSpace colorSpace = layer.getColorSpace();

            {
                channelSelector = new deLevelsChannelSelector(getWindow(), colorSpace, *this);
                deWindowWX& w = dynamic_cast<deWindowWX&>(channelSelector->getWindow());
                sizer->Add(w.getWindow());
            }                

            int h = 80;
            histogramPanel = new deChannelHistogramPanel(getWindow(), NULL, 0, width, h, margin);
            sizer->Add(histogramPanel);

            gradient = new deGradientPanel0(this, wxSize(width, 8), layer.getColorSpace(), 0, margin);
            sizer->Add(gradient);

            levelsControl = new deLevelsControl(getWindow(), property, width, 10, margin, layerProcessor, layerIndex);
            sizer->Add(levelsControl);

            setChannel(0);

            Fit();
        }

        virtual ~dePropertyLevelsUIImpl()
        {
        }

        void generate()
        {
            const deImage& sourceImage = layer.getSourceImage();

            const deValue* c = sourceImage.startRead(channel);
            int n = sourceImage.getChannelSize().getN();

            histogramPanel->generate(c, n);

            sourceImage.finishRead(channel);
        }

        void setFromProperty()
        {
            levelsControl->paint();
        }

        void setChannel(int _channel)
        {
            channel = _channel;
            levelsControl->setChannel(channel);
            gradient->changeChannel(channel);
            generate();
        }


};

dePropertyLevelsUI::dePropertyLevelsUI(deWindow& window, dePropertyLevels& _property, deLayerProcessor& _layerProcessor, int _layerIndex, deBaseLayerWithSource& _layer, int width)
:property(_property), layerProcessor(_layerProcessor), layerIndex(_layerIndex), layer(_layer)
{
    deWindowWX* w = dynamic_cast<deWindowWX*>(&window);
    if (w)
    {
        impl = new dePropertyLevelsUIImpl(*this, window, property, layer, layerProcessor, layerIndex, width);
    }
    else
    {
        impl = NULL;
    }
}

dePropertyLevelsUI::~dePropertyLevelsUI()
{
    if (impl)
    {
        delete impl;
    }        
}

deWindow& dePropertyLevelsUI::getWindow()
{
    return impl->getWindow();
}

void dePropertyLevelsUI::setFromProperty()
{
    if (impl)
    {
        impl->setFromProperty();
    }
}

void deLevelsChannelSelector::onValueChange(int channel)
{
    ui.setChannel(channel);
}

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

#include "property_curves_ui.h"
#include "property_curves.h"
#include "layer_processor.h"
#include "panel_wx.h"
#include "window_wx.h"
#include <wx/dcbuffer.h>
#include "canvas_wx.h"
#include "gradient_panel.h"
#include "channel_selector.h"
#include "curves_panel.h"
#include "base_layer_with_source.h"

class deCurvesChannelSelector:public deChannelSelector
{
    private:
        dePropertyCurvesUIImpl& ui;
    public:
        deCurvesChannelSelector(deWindow& window, deColorSpace colorSpace, dePropertyCurvesUIImpl& _ui)
        :deChannelSelector(window, colorSpace), ui(_ui)
        {
        }
        virtual ~deCurvesChannelSelector()
        {
        }
        virtual void onValueChange(int channel);
};


class dePropertyCurvesUIImpl:public dePanelWX
{
    private:
        deBaseLayerWithSource& layer;
        dePropertyCurvesUI& parent;
        dePropertyCurves& property;

        deCurvesChannelSelector *channelSelector;
        deCurvesPanel* curvesPanel;
        deGradientPanel1* leftBar;
        deGradientPanel1* bottomBar;

        int channel;

    public:
        dePropertyCurvesUIImpl(dePropertyCurvesUI& _parent, deWindow& _parentWindow, dePropertyCurves& _property, deBaseLayerWithSource& _layer, deLayerProcessor& layerProcessor, int layerIndex, int width)
        :dePanelWX(_parentWindow), layer(_layer), parent(_parent), property(_property)
        {
            wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
            SetSizer(sizer);

            channel = -1;

            deColorSpace colorSpace = layer.getColorSpace();

            {
                channelSelector = new deCurvesChannelSelector(getWindow(), colorSpace, *this);
                deWindowWX& w = dynamic_cast<deWindowWX&>(channelSelector->getWindow());
                sizer->Add(w.getWindow());
            }                

            curvesPanel = new deCurvesPanel(this, layerProcessor, layerIndex, property, colorSpace);

            int barSize = 16;

            leftBar = new deGradientPanel1(this, wxSize(barSize, CURVES_PANEL_SIZE_Y), colorSpace, 0, -1, -1, -1, -1);
            bottomBar = new deGradientPanel1(this, wxSize(CURVES_PANEL_SIZE_X, barSize), colorSpace, 0, -1, -1, -1, -1);

            wxSizer* sizerC = new wxFlexGridSizer(2, 8, 8);
            sizer->Add(sizerC);

            sizerC->Add(leftBar, 0, wxCENTER);
            sizerC->Add(curvesPanel, 0, wxCENTER);
            sizerC->Add(-1, 0, wxCENTER);
            sizerC->Add(bottomBar, 0, wxCENTER);

            setChannel(0);

            Fit();
        }

        virtual ~dePropertyCurvesUIImpl()
        {
        }

        void generate()
        {
            const deImage& sourceImage = layer.getSourceImage();

            int chan = channel;
            int hc = property.getHorizontalChannel();
            if (hc >=0 )
            {
                chan = hc;
            }

            const deValue* c = sourceImage.startRead(chan);

            int n = sourceImage.getChannelSize().getN();

            curvesPanel->generateBackground(c, n);

            sourceImage.finishRead(chan);
        }

        void setMarker()
        {
            const deImage& sourceImage = layer.getSourceImage();

            const deValue* c = sourceImage.startRead(channel);
            int n = sourceImage.getChannelSize().getN();

            curvesPanel->setMarker(c, n);

            sourceImage.finishRead(channel);
        }

        void setFromProperty()
        {
            generate();
            updateBars();
            curvesPanel->paint();
        }

        void setChannel(int _channel)
        {
            channel = _channel;
            generate();
            curvesPanel->changeChannel(channel);
            setMarker();
            updateBars();
        }

        void updateBars()
        {
            leftBar->changeChannel(channel);
            int hc = property.getHorizontalChannel();
            if (hc < 0)
            {
                bottomBar->changeChannel(channel);
            }
            else
            {
                bottomBar->changeChannel(hc);
            }
        }

        bool onImageClick(deValue x, deValue y)
        {
            const deImage& sourceImage = layer.getSourceImage();

            const deValue* c = sourceImage.startRead(channel);

            curvesPanel->onImageClick(x, y, c, sourceImage.getChannelSize());

            sourceImage.finishRead(channel);

            return true;
        }

        bool onKey(int key)
        {
            return true;
        }


};

dePropertyCurvesUI::dePropertyCurvesUI(deWindow& window, dePropertyCurves& _property, deLayerProcessor& _layerProcessor, int _layerIndex, deBaseLayerWithSource& _layer, int width)
:property(_property), layerProcessor(_layerProcessor), layerIndex(_layerIndex), layer(_layer)
{
    deWindowWX* w = dynamic_cast<deWindowWX*>(&window);
    if (w)
    {
        impl = new dePropertyCurvesUIImpl(*this, window, property, layer, layerProcessor, layerIndex, width);
    }
    else
    {
        impl = NULL;
    }
}

dePropertyCurvesUI::~dePropertyCurvesUI()
{
    if (impl)
    {
        delete impl;
    }        
}

deWindow& dePropertyCurvesUI::getWindow()
{
    return impl->getWindow();
}

void dePropertyCurvesUI::setFromProperty()
{
    if (impl)
    {
        impl->setFromProperty();
    }
}

bool dePropertyCurvesUI::onImageClick(deValue x, deValue y)
{
    if (impl)
    {
        impl->onImageClick(x,y);
    }
}

bool dePropertyCurvesUI::onKey(int key)
{
    if (impl)
    {
        impl->onKey(key);
    }
}

void deCurvesChannelSelector::onValueChange(int channel)
{
    ui.setChannel(channel);
}

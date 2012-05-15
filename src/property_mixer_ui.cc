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

#include "property_mixer_ui.h"
#include "property_mixer.h"
#include "layer_processor.h"
#include "panel_wx.h"
#include "window_wx.h"
#include "histogram.h"
#include <wx/dcbuffer.h>
#include "canvas_wx.h"
#include "gradient_panel.h"
#include "color_space_utils.h"
#include "slider.h"
#include "logger.h"
#include "channel_selector.h"

class deMixerChannelSelector:public deChannelSelector
{
    private:
        dePropertyMixerUIImpl& ui;
    public:
        deMixerChannelSelector(deWindow& window, deColorSpace colorSpace, dePropertyMixerUIImpl& _ui)
        :deChannelSelector(window, colorSpace), ui(_ui)
        {
        }
        virtual ~deMixerChannelSelector()
        {
        }
        virtual void onValueChange(int channel);
};

class deMixerSlider:public deSlider
{
    private:
        dePropertyMixerUIImpl& ui;
        int index;
    public:
        deMixerSlider(deWindow& window, const std::string& _name, deValue _min, deValue _max, int _width, int widthn, int widthl, dePropertyMixerUIImpl& _ui, int _index)
        :deSlider(window, _name, _min, _max, _width, widthn, widthl),
         ui(_ui), index(_index)
        {
            logInfo("deMixerSlider constructor");
        }
        
        virtual ~deMixerSlider()
        {
            logInfo("deMixerSlider destructor");
        }

        virtual void onValueChange(deValue value, bool finished);
};

class dePropertyMixerUIImpl:public dePanelWX
{
    private:
        dePropertyMixerUI& parent;
        dePropertyMixer& property;
        deMixerChannelSelector *channelSelector;
        std::vector<deMixerSlider*> sliders;
        int channel;
        deLayerProcessor& layerProcessor;
        deGradientPanel0* gradient;
        int layerIndex;

    public:
        dePropertyMixerUIImpl(dePropertyMixerUI& _parent, deWindow& _parentWindow, dePropertyMixer& _property, deBaseLayerWithSource& layer, deLayerProcessor& _layerProcessor, int _layerIndex, int width)
        :dePanelWX(_parentWindow), parent(_parent), property(_property), layerProcessor(_layerProcessor), layerIndex(_layerIndex)
        {
            int margin = 10;

            wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
            SetSizer(sizer);

            deColorSpace colorSpace = layer.getColorSpace();

            {
                channelSelector = new deMixerChannelSelector(getWindow(), colorSpace, *this);
                deWindowWX& w = dynamic_cast<deWindowWX&>(channelSelector->getWindow());
                sizer->Add(w.getWindow());
            }                

            const deImage& sourceImage = layer.getSourceImage();

            channel = 0;

            int n = getColorSpaceSize(layer.getColorSpace());

            deValue min = -3;
            deValue max = 3;
            int w = 400;
            int wn = 80;
            int wl = 40;
            int i;
            for (i = 0; i < n; i++)
            {
                std::string c = getChannelName(layer.getColorSpace(), i);
                deMixerSlider* s = new deMixerSlider(getWindow(), c, min, max, w, wn, wl, *this, i);
                sliders.push_back(s);
                deWindowWX& w = dynamic_cast<deWindowWX&>(s->getWindow());
                sizer->Add(w.getWindow());
            }

            gradient = new deGradientPanel0(this, wxSize(width, 8), layer.getColorSpace(), channel, margin);
            sizer->Add(gradient);

            setFromProperty();

            Fit();
        }

        virtual ~dePropertyMixerUIImpl()
        {
            delete channelSelector;
        }

        void setFromProperty()
        {
            deMixer* mixer = property.getMixer(channel);

            int n = sliders.size();
            int i;
            for (i = 0; i < n; i++)
            {
                deValue w = mixer->getWeight(i);
                deMixerSlider* s = sliders[i];
                s->setValue(w);
            }
        }

        void setFromSlider(int index, deValue v)
        {
            deMixer* mixer = property.getMixer(channel);
            mixer->setWeight(index, v);

            layerProcessor.markUpdateSingleChannel(layerIndex, channel);
        }

        void setChannel(int _channel)
        {
            channel = _channel;
            gradient->changeChannel(channel);
            setFromProperty();
            layerProcessor.setHistogramChannel(channel);

        }


};

dePropertyMixerUI::dePropertyMixerUI(deWindow& window, dePropertyMixer& _property, deLayerProcessor& _layerProcessor, int _layerIndex, deBaseLayerWithSource& _layer, int width)
:property(_property), layerProcessor(_layerProcessor), layerIndex(_layerIndex), layer(_layer)
{
    deWindowWX* w = dynamic_cast<deWindowWX*>(&window);
    if (w)
    {
        impl = new dePropertyMixerUIImpl(*this, window, property, layer, layerProcessor, layerIndex, width);
    }
    else
    {
        impl = NULL;
    }
}

dePropertyMixerUI::~dePropertyMixerUI()
{
    if (impl)
    {
        delete impl;
    }        
}

deWindow& dePropertyMixerUI::getWindow()
{
    return impl->getWindow();
}

void dePropertyMixerUI::setFromProperty()
{
    if (impl)
    {
        impl->setFromProperty();
    }
}

void deMixerSlider::onValueChange(deValue value, bool finished)
{
    ui.setFromSlider(index, value);
};


void deMixerChannelSelector::onValueChange(int channel)
{
    ui.setChannel(channel);
};

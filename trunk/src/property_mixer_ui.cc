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

class dePropertyMixerUIImpl:public dePanelWX
{
    private:
        dePropertyMixerUI& parent;
        dePropertyMixer& property;

    public:
        dePropertyMixerUIImpl(dePropertyMixerUI& _parent, deWindow& _parentWindow, dePropertyMixer& _property, deBaseLayerWithSource& layer, deLayerProcessor& layerProcessor, int layerIndex, int width)
        :dePanelWX(_parentWindow), parent(_parent), property(_property)
        {
            int margin = 10;

            wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
            SetSizer(sizer);

            const deImage& sourceImage = layer.getSourceImage();

            int channel = 0;

            deGradientPanel0* gradient = new deGradientPanel0(this, wxSize(width, 8), layer.getColorSpace(), channel, margin);
            sizer->Add(gradient);

            Fit();
        }

        virtual ~dePropertyMixerUIImpl()
        {
        }

        void setFromProperty()
        {
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

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

#include "equalizer_frame.h"
#include "equalizer_layer.h"
#include "equalizer.h"
#include "layer_processor.h"
#include <iostream>
#include "property_value_slider.h"
#include "property_choice_ui.h"
#include "gradient_panel.h"

class deEqualizerSlider:public deSlider
{
    private:
        deEqualizerLayer& layer;
        deEqualizer& equalizer;
        int index;
        int channel;
        deLayerProcessor& layerProcessor;

    public:
        deEqualizerSlider(wxWindow *parent, int range, deEqualizerLayer& _layer, deEqualizer& _equalizer, int _index, int _channel, const std::string& name, deLayerProcessor& _layerProcessor)
        :deSlider(parent, name, range, -1.0, 1.0, 0.0, _layerProcessor), layer(_layer), equalizer(_equalizer), index(_index), channel(_channel),
         layerProcessor(_layerProcessor)
        {
            setValue(equalizer.getValue(index));
        }

        virtual ~deEqualizerSlider()
        {
        }

        virtual void onValueChange(deValue value, bool finished)
        {
            if ((finished) || (layerProcessor.isRealtime()))
            {
                equalizer.setValue(index, value);
                layer.setHistogramChannel(index);

                int ind = layer.getIndex();
                layerProcessor.markUpdateSingleChannel(ind, channel);
            }                
        }
};        

deEqualizerFrame::deEqualizerFrame(wxWindow *parent, deActionLayer& _layer, deLayerProcessor& _layerProcessor, deLayerFrameManager& _frameManager)
:deActionFrame(parent, _layer, _frameManager), layerProcessor(_layerProcessor)
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    deEqualizerLayer& equalizerLayer = dynamic_cast<deEqualizerLayer&>(_layer);
    deColorSpace colorSpace = equalizerLayer.getColorSpace();

    int range = 300;

    int bands = equalizerLayer.getBands();
    int i;

    int n = getColorSpaceSize(equalizerLayer.getColorSpace());

    for (i = 0; i < bands; i++)
    {
        wxSizer* sSizer = new wxStaticBoxSizer(wxHORIZONTAL, this);
        sizer->Add(sSizer);

        int c = getEqualizerChannel(colorSpace);

        int w = 60;
        int h = 80;
        deValue l = (deValue) i / bands;

        deGradientPanel* gradient = new deGradientPanel(this, wxSize(w, h), colorSpace, c, -1, -1, l, -1);
        sSizer->Add(gradient);

        sSizer->AddSpacer(10);

        wxSizer* slidersSizer = new wxBoxSizer(wxVERTICAL);
        sSizer->Add(slidersSizer);

        int j;
        for (j = 0; j < n; j++)
        {
            deEqualizer* equalizer = equalizerLayer.getEqualizer(j);
            if (equalizer)
            {
                const std::string n = getChannelName(equalizerLayer.getColorSpace(), j);
                deEqualizerSlider* s = new deEqualizerSlider(this, range, equalizerLayer, *equalizer, i, j, n, layerProcessor);
                slidersSizer->Add(s);
            }
        }
    }

    reset = new wxButton(this, wxID_ANY, _T("reset"), wxDefaultPosition, wxSize(100,25));
    sizer->Add(reset, 0);

    /*

    radius = new dePropertyValueSlider(this, range, equalizerLayer.getPropertyRadius(), equalizerLayer, layerProcessor);
    sizer->Add(radius);

    threshold = new dePropertyValueSlider(this, range, equalizerLayer.getPropertyThreshold(), equalizerLayer, layerProcessor);
    sizer->Add(threshold);

    equalizerType = new dePropertyChoiceUI(this, equalizerLayer.getPropertyType(), equalizerLayer, layerProcessor);
    sizer->Add(equalizerType);

    */

    Fit();

}

deEqualizerFrame::~deEqualizerFrame()
{
}


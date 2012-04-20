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
#include "wx/notebook.h"
#include "color_space_utils.h"

class deEqualizerSlider:public deSlider
{
    private:
        deEqualizerLayer& layer;
        deEqualizer& equalizer;
        int index;
        int channel;
        deLayerProcessor& layerProcessor;
        int layerIndex;

    public:
        deEqualizerSlider(wxWindow *parent, int range, deEqualizerLayer& _layer, deEqualizer& _equalizer, int _index, int _channel, const std::string& name, deLayerProcessor& _layerProcessor, int _layerIndex)
        :deSlider(parent, name, range, -1.0, 1.0, 0.0), layer(_layer), equalizer(_equalizer), index(_index), channel(_channel),
         layerProcessor(_layerProcessor),
         layerIndex(_layerIndex)
        {
            setFromProperty();
        }

        virtual ~deEqualizerSlider()
        {
        }

        virtual void onValueChange(deValue value, bool finished)
        {
            if ((finished) || (layerProcessor.isRealtime()))
            {
                equalizer.setValue(index, value);
                layerProcessor.setHistogramChannel(index);

                layerProcessor.markUpdateSingleChannel(layerIndex, channel);
            }                
        }

        void setFromProperty()
        {
            setValue(equalizer.getValue(index));
        }
};        


deEqualizerFrame::deEqualizerFrame(wxWindow *parent, deLayer& _layer, deLayerProcessor& _layerProcessor, deLayerFrameManager& _frameManager, int _layerIndex)
:deActionFrame(parent, _layer, _frameManager, _layerIndex), layerProcessor(_layerProcessor)
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    deEqualizerLayer& equalizerLayer = dynamic_cast<deEqualizerLayer&>(_layer);
    deColorSpace colorSpace = equalizerLayer.getColorSpace();

    int range = 300;

    int bands = equalizerLayer.getBands();
    int i;

    int n = getColorSpaceSize(equalizerLayer.getColorSpace());

    wxNotebook* notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, _T("notebook"));
    sizer->Add(notebook, 1, wxEXPAND);

    int c = getEqualizerChannel(colorSpace);
    int j;
    for (j = 0; j < n; j++)
    {
        const std::string n = getChannelName(equalizerLayer.getColorSpace(), j);
        wxPanel* slidersPanel = new wxPanel(notebook);
        notebook->AddPage(slidersPanel, wxString::FromAscii(n.c_str()));
        wxSizer* sSizer = new wxFlexGridSizer(2);
        slidersPanel->SetSizer(sSizer);

        deEqualizer* equalizer = equalizerLayer.getEqualizer(j);

        if (equalizer)
        {
            for (i = 0; i < bands; i++)
            {
                int w = 100;
                int h = 30;
                deValue l = (deValue) i / bands;

                deColorPanel* gradient = new deColorPanel(slidersPanel, wxSize(w, h), NULL, 0);
                gradient->setColor(colorSpace, c, l);
                sSizer->Add(gradient);

                deEqualizerSlider* s = new deEqualizerSlider(slidersPanel, range, equalizerLayer, *equalizer, i, j, "", layerProcessor, layerIndex);
                sSizer->Add(s);
                sliders.push_back(s);
            }
        }
    }

    reset = new wxButton(this, wxID_ANY, _T("reset"), wxDefaultPosition, wxSize(100,25));
    sizer->Add(reset, 0);

    Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deEqualizerFrame::click));

    Fit();

}

deEqualizerFrame::~deEqualizerFrame()
{
}

void deEqualizerFrame::click(wxCommandEvent &event)
{
    int id = event.GetId();
    deEqualizerLayer& equalizerLayer = dynamic_cast<deEqualizerLayer&>(layer);

    if (id == reset->GetId())
    {
        equalizerLayer.reset();
    }

    std::vector<deEqualizerSlider*>::iterator j;
    for (j = sliders.begin(); j != sliders.end(); j++)
    {
        (*j)->setFromProperty();
    }

    //int index = layer.getIndex();
    layerProcessor.markUpdateAllChannels(layerIndex);
}   

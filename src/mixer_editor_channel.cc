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

#include "mixer_editor_channel.h"
#include "mixer_layer.h"
#include "slider.h"
#include "gradient_panel.h"
#include "layer_processor.h"

class deMixerSlider:public deSlider
{
    private:
        deMixerLayer& layer;
        int s;
        int d;
        deLayerProcessor& layerProcessor;

    public:
        deMixerSlider(wxWindow *parent, int range, deMixerLayer& _layer, int _s, int _d, const std::string& name, deLayerProcessor& _layerProcessor)
        :deSlider(parent, name, range, -2.0, 2.0, 0.0), layer(_layer), s(_s), d(_d),
         layerProcessor(_layerProcessor)
        {
            setValue(layer.getWeight(s, d));
        }

        virtual ~deMixerSlider()
        {
        }

        virtual void onValueChange(deValue value, bool finished)
        {
            if (finished)
            {
                layer.setWeight(s, d, value);

                int index = layer.getIndex();
                layerProcessor.markUpdateSingleChannel(index, d);
            }                
        }
};        

deMixerEditorChannel::deMixerEditorChannel(wxWindow *parent, deMixerLayer& _layer, int _index, deLayerProcessor& _layerProcessor)
:wxPanel(parent), layer(_layer), index(_index), layerProcessor(_layerProcessor)
{
    deColorSpace colorSpace = layer.getColorSpace();
    unsigned int n = getColorSpaceSize(colorSpace);

    std::string name = getChannelName(colorSpace, index);
    wxSizer* sizer = new wxStaticBoxSizer(wxVERTICAL, this,  wxString::FromAscii(name.c_str()));
    SetSizer(sizer);

    int barSize = 20;
    int width = 300;

    deGradientPanel* gradient = new deGradientPanel(this, wxSize(width, barSize), colorSpace, index, -1, -1, -1, -1);
    sizer->Add(gradient, 0, wxCENTER);

    std::string src1;
    std::string src2;

    unsigned int i;
    int counter = 0;
    for (i = 0; i < n; i++)
    {
        std::string src = getChannelName(colorSpace, i);
        deMixerSlider* slider = new deMixerSlider(this, width, layer, i, index, src, layerProcessor);        
        sliders.push_back(slider);
        sizer->Add(slider);
        if (i != index)
        {
            counter++;
            if (counter == 1)
            {
                src1 = src;
            }
            else
            {
                src2 = src;
            }
        }            
    }

    wxSizer* sizerB = new wxStaticBoxSizer(wxHORIZONTAL, this, _T(""));
    sizer->Add(sizerB, 0);

    reset = new wxButton(this, wxID_ANY, _T("reset"), wxDefaultPosition, wxSize(60,25));
    sizerB->Add(reset, 0);

    app1 = new wxButton(this, wxID_ANY, wxString::FromAscii(src1.c_str()), wxDefaultPosition, wxSize(60,25));
    sizerB->Add(app1, 0);

    app2 = new wxButton(this, wxID_ANY, wxString::FromAscii(src2.c_str()), wxDefaultPosition, wxSize(60,25));
    sizerB->Add(app2, 0);

    mix1 = new wxButton(this, wxID_ANY, _T("0.1"), wxDefaultPosition, wxSize(60,25));
    sizerB->Add(mix1, 0);

    mix2 = new wxButton(this, wxID_ANY, _T("0.3"), wxDefaultPosition, wxSize(60,25));
    sizerB->Add(mix2, 0);

    mix3 = new wxButton(this, wxID_ANY, _T("-0.3"), wxDefaultPosition, wxSize(60,25));
    sizerB->Add(mix3, 0);

    mix4 = new wxButton(this, wxID_ANY, _T("-0.5"), wxDefaultPosition, wxSize(60,25));
    sizerB->Add(mix4, 0);

    Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deMixerEditorChannel::click));

    Layout();
    Fit();

}

deMixerEditorChannel::~deMixerEditorChannel()
{
}

void deMixerEditorChannel::click(wxCommandEvent &event)
{
    int id = event.GetId();

    if (reset->GetId() == id)
    {
        preset(0);
    }      

    if (mix1->GetId() == id)
    {
        preset(0.1);
    }      

    if (mix2->GetId() == id)
    {
        preset(0.3);
    }      

    if (mix3->GetId() == id)
    {
        preset(-0.3);
    }      

    if (mix4->GetId() == id)
    {
        preset(-0.5);
    }      

    if (app1->GetId() == id)
    {
        preset2(1.0, 0, 0);
    }      

    if (app2->GetId() == id)
    {
        preset2(0, 1.0, 0);
    }      
}

void deMixerEditorChannel::preset(deValue a)
{
    unsigned int i;
    for (i = 0; i < sliders.size(); i++) 
    {
        deValue v = a;
        if (i == index)
        {
            v = 1.0 - 2 * a;
        }
        sliders[i]->setValue(v);
        layer.setWeight(i, index, v);
    }

    int l_index = layer.getIndex();
    layerProcessor.markUpdateSingleChannel(l_index, index);
}

void deMixerEditorChannel::preset2(deValue a, deValue b, deValue c)
{
    unsigned int i;
    int counter = 0;
    for (i = 0; i < sliders.size(); i++) 
    {
        deValue v = c;
        if (i != index)
        {
            counter++;
            if (counter == 1)
            {
                v = a;
            }
            else
            {
                v = b;
            }
        }
        sliders[i]->setValue(v);
        layer.setWeight(i, index, v);
    }

    int l_index = layer.getIndex();
    layerProcessor.markUpdateSingleChannel(l_index, index);
}

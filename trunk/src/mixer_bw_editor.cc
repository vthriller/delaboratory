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

#include "mixer_bw_editor.h"
#include "conversion_bw_layer.h"
#include "slider.h"

class deMixerBWSlider:public deSlider
{
    private:
        deConversionBWLayer& layer;
        int s;

    public:
        deMixerBWSlider(wxWindow *parent, int range, deConversionBWLayer& _layer, int _s, const std::string& name)
        :deSlider(parent, name, range, -2.0, 2.0, 0.0), layer(_layer), s(_s)
        {
            setValue(layer.getWeight(s));
        }

        virtual ~deMixerBWSlider()
        {
        }

        virtual void onValueChange(deValue value, bool finished)
        {
            if (finished)
            {
                layer.setWeight(s, value);
                /*
                layer.onChannelChange(d);
                layer.updateOtherLayers();
                */
                layer.updateImage();
                layer.updateAndRepaint();
            }                
        }
};        

deMixerBWEditor::deMixerBWEditor(wxWindow *parent, deConversionBWLayer& _layer, const std::string& _name)
:deFrame(parent, _name), layer( _layer)
{
    layer.setActionFrame(this);

    deColorSpace sourceColorSpace = layer.getSourceColorSpace();

    int n = getColorSpaceSize(sourceColorSpace);

    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    int barSize = 20;
    int width = 300;

    int i;
    for (i = 0; i < n; i++)
    {
        std::string src = getChannelName(sourceColorSpace, i);
        deMixerBWSlider* slider = new deMixerBWSlider(this, width, layer, i, src);        
    //    sliders.push_back(slider);
        sizer->Add(slider);
    }

    Layout();
    Fit();

}

deMixerBWEditor::~deMixerBWEditor()
{
    layer.closeActionFrame();
}


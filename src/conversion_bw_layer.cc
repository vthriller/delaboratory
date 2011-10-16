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

#include "conversion_bw_layer.h"
#include "mixer_bw_editor.h"

deConversionBWLayer::deConversionBWLayer(int _index, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, int n)
:deConversionLayer(deColorSpaceBW, _index, _sourceLayer, _layerStack, _channelManager), mixer(n)
{

}

deConversionBWLayer::~deConversionBWLayer()
{
}

void deConversionBWLayer::createActionFrame(wxWindow* parent)
{
    if (!actionFrame)
    {
        actionFrame = new deMixerBWEditor(parent, *this, "BW conversion");
        actionFrame->Show(true);
    }        
}

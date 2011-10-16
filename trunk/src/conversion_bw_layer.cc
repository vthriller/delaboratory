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
#include "layer_stack.h"
#include "convert_image.h"
#include "channel_manager.h"

deConversionBWLayer::deConversionBWLayer(int _index, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, int n)
:deConversionLayer(deColorSpaceBW, _index, _sourceLayer, _layerStack, _channelManager), mixer(n)
{
    if (n == 3)
    {
        mixer.setWeight(0,0.3);
        mixer.setWeight(1,0.6);
        mixer.setWeight(2,0.1);
    }
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

void deConversionBWLayer::updateImage()
{
    deLayer* source = layerStack.getLayer(sourceLayer);
    const deImage& sourceImage = source->getImage();

    image.enableAllChannels();
//    convertImage(sourceImage, image, channelManager);

    int n = channelManager.getChannelSize().getN();

    deChannel* sc1 = NULL;
    deChannel* sc2 = NULL;
    deChannel* sc3 = NULL;
    deChannel* sc4 = NULL;

    if (n > 0)
    {
       sc1 = channelManager.getChannel(sourceImage.getChannelIndex(0));
    }

    if (n > 1)
    {
       sc2 = channelManager.getChannel(sourceImage.getChannelIndex(1));
    }

    if (n > 2)
    {
       sc3 = channelManager.getChannel(sourceImage.getChannelIndex(2));
    }

    if (n > 3)
    {
       sc4 = channelManager.getChannel(sourceImage.getChannelIndex(3));
    }

    deChannel* d = channelManager.getChannel(image.getChannelIndex(0));

    mixer.process(sc1, sc2, sc3, sc4, *d, n);
}

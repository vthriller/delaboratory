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

#include "source_image_layer.h"
#include <cassert>
#include "source_image.h"
#include "color_space.h"
#include <iostream>
#include "channel.h"
#include "exception.h"
#include <wx/wx.h>
#include "layer_dialog.h"

deSourceImageLayer::deSourceImageLayer(deLayerStack& _stack, int _index, const deSourceImage& _sourceImage, const std::string& _name)
:deLayer(_stack, _index, _name), sourceImage(_sourceImage), previewSize(0,0)
{
    colorSpace.setColorSpace(deColorSpaceRGB);
}

deSourceImageLayer::~deSourceImageLayer()
{
}

void deSourceImageLayer::setPreviewSize(const deSize& size)
{
    previewSize = size;
}

deActionFrame* deSourceImageLayer::createActionFrame(wxWindow* parent, int layerNumber, deProject* project)
{
    return NULL;
}

dePreview* deSourceImageLayer::createPreview(dePreviewStack& previewStack)
{
    int n = getColorSpaceSize(sourceImage.getColorSpace());
    int i;
    for (i = 0; i < n; i++)
    {
        const deBaseChannel* channel = sourceImage.getChannel(i);
        if (!channel)
        {
            return NULL;
        }
    }

    dePreview* preview = new dePreview(sourceImage.getColorSpace(), previewSize);

    for (i = 0; i < n; i++)
    {
        const deChannel* sourceChannel = dynamic_cast<const deChannel*>(sourceImage.getChannel(i));
        if (!sourceChannel)
        {
            throw deException("no sourceChannel in deSourceImageLayer::generatePreview");
        }
        deChannel* destinationChannel = dynamic_cast<deChannel*>(preview->getChannel(i));
        if (!destinationChannel)
        {
            throw deException("no destinationChannel in deSourceImageLayer::generatePreview");
        }
        destinationChannel->scale(sourceChannel);
    }

    return preview;
}

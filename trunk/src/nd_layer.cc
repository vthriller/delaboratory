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

#include "nd_layer.h"
#include "nd_frame.h"
#include "blend.h"
#include "preview_stack.h"
#include "size.h"
#include "preview.h"
#include "project.h"

deNDLayer::deNDLayer(deLayerStack& _stack, int _index, const std::string& _name)
:deLayer(_stack, _index, _name), ndType(*this), blendMode(*this), channels(*this), xCenter(*this, "x center", -1, 1), yCenter(*this, "y center", -1, 1), power(*this, "power", -1, 1)
{
    blendMode.setBlendMode(deBlendMultiply);
    clearEnabledChannels();
}

deNDLayer::~deNDLayer()
{
}

dePreview* deNDLayer::createPreview(dePreviewStack& previewStack)
{
    const dePreview* sourcePreview = previewStack.getPreview(sourceLayer.getIndex());

    if (!sourcePreview)
    {
        return NULL;
    }

    const deSize& sourceSize = sourcePreview->getSize();

    dePreview* preview = new dePreview(colorSpace.getColorSpace(), sourceSize);

    //blend(*sourcePreview, nd.getND(), *preview, blendMode.getBlendMode(), channels.getChannels());

    return preview;
}

deActionFrame* deNDLayer::createActionFrame(wxWindow* parent, int layerNumber, deProject* project)
{
    return new deNDFrame(parent, *this, project->getPreviewStack(), layerNumber);
}

void deNDLayer::onChangeColorSpace()
{
    clearEnabledChannels();
}

void deNDLayer::enableChannel(int c)
{
    channels.insert(c);
}

void deNDLayer::clearEnabledChannels()
{
    channels.clear();
}

const deChannels& deNDLayer::getEnabledChannels() const
{
    return channels.getChannels();
}

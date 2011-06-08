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

deNDLayer::deNDLayer(const std::string& _name)
:deLayer(_name)
{
    mode = deBlendMultiply;
    clearEnabledChannels();
}

deNDLayer::~deNDLayer()
{
}

dePreview* deNDLayer::createPreview(dePreviewStack& previewStack)
{
    const dePreview* sourcePreview = previewStack.getPreview(sourceLayer);

    if (!sourcePreview)
    {
        return NULL;
    }

    const deSize& sourceSize = sourcePreview->getSize();

    dePreview* preview = new dePreview(colorSpace, sourceSize);

    blend(*sourcePreview, nd, *preview, mode, enabledChannels);

    return preview;
}

deActionFrame* deNDLayer::createActionFrame(wxWindow* parent, int layerNumber, deProject* project)
{
    return new deNDFrame(parent, *this, project->getPreviewStack(), layerNumber);
}

void deNDLayer::onChangeColorSpace(const deLayerStack& layerStack)
{
    clearEnabledChannels();
}

void deNDLayer::enableChannel(int c)
{
    enabledChannels.insert(c);
}

void deNDLayer::clearEnabledChannels()
{
    enabledChannels.clear();
}

const std::set<int>& deNDLayer::getEnabledChannels() const
{
    return enabledChannels;
}

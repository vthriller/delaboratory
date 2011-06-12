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

#include "blur_layer.h"
#include "blur.h"
#include "blur_frame.h"
#include "preview_stack.h"
#include "preview.h"
#include "project.h"

deBlurLayer::deBlurLayer(deLayerStack& _stack, const std::string& _name)
:deLayer(_stack, _name), radius(*this)
{
    direction = deBlurHorizontal;
    radius.setValue(0.01);
    clearEnabledChannels();
}

deBlurLayer::~deBlurLayer()
{
}

dePreview* deBlurLayer::createPreview(dePreviewStack& previewStack)
{
    const dePreview* sourcePreview = previewStack.getPreview(sourceLayer);

    if (!sourcePreview)
    {
        return NULL;
    }

    const deSize& sourceSize = sourcePreview->getSize();

    dePreview* preview = new dePreview(colorSpace, sourceSize);

    blur(*sourcePreview, *preview, direction, radius.getValue(), enabledChannels);

    return preview;
}

deActionFrame* deBlurLayer::createActionFrame(wxWindow* parent, int layerNumber, deProject* project)
{
    return new deBlurFrame(parent, *this, project->getPreviewStack(), layerNumber);
}

deBlurDirection deBlurLayer::getBlurDirection() const
{
    return direction;
}

deValue deBlurLayer::getBlurRadius() const
{
    return radius.getValue();
}

void deBlurLayer::setBlurRadius(deValue _radius)
{
    radius.setValue(_radius);
}

void deBlurLayer::setBlurDirection(deBlurDirection _direction)
{
    direction = _direction;
}

void deBlurLayer::onChangeColorSpace()
{
    clearEnabledChannels();
}

void deBlurLayer::enableChannel(int c)
{
    enabledChannels.insert(c);
}

void deBlurLayer::clearEnabledChannels()
{
    enabledChannels.clear();
}

const std::set<int>& deBlurLayer::getEnabledChannels() const
{
    return enabledChannels;
}

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

#include "mixer_layer.h"
#include "layer_stack.h"
#include "preview.h"
#include "mixer_frame.h"
#include "preview_stack.h"
#include "project.h"
#include "layer_dialog.h"
#include "mixer.h"

deMixerLayer::deMixerLayer(deLayerStack& _stack, int _index, const std::string& _name)
:deLayer(_stack, _index, _name), mixer(*this)
{
}

deMixerLayer::~deMixerLayer()
{
}

deActionFrame* deMixerLayer::createActionFrame(wxWindow* parent, int layerNumber, deProject* project)
{
    return new deMixerFrame(parent, *this, project->getPreviewStack(), mixer);
}

void deMixerLayer::recreateMixer()
{
    if (colorSpace.getColorSpace() == deColorSpaceInvalid)
    {
        return;
    }
    const deLayer* source = stack.getLayer(sourceLayer.getIndex());
    if (!source)
    {
        return;
    }
    //mixer.recreateMixer(source->getColorSpace(), colorSpace.getColorSpace());
    mixer.recreateMixer();
}

void deMixerLayer::onChangeColorSpace()
{
    recreateMixer();
}

void deMixerLayer::onChangeSourceLayer()
{
    recreateMixer();
}

dePreview* deMixerLayer::createPreview(dePreviewStack& previewStack)
{
    const dePreview* sourcePreview = previewStack.getPreview(sourceLayer.getIndex());

    if (!sourcePreview)
    {
        return NULL;
    }

    const deSize& sourceSize = sourcePreview->getSize();

    dePreview* preview = new dePreview(colorSpace.getColorSpace(), sourceSize);

    if (mixer.getMixer())
    {
        mixer.getMixer()->calc(*sourcePreview, *preview);
    }        

    return preview;
}

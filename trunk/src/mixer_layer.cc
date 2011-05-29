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

deMixerLayer::deMixerLayer(const std::string& _name)
:deLayer(_name)
{
    mixer = NULL;
}

deMixerLayer::~deMixerLayer()
{
    if (mixer)
    {
        delete mixer;
    }        
}

wxDialog* deMixerLayer::createDialog(wxWindow* parent, int layerNumber, deProject* project)
{
    deLayerDialog* dialog = new deLayerDialog(parent, *this, layerNumber, project, "set mixer");
    dialog->addColorSpaceChoice();
    dialog->addSourceChoice();
    return dialog;
}

deActionFrame* deMixerLayer::createActionFrame(wxWindow* parent, int layerNumber, deProject* project)
{
    return new deMixerFrame(parent, *this, project->getPreviewStack(), layerNumber);
}

void deMixerLayer::recreateMixer(const deLayerStack& layerStack)
{
    if (colorSpace == deColorSpaceInvalid)
    {
        return;
    }
    const deLayer* source = layerStack.getLayer(sourceLayer);
    if (!source)
    {
        return;
    }
    if (mixer)
    {
        delete mixer;
    }
    mixer = new deMixer(source->getColorSpace(), colorSpace);
}

void deMixerLayer::changeColorSpace(deColorSpace _colorSpace, const deLayerStack& layerStack)
{
    colorSpace = _colorSpace;
    recreateMixer(layerStack);
}

void deMixerLayer::changeSourceLayer(int id, const deLayerStack& layerStack)
{
    sourceLayer = id;
    deLayer* source = layerStack.getLayer(sourceLayer);
    if (source)
    {
        colorSpace = source->getColorSpace();
    }        
    recreateMixer(layerStack);
}


dePreview* deMixerLayer::createPreview(dePreviewStack& previewStack)
{
    const dePreview* sourcePreview = previewStack.getPreview(sourceLayer);

    if (!sourcePreview)
    {
        return NULL;
    }

    const deSize& sourceSize = sourcePreview->getSize();

    dePreview* preview = new dePreview(colorSpace, sourceSize);

    if (mixer)
    {
        mixer->calc(*sourcePreview, *preview);
    }        

    return preview;
}

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

#include "rgb2bw_layer.h"
#include "preview_stack.h"
#include "blend.h"
#include "preview.h"


deRGB2BWLayer::deRGB2BWLayer(deLayerStack& _stack, int _index, const std::string& _name)
:deLayer(_stack, _index, _name), alphaR(*this, "alpha R", 0.0, 1.0), alphaB(*this, "alpha B", 0.0, 1.0)
{
    alphaR.setValue(0.0);
    alphaB.setValue(0.0);

    colorSpace.setColorSpace(deColorSpaceBW);
}

deRGB2BWLayer::~deRGB2BWLayer()
{
}

bool deRGB2BWLayer::updatePreview(const dePreview* sourcePreview, dePreview* preview)
{
    dePreview* tmp = new dePreview(deColorSpaceBW, preview->getRawSize());
    blend(*(sourcePreview->getChannel(1)), *(sourcePreview->getChannel(0)), *(tmp->getChannel(0)), alphaR.getValue(), deBlendOverlay, NULL);
    blend(*(tmp->getChannel(0)), *(sourcePreview->getChannel(2)), *(preview->getChannel(0)), alphaB.getValue(), deBlendOverlay, NULL);
    delete tmp;

}


void deRGB2BWLayer::saveSpecific(xmlNodePtr node)
{
}

void deRGB2BWLayer::loadSpecific(xmlNodePtr node)
{
}

void deRGB2BWLayer::updatePreview(dePreviewStack& previewStack)
{
    const dePreview* sourcePreview = previewStack.getPreview(getSourceLayerID());
    dePreview* preview = previewStack.getPreview(index);

    if ((sourcePreview) && (preview))
    {
        updatePreview(sourcePreview, preview);
    }
}

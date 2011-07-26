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

#include "blend_blur_layer.h"
#include "blur.h"
#include "preview_stack.h"
#include "preview.h"
#include "project.h"
#include "blend.h"
#include "channel.h"

deBlendBlurLayer::deBlendBlurLayer(deLayerStack& _stack, int _index, const std::string& _name)
:deLayer(_stack, _index, _name), radiusX(*this, "radius x", 0.0, 0.1), radiusY(*this, "radius y", 0.0, 0.1), channels(*this)
{
    radiusX.setValue(0.02);
    radiusY.setValue(0.02);
}

deBlendBlurLayer::~deBlendBlurLayer()
{
}

dePreview* deBlendBlurLayer::createPreview(dePreviewStack& previewStack)
{
    const dePreview* sourcePreview = previewStack.getPreview(sourceLayer.getIndex());

    if (!sourcePreview)
    {
        return NULL;
    }

    const deSize& sourceSize = previewStack.getPreviewSize();

    dePreview* preview = new dePreview(colorSpace.getColorSpace(), sourceSize);

    updatePreview(sourcePreview, preview);

    return preview;
}

bool deBlendBlurLayer::updatePreview(const dePreview* sourcePreview, dePreview* preview)
{
    deColorSpace sourceColorSpace = sourcePreview->getColorSpace();

    dePreview* tmp1 = new dePreview(sourceColorSpace, preview->getRawSize());
    dePreview* tmp2 = new dePreview(sourceColorSpace, preview->getRawSize());

    deBlurType type = deGaussianBlur;
    deValue t = 0;

    deChannels c = channels.getChannels();

    blur(*sourcePreview, *tmp1, deBlurHorizontal, radiusX.getValue(), c, type, t);
    blur(*tmp1, *tmp2, deBlurVertical, radiusY.getValue(), c, type, t);
    blend(*sourcePreview, *tmp2, 1.0, *tmp1, c, deBlendOverlay, NULL);

    int size = preview->getChannel(0)->getSize().getN();

    int n = getColorSpaceSize(colorSpace.getColorSpace());
    int m = getColorSpaceSize(sourceColorSpace);

    if (n == 1)
    {
        deBaseChannel* destination = preview->getChannel(0);
        int j;
        for (j = 0; j < size; j++)
        {
            deValue v = 0.0;
            int i = 1;
            {
                v= tmp1->getChannel(i)->getValue(j);
            }                    
            destination->setValue(j, v);
        }
    }
    else
    {
        int i;
        for (i = 0; i < n; i++)
        {
            deBaseChannel* source = tmp1->getChannel(i);
            deBaseChannel* destination = preview->getChannel(i);
            int j;
            for (j = 0; j < size; j++)
            {
                deValue v = source->getValue(j);
                destination->setValue(j, v );
            }
        }
    }

    delete tmp1;
    delete tmp2;
}


void deBlendBlurLayer::onChangeColorSpace()
{
    channels.fill();
}

void deBlendBlurLayer::saveSpecific(xmlNodePtr node)
{
    radiusX.save(node, "radius_x");
    radiusY.save(node, "radius_y");
    channels.save(node, "channels");
}

void deBlendBlurLayer::loadSpecific(xmlNodePtr node)
{
    xmlNodePtr child = node->xmlChildrenNode;

    while (child)
    {
        if ((!xmlStrcmp(child->name, xmlCharStrdup("radius_x")))) 
        {
            radiusX.load(child);
        }

        if ((!xmlStrcmp(child->name, xmlCharStrdup("radius_y")))) 
        {
            radiusY.load(child);
        }

        if ((!xmlStrcmp(child->name, xmlCharStrdup("channels")))) 
        {
            channels.load(child);
        }

        child = child->next;
    }
}


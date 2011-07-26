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

#include "grain_layer.h"
#include "layer_stack.h"
#include "preview.h"
#include "preview_stack.h"
#include "project.h"
#include "grain.h"

deGrainLayer::deGrainLayer(deLayerStack& _stack, int _index, const std::string& _name)
:deLayer(_stack, _index, _name)
{
}

deGrainLayer::~deGrainLayer()
{
}


void deGrainLayer::onChangeColorSpace()
{
}

dePreview* deGrainLayer::createPreview(dePreviewStack& previewStack)
{
    const deSize& sourceSize = previewStack.getPreviewSize();

    dePreview* preview = new dePreview(colorSpace.getColorSpace(), sourceSize);

    fillGrain(preview);

    return preview;
}

void deGrainLayer::saveSpecific(xmlNodePtr node)
{
    //grain.save(node, "grain");
}

void deGrainLayer::loadSpecific(xmlNodePtr node)
{
/*    xmlNodePtr child = node->xmlChildrenNode;

    while (child)
    {
        if ((!xmlStrcmp(child->name, xmlCharStrdup("grain")))) 
        {
            grain.load(child);
        }

        child = child->next;
    }*/
}

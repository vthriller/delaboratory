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

#include "curves_layer.h"
#include "preview_stack.h"
#include "layer_stack.h"
#include "size.h"
#include "preview.h"
#include "project.h"

#define CURVES_PANEL_SIZE 500

deCurvesLayer::deCurvesLayer(deLayerStack& _stack, int _index, const std::string& _name)
:deLayer(_stack, _index, _name), curves(*this, CURVES_PANEL_SIZE)
{
}

deCurvesLayer::~deCurvesLayer()
{
}

void deCurvesLayer::resetCurves()
{
    curves.resetCurves();
}

void deCurvesLayer::onChangeColorSpace()
{
    resetCurves();
}

dePreview* deCurvesLayer::createPreview(dePreviewStack& previewStack)
{
    const dePreview* sourcePreview = previewStack.getPreview(sourceLayer.getIndex());

    if (!sourcePreview)
    {
        return NULL;
    }

    const deSize& sourceSize = sourcePreview->getSize();

    dePreview* preview = new dePreview(colorSpace.getColorSpace(), sourceSize);

    updatePreview(sourcePreview, preview);

    return preview;
}

bool deCurvesLayer::updatePreview(const dePreview* sourcePreview, dePreview* preview)
{
    processCurves(*sourcePreview, *preview);
}

void deCurvesLayer::processCurves(const dePreview& source, dePreview& destination)
{
    int n = getColorSpaceSize(colorSpace.getColorSpace());
    int i;
    for (i = 0; i < n; i++)
    {
        deCurve* curve = curves.getCurve(i);
        if (curve)
        {
            const deBaseChannel* sourceChannel = source.getChannel(i);
            deBaseChannel* destinationChannel = destination.getChannel(i);
            curve->process(*sourceChannel, *destinationChannel);
        }            
    }

}

void deCurvesLayer::saveSpecific(xmlNodePtr node)
{
    curves.save(node, "curves");
}

void deCurvesLayer::loadSpecific(xmlNodePtr node)
{
    xmlNodePtr child = node->xmlChildrenNode;

    while (child)
    {
        if ((!xmlStrcmp(child->name, xmlCharStrdup("curves")))) 
        {
            curves.load(child);
        }

        child = child->next;
    }
}

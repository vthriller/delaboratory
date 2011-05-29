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
#include "layer_dialog.h"
#include "preview_stack.h"
#include "layer_stack.h"
#include "size.h"
#include "preview.h"
#include "curves_frame.h"
#include "project.h"

#define CURVES_PANEL_SIZE 500

deCurvesLayer::deCurvesLayer(const std::string& _name)
:deLayer(_name)
{
}

deCurvesLayer::~deCurvesLayer()
{
    deleteCurves();
}

void deCurvesLayer::resetCurves()
{
    int n = getColorSpaceSize(colorSpace);
    deleteCurves();
    int i;
    for (i = 0; i < n; i++)
    {
        curves.push_back(new deCurve(CURVES_PANEL_SIZE));
    }
}

void deCurvesLayer::deleteCurves()
{
    while (curves.size() > 0)
    {
        deCurves::iterator i = curves.begin();
        delete (*i);
        curves.erase(i);
    }
}

void deCurvesLayer::changeSourceLayer(int id, const deLayerStack& layerStack)
{
    sourceLayer = id;
    deLayer* source = layerStack.getLayer(sourceLayer);
    if (source)
    {
        if (source->getColorSpace() != colorSpace)
        {
            colorSpace = source->getColorSpace();
            resetCurves();
        }
    }        
}

void deCurvesLayer::changeColorSpace(deColorSpace _colorSpace, const deLayerStack& layerStack)
{
    if (colorSpace != _colorSpace)
    {
        colorSpace = _colorSpace;
        resetCurves();
    }        
}

dePreview* deCurvesLayer::createPreview(dePreviewStack& previewStack)
{
    const dePreview* sourcePreview = previewStack.getPreview(sourceLayer);

    if (!sourcePreview)
    {
        return NULL;
    }

    const deSize& sourceSize = sourcePreview->getSize();

    dePreview* preview = new dePreview(colorSpace, sourceSize);

    processCurves(*sourcePreview, *preview);
    return preview;
}

wxDialog* deCurvesLayer::createDialog(wxWindow* parent, int layerNumber, deProject* project)
{
    deLayerDialog* dialog = new deLayerDialog(parent, *this, layerNumber, project, "set curves");
    dialog->addSourceChoice();
    return dialog;
}

deActionFrame* deCurvesLayer::createActionFrame(wxWindow* parent, int layerNumber, deProject* project)
{
    return new deCurvesFrame(parent, *this, project->getPreviewStack(), layerNumber);
}

void deCurvesLayer::processCurves(const dePreview& source, dePreview& destination)
{
    int n = getColorSpaceSize(colorSpace);
    int i;
    for (i = 0; i < n; i++)
    {
        deCurve& curve = *(curves.at(i));
        const deBaseChannel* sourceChannel = source.getChannel(i);
        deBaseChannel* destinationChannel = destination.getChannel(i);
        curve.process(*sourceChannel, *destinationChannel);
    }

}

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

#include "operation_processor.h"
#include "layer_processor.h"
#include "logger.h"
#include "project.h"

deOperationProcessor::deOperationProcessor(deLayerProcessor& _layerProcessor, deProject& _project)
:layerProcessor(_layerProcessor), project(_project)
{
}

deOperationProcessor::~deOperationProcessor()
{
}

void deOperationProcessor::addNewLayerOnTop(deBaseLayer* layer)
{
    logInfo("before add new layer on top");
    layerProcessor.addLayerInLayerProcessor(layer);
    logInfo("after add new layer on top");
}

void deOperationProcessor::removeTopLayer()
{
    logInfo("before remove top layer");
    layerProcessor.removeTopLayerInLayerProcessor();
    logInfo("after remove top layer");
}

void deOperationProcessor::execute(const std::string& operation)
{
    if (operation == "remove")
    {
        removeTopLayer();
        return;
    }

    deBaseLayer* layer = project.createNewLayer(operation);

    if (layer)
    {
        addNewLayerOnTop(layer);
        project.onAddNewLayer();
    }
}

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
#include "layer_stack.h"
#include "base_layer_with_properties.h"
#include "color_space_utils.h"

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
        project.onRemoveTopLayer();
        return;
    }

    bool ok = false;
    if (tryExecuteBasicOperation(operation))
    {
        ok = true;
    }
    else
    {
        deBaseLayer* layer = project.createNewLayer(operation);

        if (layer)
        {
            addNewLayerOnTop(layer);
            ok = true;
        }
    }        

    if (ok)
    {
        project.onAddNewLayer();
    }        
}

void deOperationProcessor::initProfile(const std::string& profile)
{
    if (profile == "cmyk_auto_levels")
    {
        execute("CMYK");
        execute("levels");
    }
    if (profile == "bw")
    {
        execute("BW");
    }
    if (profile == "c2g")
    {
        execute("c2g");
    }
    if (profile == "vignette")
    {
        execute("LAB");
        execute("vignette");
    }
}

void getSupportedBasicOperations(std::vector<std::string>& actions)
{
    actions.push_back("CMYK curves");
    actions.push_back("LAB vignette");
    actions.push_back("LAB local contrast");
    actions.push_back("RGB curves");
    actions.push_back("LAB curves");
}

bool deOperationProcessor::executeOperation(deColorSpace colorSpace, const std::string& operation)
{
    deViewManager& viewManager = project.getViewManager();

    viewManager.setLastView();

    deColorSpace currentColorSpace = viewManager.getColorSpace();

    if (colorSpace != currentColorSpace)
    {
        deBaseLayer* layer1 = project.createNewLayer(getColorSpaceName(colorSpace));

        if (layer1)
        {
            addNewLayerOnTop(layer1);
            viewManager.setLastView();
        }
        else
        {
            return false;
        }
    }        

    deBaseLayer* layer2 = project.createNewLayer(operation);

    if (layer2)
    {
        addNewLayerOnTop(layer2);
        viewManager.setLastView();
    }
    else
    {
        return false;
    }

    project.openLayerFrame(project.getLayerStack().getSize() - 1);

    return true;
}

bool deOperationProcessor::tryExecuteBasicOperation(const std::string& operation)
{
    if (operation == "CMYK curves")
    {
        return executeOperation(deColorSpaceCMYK, "curves");
    }

    if (operation == "LAB vignette")
    {
        return executeOperation(deColorSpaceLAB, "vignette");
    }

    if (operation == "LAB local contrast")
    {
        return executeOperation(deColorSpaceLAB, "local_contrast");
    }

    if (operation == "RGB curves")
    {
        return executeOperation(deColorSpaceRGB, "curves");
    }

    if (operation == "LAB curves")
    {
        return executeOperation(deColorSpaceLAB, "curves");
    }

    return false;
}


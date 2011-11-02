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

#include "layer_processor.h"
#include "main_frame.h"
#include "layer_stack.h"
#include "view_manager.h"
#include <string>


deLayerProcessor::deLayerProcessor()
{
    mainFrame = NULL;
    stack = NULL;
    viewManager = NULL;
}

deLayerProcessor::~deLayerProcessor()
{
}

void deLayerProcessor::setMainFrame(deMainFrame* _mainFrame)
{
    mainFrame = _mainFrame;
}

void deLayerProcessor::setLayerStack(deLayerStack* _layerStack)
{
    stack = _layerStack;
}

void deLayerProcessor::setViewManager(deViewManager* _viewManager)
{
    viewManager = _viewManager;
}

void deLayerProcessor::repaintImage(bool calcHistogram)
{
    if (mainFrame)
    {
        mainFrame->repaint(calcHistogram);
    }
}

void deLayerProcessor::updateAllImages(bool calcHistogram)
{
    if (stack)
    {
        int view = viewManager->getView();
        stack->updateImages(0, view);
    }        
    repaintImage(calcHistogram);
}    

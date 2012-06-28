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

#include "gui.h"
#include "view_mode_panel.h"
#include "layer_grid_panel.h"
#include "layer_frame_manager.h"
#include "window_wx.h"
#include "base_layer.h"
#include "generic_layer_frame.h"
#include "histogram_mode_panel.h"
#include "image_area_panel.h"
#include "logger.h"

deGUI::deGUI()
{
    viewModePanel = NULL;
    layerGridPanel = NULL;
    histogramModePanel = NULL;
    imageAreaPanel = NULL;
}

deGUI::~deGUI()
{
}

void deGUI::setViewModePanel(deViewModePanel* _viewModePanel)
{
    viewModePanel = _viewModePanel;
}

void deGUI::setLayerGridPanel(deLayerGridPanel* _panel)
{
    layerGridPanel = _panel;
}

void deGUI::setHistogramModePanel(deHistogramModePanel* _histogramModePanel)
{
    histogramModePanel = _histogramModePanel;
}


void deGUI::updateViewModePanelMode()
{
    if (viewModePanel)
    {
        viewModePanel->updateMode();
    }
}    

void deGUI::updateViewModePanelNames()
{
    if (viewModePanel)
    {
        viewModePanel->updateNames();
    }
}    

void deGUI::updateLayerGrid()
{
    if (layerGridPanel)
    {
        layerGridPanel->update();
    }        
}    

void deGUI::openLayerFrame(deBaseLayer& layer, deLayerProcessor& layerProcessor, deLayerFrameManager& layerFrameManager, int index)
{
    if (!layerFrameManager.checkLayerFrame(index))
    {
        deWindowWX window(layerGridPanel);
        const std::string name = layer.getType();

        deFrame* frame = new deGenericLayerFrame(window, name, layer, layerProcessor, layerFrameManager, index);
        if (frame)
        {
            frame->show();
        }
    }        
}    

void deGUI::updateHistogramMode(int channel)
{
    if (histogramModePanel)
    {
        histogramModePanel->updateMode(channel);
    }            
}    

void deGUI::updateHistogramNames()
{
    if (histogramModePanel)
    {
        histogramModePanel->updateNames();
    }
}    

void deGUI::setImageAreaPanel(deImageAreaPanel* _imageAreaPanel)
{
    imageAreaPanel = _imageAreaPanel;
}

void deGUI::updateImageAreaSize()
{
    if (imageAreaPanel)
    {
        imageAreaPanel->updateSize(false);
    }        
}    

void deGUI::lockSize()
{
    if (imageAreaPanel)
    {
        imageAreaPanel->lockSize();
    }        
    else
    {
        logError("can't lockSize, no imageAreaPanel");
    }
}

void deGUI::unlockSize()
{
    if (imageAreaPanel)
    {
        imageAreaPanel->unlockSize();
    }
    else
    {
        logError("can't unlockSize, no imageAreaPanel");
    }
}


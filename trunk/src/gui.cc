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
#include "image_panel.h"
#include "histogram_panel.h"
#include "histogram_mode_panel.h"
#include "sampler_list_panel.h"
#include "info_bar_panel.h"
#include "layer_list_panel.h"
#include "view_mode_panel.h"
#include "center_panel.h"

deGUI::deGUI()
{
    imagePanel = NULL;
    histogramPanel = NULL;
    histogramMode = -1;
    samplerListPanel = NULL;
    layerListPanel = NULL;
    viewModePanel = NULL;
    renderer = NULL;
    histogramModePanel = NULL;
    layerListPanel = NULL;
}

deGUI::~deGUI()
{
}

void deGUI::setImagePanel(deImagePanel* panel)
{
    imagePanel = panel;
}

void deGUI::refreshView()
{
    if (imagePanel)
    {
        imagePanel->Refresh();
    }
    if (histogramPanel)
    {
        histogramPanel->Refresh();
    }            
    updateSamplers();
    updateHistogramPanel();
}    

void deGUI::updateHistogramPanel()
{
    if (histogramPanel)
    {
        histogramPanel->updateHistograms();
    }        
}

void deGUI::setHistogramPanel(deHistogramPanel* panel)
{
    histogramPanel = panel;
}

void deGUI::setHistogramMode(int mode)
{
    histogramMode = mode;
    if (histogramPanel)
    {
        updateHistogramPanel();
        histogramPanel->Refresh();
    }            
}

int deGUI::getHistogramMode() const
{
    return histogramMode;
}

void deGUI::setHistogramModePanel(deHistogramModePanel* panel)
{
    histogramModePanel = panel;    
}

void deGUI::updateHistogramModeButtons(deColorSpace colorSpace)
{
    if (histogramModePanel)
    {
        histogramModePanel->updateButtons(colorSpace);
    }        
}    

void deGUI::setSamplerListPanel(deSamplerListPanel* panel)
{
    samplerListPanel = panel;
}

void deGUI::updateSamplers()
{
    if (samplerListPanel)
    {
        samplerListPanel->updateSamplers();
    }        
}

void deGUI::setInfoBarPanel(deInfoBarPanel* panel)
{
    infoBarPanel = panel;
}

void deGUI::setInfo(int index, const std::string& s)
{
#ifdef DE_PROFILER
    if (infoBarPanel)
    {
        infoBarPanel->setInfo(index, s);
    }
#endif    
}

void deGUI::setLayerListPanel(deLayerListPanel* _panel)
{
    layerListPanel = _panel;
}

void deGUI::updateLayerListPanel()
{
    if (layerListPanel)
    {
        layerListPanel->update();
    }
}

void deGUI::updateViewModePanel(deColorSpace colorSpace)
{
    if (viewModePanel)
    {
        viewModePanel->updateButtons(colorSpace);
    }        
}    

void deGUI::setViewModePanel(deViewModePanel* panel)
{
    viewModePanel = panel;
}

void deGUI::updateAfterSetView(deColorSpace colorSpace)
{
    updateLayerListPanel();
    updateViewModePanel(colorSpace);
    updateHistogramModeButtons(colorSpace);
}

void deGUI::setCenterPanel(deCenterPanel* _centerPanel)
{
    centerPanel = _centerPanel;
}

void deGUI::updateCenterPanel()
{
    if (centerPanel)
    {
        centerPanel->updateSize();
    }        
}

void deGUI::setRenderer(deRenderer* _renderer)
{
    renderer = _renderer;
}

void deGUI::setViewMode(const deViewMode& mode, int channel)
{
    if (renderer)
    {
        renderer->setViewMode(mode, channel);
    }        
}

void deGUI::updateSamplerList()
{
    if (samplerListPanel)
    {
        samplerListPanel->updateList();
    }        
}    

void deGUI::start()
{
    updateViewModePanel(deColorSpaceRGB);
    updateHistogramModeButtons(deColorSpaceRGB);
    updateLayerListPanel();
}


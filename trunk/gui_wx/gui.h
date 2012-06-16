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

#ifndef _DE_GUI_H
#define _DE_GUI_H

class deViewModePanel;
class deLayerGridPanel;
class deLayerProcessor;
class deLayerFrameManager;
class deBaseLayer;
class deHistogramModePanel;
class deImageAreaPanel;

class deGUI
{
    private:
        deGUI(const deGUI& gui);
        deGUI& operator =(const deGUI& gui);

        deViewModePanel* viewModePanel;
        deLayerGridPanel* layerGridPanel;
        deHistogramModePanel* histogramModePanel;
        deImageAreaPanel* imageAreaPanel;

    public:
        deGUI();

        virtual ~deGUI();

        void setViewModePanel(deViewModePanel* _viewModePanel);
        void updateViewModePanelMode();
        void updateViewModePanelNames();

        void setLayerGridPanel(deLayerGridPanel* _panel);
        void updateLayerGrid();

        void openLayerFrame(deBaseLayer& layer, deLayerProcessor& layerProcessor, deLayerFrameManager& layerFrameManager, int index);

        void setHistogramModePanel(deHistogramModePanel* _histogramModePanel);
        void updateHistogramMode(int channel);
        void updateHistogramNames();

        void setImageAreaPanel(deImageAreaPanel* _imageAreaPanel);
        void updateImageAreaSize();


};

#endif

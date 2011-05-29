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

#include <string>
#include "view_mode.h"
#include "color_space.h"
class deImagePanel;
class deHistogramPanel;
class deHistogramModePanel;
class deProject;
class deSamplerListPanel;
class deInfoBarPanel;
class deLayerListPanel;
class deViewModePanel;
class deCenterPanel;
class deRenderer;

class deGUI
{
    private:
        deImagePanel* imagePanel;
        deHistogramPanel* histogramPanel;
        int histogramMode;
        deHistogramModePanel* histogramModePanel;
        deSamplerListPanel* samplerListPanel;
        deInfoBarPanel* infoBarPanel;
        deLayerListPanel* layerListPanel;
        deViewModePanel* viewModePanel;
        deCenterPanel* centerPanel;
        deRenderer* renderer;

    public:    
        deGUI();
        virtual ~deGUI();

        void setImagePanel(deImagePanel* panel);
        void refreshView();
        void updateHistogramPanel();
        void setHistogramPanel(deHistogramPanel* panel);
        void setHistogramMode(int mode);
        int getHistogramMode() const;
        void setHistogramModePanel(deHistogramModePanel* panel);
        void updateHistogramModeButtons(deColorSpace colorSpace);
        void setSamplerListPanel(deSamplerListPanel* panel);
        void updateSamplers();
        void setInfoBarPanel(deInfoBarPanel* panel);
        void setInfo(int index, const std::string& s);
        void setLayerListPanel(deLayerListPanel* _panel);
        void updateLayerListPanel();
        void setViewModePanel(deViewModePanel* panel);
        void updateViewModePanel(deColorSpace colorSpace);
        void updateAfterSetView(deColorSpace colorSpace);
        void setCenterPanel(deCenterPanel* _centerPanel);
        void updateCenterPanel();
        void setRenderer(deRenderer* _renderer);
        void setViewMode(const deViewMode& mode, int channel);
        void updateSamplerList();
        void start();

};

#endif

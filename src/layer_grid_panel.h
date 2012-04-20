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

#ifndef _DE_LAYER_GRID_PANEL_H
#define _DE_LAYER_GRID_PANEL_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <vector>

class deProject;
class deLayerProcessor;
class deChannelManager;

class deLayerGridPanel:public wxPanel
{
    class deLayerRow
    {
        public:
            deLayerRow(int _index)
            :index(_index)
            {
            };

            int index;
            wxStaticText* id;
            wxRadioButton* view;
            wxButton* action;
            wxButton* blend;
            wxCheckBox* enabled;
    };

    private:
        std::vector<deLayerRow> layerRows;

        deProject& project;
        deLayerProcessor& layerProcessor;

        wxSizer* mainSizer;

        wxFlexGridSizer* gridSizer;
        deChannelManager& channelManager;

        int maxRows;

        void check(wxCommandEvent &event);
        void select(wxCommandEvent &event);
        void click(wxCommandEvent &event);
    
    public:
        deLayerGridPanel(wxWindow* parent, deProject& _project, deLayerProcessor& _processor, deChannelManager& _channelManager);
        ~deLayerGridPanel();

        void buildRows();
        void clearRows();

};

#endif

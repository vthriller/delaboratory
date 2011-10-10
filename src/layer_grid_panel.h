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
            wxStaticText* name;
            wxButton* action;
            wxButton* blend;
            wxCheckBox* enabled;
    };

    private:
        std::vector<deLayerRow> layerRows;

        deProject* project;

        wxSizer* mainSizer;

        wxFlexGridSizer* gridSizer;

        int maxRows;

        /*

        wxButton* addCurvesQuick;
        wxButton* addMixerQuick;
        wxButton* addLABQuick;
        wxButton* addLCHQuick;
        wxButton* addRGBQuick;

        wxButton* addCurves;
        wxButton* addLAB;
        wxButton* addRGB;
        wxButton* addHSV;
        wxButton* addHSL;
        wxButton* deleteLayer;

        wxButton* exportTIFF;
        wxButton* externalEditor;
        */
    /*
        int selected;

        wxListCtrl* listbox;
        wxButton* buttonAdd;
        wxButton* buttonRemove;
        wxButton* buttonView;

        std::vector<std::string> items;

        void select(wxCommandEvent &event);
        void deselect(wxCommandEvent &event);
        void activate(wxCommandEvent &event);
        void right_click(wxCommandEvent &event);
        void clickAdd(wxCommandEvent &event);
        void clickRemove(wxCommandEvent &event);
        void clickView(wxCommandEvent &event);
        void fill(std::vector<std::string>& names);
        */

        void check(wxCommandEvent &event);
        void select(wxCommandEvent &event);
        void click(wxCommandEvent &event);
    
    public:
        deLayerGridPanel(wxWindow* parent, deProject* _project);
        ~deLayerGridPanel();

        void repaintImage();
        void buildRows();
        void clearRows();


//      void update();
        
};

#endif

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

#ifndef _DE_LAYER_LIST_PANEL_H
#define _DE_LAYER_LIST_PANEL_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <vector>

class deProject;

class deLayerListPanel:public wxPanel
{
    private:
        int selected;

        wxListCtrl* listbox;
        wxButton* buttonAdd;
        wxButton* buttonRemove;
        wxButton* buttonView;
        deProject* project;

        std::vector<std::string> items;

        void select(wxCommandEvent &event);
        void deselect(wxCommandEvent &event);
        void activate(wxCommandEvent &event);
        void right_click(wxCommandEvent &event);
        void clickAdd(wxCommandEvent &event);
        void clickRemove(wxCommandEvent &event);
        void clickView(wxCommandEvent &event);
        void fill(std::vector<std::string>& names);
    
    public:
        deLayerListPanel(wxWindow* parent, deProject* _project);
        ~deLayerListPanel();


        void update();
        
};

#endif

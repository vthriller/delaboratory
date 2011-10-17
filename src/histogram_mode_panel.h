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

#ifndef _DE_HISTOGRAM_MODE_PANEL_H
#define _DE_HISTOGRAM_MODE_PANEL_H

#include <wx/wx.h>
#include <vector>
/*
#include <map>
#include "color_space.h"
*/
class deProject;

class deHistogramModePanel:public wxPanel
{
    private:
        std::vector<wxRadioButton*> buttons;
        deProject& project;

        void select(wxCommandEvent &event);
    public:
        deHistogramModePanel(wxWindow* parent, deProject& _project);
        virtual ~deHistogramModePanel();

        void updateNames();
        void updateMode();

};

#endif

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

#ifndef _DE_CREATE_LAYER_DIALOG_H
#define _DE_CREATE_LAYER_DIALOG_H

#include "project.h"

class deCreateLayerDialog:public wxDialog
{
    private:
        deProject& project;

        wxChoice* source;
        wxButton* ok;
        wxButton* cancel;
        wxTextCtrl* nameEdit;

        void click(wxCommandEvent &event);
        void close(wxCommandEvent &event);
        void doCreateLayer();

        std::string type;

    public:
        deCreateLayerDialog(wxWindow *parent, const std::string& _type, deProject& _project);
        virtual ~deCreateLayerDialog();
};

#endif

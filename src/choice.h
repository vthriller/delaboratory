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

#ifndef _DE_CHOICE_H
#define _DE_CHOICE_H

#include <wx/wx.h>
#include <vector>
#include <string>

class deChoice:public wxPanel
{
    private:
        wxSizer* sizer;
        wxStaticText* label;
        wxChoice* choice;

        void choose(wxCommandEvent &event);

    public:
        deChoice(wxWindow *parent, const std::string& labelString, const std::vector<std::string>& choices);
        virtual ~deChoice();

        virtual void onChoose(int c) = 0;

        void set(int index);
};        

#endif

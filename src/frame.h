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

#ifndef _DE_FRAME_H
#define _DE_FRAME_H

#include <wx/wx.h>
#include <string>
class deWindow;
class dePanel;

class deFrameOld:public wxFrame
{
    protected:
    public:
        deFrameOld(wxWindow *parent, const std::string& name);
        virtual ~deFrameOld();

        virtual void onKey(int key) {};

        virtual void onUpdateProperties() {};

};


class deFrameImpl;

class deFrame
{
    private:
        deFrameImpl* impl;

    protected:
        void addWidget(deWindow& window);
        void addWidget(const std::string& _name, deWindow& window);
        void addSizer(const std::string& _name);

    public:
        deFrame(deWindow& parent, const std::string& name);
        virtual ~deFrame();

        void show();

        deWindow& getWindow();

        void fit();

        bool clearImpl();
};


#endif

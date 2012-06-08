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

#include "button.h"
#include <sstream>
#include "layer_processor.h"
#include "panel_wx.h"
#include "window_wx.h"

class deButtonImpl:public dePanelWX
{
    private:
        deButton& parent;
        wxButton* button;
    public:
        deButtonImpl(deButton& _parent, deWindow& _parentWindow, const std::string& _name)
        :dePanelWX(_parentWindow), parent(_parent)
        {
            button = new wxButton(this, wxID_ANY, wxString::FromAscii(_name.c_str()));

            Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deButtonImpl::click));
        }

        virtual ~deButtonImpl()
        {
        }

        void click(wxCommandEvent &event)
        {
            parent.onClick();
        }


};

deButton::deButton(deWindow& window, const std::string& _name)
{
    deWindowWX* w = dynamic_cast<deWindowWX*>(&window);
    if (w)
    {
        impl = new deButtonImpl(*this, window, _name);
    }
    else
    {
        impl = NULL;
    }
}

deButton::~deButton()
{
    if (impl)
    {
        delete impl;
    }        
}

deWindow& deButton::getWindow()
{
    return impl->getWindow();
}

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

#include "check_box.h"
#include "window_wx.h"
#include "panel_wx.h"

class deCheckBoxImpl:public dePanelWX
{
    private:
        deCheckBox& parent;
        wxCheckBox* checkBox;
    public:
        deCheckBoxImpl(deCheckBox& _parent, deWindow& _parentWindow, const std::string& _name)
        :dePanelWX(_parentWindow), parent(_parent)
        {
            wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
            SetSizer(sizer);

            checkBox =  new wxCheckBox(this, wxID_ANY, wxString::FromAscii(_name.c_str()));
            sizer->Add(checkBox);

            Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(deCheckBoxImpl::check));
        }

        void check(wxCommandEvent &event)
        {
            if (checkBox->IsChecked())
            {
                parent.onCheck(true);
            }
            else
            {
                parent.onCheck(false);
            }
        }

        void set(bool b)
        {
            if (b)
            {
                checkBox->SetValue(1);
            }
            else
            {
                checkBox->SetValue(0);
            }
        }
};        

deCheckBox::deCheckBox(deWindow& window, const std::string& _name)
{
    deWindowWX* w = dynamic_cast<deWindowWX*>(&window);
    if (w)
    {
        impl = new deCheckBoxImpl(*this, window, _name);
    }
    else
    {
        impl = NULL;
    }
}

deCheckBox::~deCheckBox()
{
    if (impl)
    {
        delete impl;
    }
}

void deCheckBox::set(bool c)
{
    if (impl)
    {
        impl->set(c);
    }
}

deWindow& deCheckBox::getWindow()
{
    return impl->getWindow();
}

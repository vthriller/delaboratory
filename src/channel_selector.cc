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

#include "channel_selector.h"
#include <sstream>
#include "layer_processor.h"
#include "panel_wx.h"
#include "window_wx.h"
#include "logger.h"
#include "str.h"
#include "color_space_utils.h"

class deChannelSelectorImpl:public dePanelWX
{
    private:
        deChannelSelector& parent;
        deColorSpace colorSpace;
        std::vector<wxRadioButton*> buttons;
    public:
        deChannelSelectorImpl(deChannelSelector& _parent, deWindow& _parentWindow, deColorSpace _colorSpace)
        :dePanelWX(_parentWindow), parent(_parent), colorSpace(_colorSpace)
        {
            wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
            SetSizer(sizer);

            int n = getColorSpaceSize(colorSpace);

            int i;
            for (i = 0; i < n; i++)
            {
                int style = 0;
                if (i == 0)
                {
                    style = wxRB_GROUP;
                }
                std::string s = getChannelName(colorSpace, i);
                wxRadioButton* b = new wxRadioButton(this, wxID_ANY, wxString::FromAscii(s.c_str()), wxDefaultPosition, wxDefaultSize, style);
                sizer->Add(b);
                buttons.push_back(b);
            }        

            Connect(wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(deChannelSelectorImpl::select));

        }

        virtual ~deChannelSelectorImpl()
        {
            logInfo("~deChannelSelectorImpl");
        }

        void setValue(int channel)
        {
            buttons[channel]->SetValue(1);
        }

        void select(wxCommandEvent &event)
        {
            int i = event.GetId();
            int j;
            for (j = 0; j < buttons.size(); j++)
            {
                if (buttons[j]->GetId() == i)
                {
                    parent.onValueChange(j);
                }
            }

        }


};

deChannelSelector::deChannelSelector(deWindow& window, deColorSpace colorSpace)
{
    logInfo("deChannelSelector constructor");
    deWindowWX* w = dynamic_cast<deWindowWX*>(&window);
    if (w)
    {
        impl = new deChannelSelectorImpl(*this, window, colorSpace);
    }
    else
    {
        impl = NULL;
    }
}

deChannelSelector::~deChannelSelector()
{
    logInfo("deChannelSelector destructor");
    if (impl)
    {
        delete impl;
    }        
}

void deChannelSelector::setValue(int channel)
{
    if (impl)
    {
        impl->setValue(channel);
    }        
}

deWindow& deChannelSelector::getWindow()
{
    return impl->getWindow();
}

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

#include "choice.h"
#include "window_wx.h"
#include "panel_wx.h"

class deChoiceImpl:public dePanelWX
{
    private:
        deChoice& parent;
        wxChoice* choice;
    public:
        deChoiceImpl(deChoice& _parent, deWindow& _parentWindow, const std::string& _name, const std::vector<std::string>& choices)
        :dePanelWX(_parentWindow), parent(_parent)
        {
            wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
            SetSizer(sizer);

            if (_name.size() > 0)
            {
                int widthn = 100;
                wxStaticText* label = new wxStaticText(this, wxID_ANY, wxString::FromAscii(_name.c_str()), wxDefaultPosition, wxSize(widthn, 30));
                sizer->Add(label, 0, wxCENTER);
            }

            wxString* ws = new wxString [choices.size()];
            unsigned int i;
            for (i = 0; i < choices.size(); i++)
            {
                ws[i] = wxString::FromAscii(choices[i].c_str());
            }        

            choice =  new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(200, -1), choices.size(), ws);
            sizer->Add(choice);

            Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(deChoiceImpl::choose));
        }

        void choose(wxCommandEvent &event)
        {
            int i = event.GetInt();
            parent.onChoose(i);
        }

        void set(int index)
        {
            choice->SetSelection(index);
        }
};

deChoice::deChoice(deWindow& window, const std::string& _name, const std::vector<std::string>& _choices)
{
    deWindowWX* w = dynamic_cast<deWindowWX*>(&window);
    if (w)
    {
        impl = new deChoiceImpl(*this, window, _name, _choices);
    }
    else
    {
        impl = NULL;
    }
}

deChoice::~deChoice()
{
    if (impl)
    {
        delete impl;
    }
}

void deChoice::set(int index)
{
    if (impl)
    {
        impl->set(index);
    }
}

deWindow& deChoice::getWindow()
{
    return impl->getWindow();
}

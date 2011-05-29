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

#include "sampler_list_panel.h"
#include "project.h"
#include "sampler_panel.h"
#include "sampler.h"

deSamplerListPanel::deSamplerListPanel(wxWindow* parent, deProject* _project)
:wxPanel(parent, wxID_ANY), project(_project)
{
    project->getGUI().setSamplerListPanel(this);

    sizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* staticText = new wxStaticText(this, wxID_ANY, _T("samplers"), wxDefaultPosition, wxSize(170,-1) );
    sizer->Add(staticText, 1, wxEXPAND);

    SetSizer(sizer);

    createList();
}

deSamplerListPanel::~deSamplerListPanel()
{
    destroyList();
}

void deSamplerListPanel::updateSamplers()
{
    std::list<deSamplerPanel*>::iterator i;
    for (i = panels.begin(); i != panels.end(); i++)
    {
        (*i)->setLabels();
    }
}

void deSamplerListPanel::createList()
{
    deSamplerList& samplerList = project->getSamplerList();
    deSamplers& samplers = samplerList.getSamplers();
    deSamplers::iterator j;
    int n = 0;
    for (j = samplers.begin(); j != samplers.end(); j++)
    {
        deSampler* sampler = *j;
        deSamplerPanel* samplerPanel = new deSamplerPanel(this, sampler, project->getGUI());
        panels.push_back(samplerPanel);
        sizer->Add(samplerPanel, 0, wxEXPAND);

        n++;
    }
    sizer->Layout();
}

void deSamplerListPanel::destroyList()
{
    std::list<deSamplerPanel*>::iterator i;
    while (panels.size() > 0)
    {
        i = panels.begin();
        panels.erase(i);
        sizer->Detach(*i);
        delete *i;
    }
}    

void deSamplerListPanel::updateList()
{
    deSamplerList& samplerList = project->getSamplerList();
    int selected = samplerList.getSelected();

    std::list<deSamplerPanel*>::iterator j;
    int n = 0;
    for (j = panels.begin(); j != panels.end(); j++)
    {
        deSamplerPanel* samplerPanel = *j;

        if (n == selected)
        {
            samplerPanel->SetBackgroundColour(*wxWHITE);
        }
        else
        {
            samplerPanel->SetBackgroundColour(*wxLIGHT_GREY);
        }
        n++;
    }

}

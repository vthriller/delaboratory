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

#include "samplers_panel.h"
#include "sampler_panel.h"
#include "project.h"
#include "layer_processor.h"

deSamplersPanel::deSamplersPanel(wxWindow* parent, deProject& _project)
:wxPanel(parent, wxID_ANY, wxDefaultPosition), project(_project)
{
    deSamplerManager& samplerManager = project.getSamplerManager();
    unsigned int n = samplerManager.getNumberOfSamplers();

    unsigned int i;

    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    show = new wxCheckBox(this, wxID_ANY, _T("show samplers"));
    show->SetValue(0);
    sizer->Add(show);

    for (i = 0; i < n; i++)
    {
        deSampler* sampler = samplerManager.getSampler(i);

        if (sampler)
        {
            deSamplerPanel* panel = new deSamplerPanel(this, *sampler, project);
            sizer->Add(panel);
            panels.push_back(panel);
        }            
    }        

    Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(deSamplersPanel::check));

}

deSamplersPanel::~deSamplersPanel()
{
}

void deSamplersPanel::update()
{
    std::list<deSamplerPanel*>::iterator i;
    for (i = panels.begin(); i != panels.end(); i++)
    {
        (*i)->update();
    }
}

void deSamplersPanel::check(wxCommandEvent &event)
{
    bool checked = show->IsChecked();
    project.setShowSamplers(checked);
    project.getLayerProcessor().onGUIUpdate();
}

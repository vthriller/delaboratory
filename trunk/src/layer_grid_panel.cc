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

#include "layer_grid_panel.h"

#include "project.h"
#include <sstream>
#include "layer.h"

#include <iostream>

#include "layer_factory.h"

void deLayerGridPanel::buildRows()
{
    deLayerStack& layerStack = project->getLayerStack();
    int n = layerStack.getSize();

    int i;
    for (i = n-1; i >=0; i--)
    {
        deLayer* layer = layerStack.getLayer(i);

        layerRows.push_back(deLayerRow(i));
        deLayerRow& row = layerRows.back();

        std::ostringstream oss;
        oss << i;

        row.id = new wxStaticText(this, wxID_ANY, wxString::FromAscii(oss.str().c_str()), wxDefaultPosition, wxSize(18, -1));
        gridSizer->Add(row.id, 0, wxALIGN_CENTER);
        
        row.view = new wxRadioButton(this, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize);
        gridSizer->Add(row.view, 0, wxALIGN_CENTER);

        row.name = new wxStaticText(this, wxID_ANY, wxString::FromAscii(layer->getName().c_str()), wxDefaultPosition, wxSize(100, -1));
        gridSizer->Add(row.name, 0, wxALIGN_CENTER);

        row.action = new wxButton(this, wxID_ANY, _T("action"), wxDefaultPosition, wxSize(60,25));
        if (layer->hasAction())
        {
            gridSizer->Add(row.action, 0);
        }
        else
        {
            gridSizer->Add(row.action, 0);
            row.action->Hide();
        }

        row.blend = new wxButton(this, wxID_ANY, _T("blend"), wxDefaultPosition, wxSize(60,25));
        if (layer->hasBlending())
        {
            gridSizer->Add(row.blend, 0);
        }
        else
        {
            gridSizer->Add(row.blend, 0);
            row.blend->Hide();
        }

        row.enabled = new wxCheckBox(this, wxID_ANY, _T(""));
        if (layer->isEnabled())
        {
            row.enabled->SetValue(true);
        }
        if (layer->canDisable())
        {
            gridSizer->Add(row.enabled, 0, wxALIGN_CENTER);
        }
        else
        {
            gridSizer->Add(row.enabled, 0, wxALIGN_CENTER);
            row.enabled->Hide();
        }

    }

}

void deLayerGridPanel::clearRows()
{
    
    std::vector<deLayerRow>::iterator i;
    for (i = layerRows.begin(); i != layerRows.end(); i++)
    {
        deLayerRow& row = *i;

        gridSizer->Detach(row.id);
        delete row.id;
        
        gridSizer->Detach(row.view);
        delete row.view;
        
        gridSizer->Detach(row.name);
        delete row.name;

        gridSizer->Detach(row.action);
        delete row.action;

        gridSizer->Detach(row.blend);
        delete row.blend;

        gridSizer->Detach(row.enabled);
        delete row.enabled;
    }

    gridSizer->Clear();

    layerRows.clear();
}

deLayerGridPanel::deLayerGridPanel(wxWindow* parent, deProject* _project)
:wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(250, 400)), project(_project)
{
    maxRows = 10;

    gridSizer = new wxFlexGridSizer(6);
    gridSizer->SetFlexibleDirection(wxHORIZONTAL);
    SetSizer(gridSizer);

    buildRows();

    SetMinSize(wxSize(300, 360));

    Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(deLayerGridPanel::check));
    Connect(wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(deLayerGridPanel::select));
    Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deLayerGridPanel::click));
}

deLayerGridPanel::~deLayerGridPanel()
{
}

void deLayerGridPanel::check(wxCommandEvent &event)
{
    int id = event.GetId();
    bool checked = (event.GetInt() == 1);
    std::vector<deLayerRow>::const_iterator i;
    for (i = layerRows.begin(); i != layerRows.end(); i++)
    {
        const deLayerRow& row = *i;
        if (row.enabled->GetId() == id)
        {
            deLayerStack& layerStack = project->getLayerStack();
            deLayer* layer = layerStack.getLayer(row.index);
            layer->setEnabled(checked);
            repaintImage();
        }
    }
}

void deLayerGridPanel::select(wxCommandEvent &event)
{
    int id = event.GetId();
    std::vector<deLayerRow>::const_iterator i;
    for (i = layerRows.begin(); i != layerRows.end(); i++)
    {
        const deLayerRow& row = *i;
        if (row.view->GetId() == id)
        {
            project->getViewManager().setView(row.index);
            repaintImage();
        }
    }
}

void deLayerGridPanel::click(wxCommandEvent &event)
{
    deLayerStack& layerStack = project->getLayerStack();
    int id = event.GetId();
    std::vector<deLayerRow>::const_iterator i;
    for (i = layerRows.begin(); i != layerRows.end(); i++)
    {
        const deLayerRow& row = *i;
        if (row.action->GetId() == id)
        {
            deLayer* layer = layerStack.getLayer(row.index);
            layer->createActionFrame(this);
        }
        if (row.blend->GetId() == id)
        {
            deLayer* layer = layerStack.getLayer(row.index);
            layer->createBlendFrame(this);
        }
    }

}


void deLayerGridPanel::repaintImage()
{
    project->repaintImage();
}

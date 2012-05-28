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
#include "base_layer_with_source.h"
#include "switchable_layer.h"

#include <iostream>

#include "layer_factory.h"
#include "layer_processor.h"

#include "frame.h"

#include "str.h"

#include "layer_stack.h"

#include "layer_frame_manager.h"

#include "window_wx.h"

#include "generic_layer_frame.h"

void deLayerGridPanel::buildRows()
{
    logInfo("build rows");

    deLayerStack& layerStack = project.getLayerStack();
    int n = layerStack.getSize();

    int idWidth = 24;
    int actionWidth = 200;

    int i;
    for (i = n-1; i >=0; i--)
    {
        deBaseLayer* layer = layerStack.getLayer(i);

        layerRows.push_back(deLayerRow(i));
        deLayerRow& row = layerRows.back();

        std::ostringstream oss;
        oss << i;

        row.id = new wxStaticText(this, wxID_ANY, wxString::FromAscii(oss.str().c_str()), wxDefaultPosition, wxSize(idWidth, -1));
        gridSizer->Add(row.id, 0, wxALIGN_CENTER);

        int style = 0;
        if (i == n-1)
        {
            style = wxRB_GROUP;
        }
        
        row.view = new wxRadioButton(this, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, style);
        gridSizer->Add(row.view, 0, wxALIGN_CENTER);

        std::string action = layer->getType();

        row.action = new wxButton(this, wxID_ANY, wxString::FromAscii(action.c_str()), wxDefaultPosition, wxSize(actionWidth,25));
        if (action.size() > 0)
        {
            gridSizer->Add(row.action, 0);
        }
        else
        {
            gridSizer->Add(row.action, 0);
            row.action->Hide();
        }

    }
}

void deLayerGridPanel::clearRows()
{
    logInfo("clear rows");
    
    std::vector<deLayerRow>::iterator i;
    int index = 0;
    for (i = layerRows.begin(); i != layerRows.end(); i++)
    {
        index++;
        deLayerRow& row = *i;

        gridSizer->Detach(row.id);
        delete row.id;
        
        gridSizer->Detach(row.view);
        delete row.view;
       
        gridSizer->Detach(row.action);
        delete row.action;

    }

    gridSizer->Clear();

    layerRows.clear();
}

deLayerGridPanel::deLayerGridPanel(wxWindow* parent, deProject& _project, deLayerProcessor& _processor, deChannelManager& _channelManager)
:wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(250, 400)), 
project(_project), layerProcessor(_processor), channelManager(_channelManager)
{
    project.setLayerGridPanel(this);

    gridSizer = new wxFlexGridSizer(3);
    gridSizer->SetFlexibleDirection(wxHORIZONTAL);
    SetSizer(gridSizer);

    buildRows();

    SetMinSize(wxSize(300, 360));

    Connect(wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(deLayerGridPanel::select));
    Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deLayerGridPanel::click));
}

deLayerGridPanel::~deLayerGridPanel()
{
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
            project.getViewManager().setView(row.index);
            layerProcessor.onChangeViewMode();
        }
    }
}

void deLayerGridPanel::click(wxCommandEvent &event)
{
    deLayerStack& layerStack = project.getLayerStack();
    int id = event.GetId();
    std::vector<deLayerRow>::const_iterator i;
    for (i = layerRows.begin(); i != layerRows.end(); i++)
    {
        deLayerFrameManager& frameManager = project.getLayerFrameManager();

        const deLayerRow& row = *i;
        if (row.action->GetId() == id)
        {
            int layerIndex = row.index;

            deBaseLayer* layer = layerStack.getLayer(row.index);

            if (!project.getLayerFrameManager().checkLayerFrame(row.index))
            {
                deWindowWX window(this);
                const std::string name = layer->getType();

                deFrame* frame = new deGenericLayerFrame(window, name, *layer, layerProcessor, frameManager, layerIndex);
                if (frame)
                {
                    frame->show();
                }
            }        

        }
    }

}

void deLayerGridPanel::update()
{
    clearRows();
    buildRows();
    Layout();
    layerProcessor.onGUIUpdate();
}


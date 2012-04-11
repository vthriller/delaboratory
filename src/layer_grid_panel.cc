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
#include "layer_processor.h"

#include "frame_factory.h"

#include "frame.h"

#include "blend_frame.h"
#include "action_layer.h"

#include "str.h"

#include "layer_stack.h"

#include "layer_frame_manager.h"

void deLayerGridPanel::buildRows()
{
    project.log("build rows start");

    deLayerStack& layerStack = project.getLayerStack();
    int n = layerStack.getSize();

    int i;
    for (i = n-1; i >=0; i--)
    {
        project.log("build rows i=" + str(i));
        deLayer* layer = layerStack.getLayer(i);

        layerRows.push_back(deLayerRow(i));
        deLayerRow& row = layerRows.back();

        std::ostringstream oss;
        oss << i;

        row.id = new wxStaticText(this, wxID_ANY, wxString::FromAscii(oss.str().c_str()), wxDefaultPosition, wxSize(18, -1));
        gridSizer->Add(row.id, 0, wxALIGN_CENTER);

        int style = 0;
        if (i == n-1)
        {
            style = wxRB_GROUP;
        }
        
        row.view = new wxRadioButton(this, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, style);
        gridSizer->Add(row.view, 0, wxALIGN_CENTER);

/*
        row.name = new wxStaticText(this, wxID_ANY, wxString::FromAscii(layer->getName().c_str()), wxDefaultPosition, wxSize(100, -1));
        gridSizer->Add(row.name, 0, wxALIGN_CENTER);
        */

        std::string action = layer->getActionName();

        row.action = new wxButton(this, wxID_ANY, wxString::FromAscii(action.c_str()), wxDefaultPosition, wxSize(160,25));
        if (layer->hasAction())
        {
            gridSizer->Add(row.action, 0);
        }
        else
        {
            gridSizer->Add(row.action, 0);
            row.action->Hide();
        }

        row.blend = new wxButton(this, wxID_ANY, _T("b"), wxDefaultPosition, wxSize(20,25));
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
    project.log("build rows end");

}

void deLayerGridPanel::clearRows()
{
    project.log("clear rows start");
    
    std::vector<deLayerRow>::iterator i;
    int index = 0;
    for (i = layerRows.begin(); i != layerRows.end(); i++)
    {
        project.log("clear rows index=" + str(index));
        index++;
        deLayerRow& row = *i;

        gridSizer->Detach(row.id);
        delete row.id;
        
        gridSizer->Detach(row.view);
        delete row.view;
       
       /*
        gridSizer->Detach(row.name);
        delete row.name;
        */

        gridSizer->Detach(row.action);
        delete row.action;

        gridSizer->Detach(row.blend);
        delete row.blend;

        gridSizer->Detach(row.enabled);
        delete row.enabled;
    }

    gridSizer->Clear();

    layerRows.clear();
    project.log("clear rows end");
}

deLayerGridPanel::deLayerGridPanel(wxWindow* parent, deProject& _project, deLayerProcessor& _processor)
:wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(250, 400)), project(_project), layerProcessor(_processor)
{
    maxRows = 10;

    gridSizer = new wxFlexGridSizer(5);
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
            deLayerStack& layerStack = project.getLayerStack();
            deLayer* layer = layerStack.getLayer(row.index);
            layer->setEnabled(checked);
            int index = row.index;
            layerProcessor.markUpdateAllChannels(index);
            layerProcessor.onChangeViewMode();
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
            project.getViewManager().setView(row.index);
            layerProcessor.onChangeViewMode();
        }
    }
}

void deLayerGridPanel::click(wxCommandEvent &event)
{
    project.log("click in layer grid panel");
    deLayerStack& layerStack = project.getLayerStack();
    int id = event.GetId();
    std::vector<deLayerRow>::const_iterator i;
    for (i = layerRows.begin(); i != layerRows.end(); i++)
    {
        deLayerFrameManager& frameManager = project.getLayerFrameManager();

        const deLayerRow& row = *i;
        if (row.action->GetId() == id)
        {
            project.log("new action frame requested in row " + str(row.index));
            deLayer* layer = layerStack.getLayer(row.index);

            //if (!layer->checkActionFrame())
            if (!project.getLayerFrameManager().checkActionFrame(row.index))
            {
                project.log("creating action frame");
                deFrame* actionFrame = createFrame(this, *layer, layerProcessor, frameManager);
                if (actionFrame)
                {
//                    project.getLayerFrameManager().addActionFrame(actionFrame);
//                    layer->setActionFrame(actionFrame);
                    project.log("created action frame");
                    actionFrame->Show(true);
                }
            }        

        }
        if (row.blend->GetId() == id)
        {
            project.log("new blend frame requested in row " + str(row.index));

            deLayer* layer = layerStack.getLayer(row.index);
            deActionLayer* al = dynamic_cast<deActionLayer*>(layer);
            //if (!layer->checkBlendFrame())
            if (!frameManager.checkBlendFrame(row.index))
            {
                project.log("creating blend frame");
                deBlendFrame* blendFrame = new deBlendFrame(this, *al, layerProcessor, frameManager);
                if (blendFrame)
                {
                    //layer->setBlendFrame(blendFrame);
  //                  frameManager.addBlendFrame(blendFrame);
                    project.log("created blend frame");
                    blendFrame->Show(true);
                }
            }        
        }
    }

}


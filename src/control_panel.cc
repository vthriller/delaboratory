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

#include "control_panel.h"
#include "project.h"
#include "layer.h"
#include "layer_factory.h"
#include "layer_grid_panel.h"
#include "file_dialogs.h"
#include "layer_processor.h"
#include "wx/notebook.h"
#include "layer_stack.h"
#include "layer_frame_manager.h"
#include "frame_factory.h"
#include "color_space_utils.h"
#include "operation_processor.h"

const int g_txt = 220;

deControlPanel::deControlPanel(wxWindow* parent, deProject& _project, deLayerProcessor& _processor,  deLayerGridPanel* _layerGridPanel, deOperationProcessor& _operationProcessor, deChannelManager& _channelManager)
:wxPanel(parent), project(_project), layerGridPanel(_layerGridPanel), layerProcessor(_processor), operationProcessor(_operationProcessor), channelManager(_channelManager)
{
    project.setControlPanel(this);

    mainSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(mainSizer);

    wxNotebook* notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, _T("notebook"));
    mainSizer->Add(notebook, 1, wxEXPAND);

    std::vector<std::string> actionGroups;
    getSupportedActionGroups(actionGroups);

    std::vector<std::string> actions;
    getSupportedActions(actions);

    std::vector<std::string>::iterator i;
    for (i = actionGroups.begin(); i != actionGroups.end(); i++)
    {
        std::string n = *i;

        wxPanel* actionsPanel = new wxPanel(notebook);
        notebook->AddPage(actionsPanel, wxString::FromAscii(n.c_str()));

        wxSizer* gridSizer = new wxGridSizer(3);
        actionsPanel->SetSizer(gridSizer);

        std::vector<std::string>::iterator j;
        for (j = actions.begin(); j != actions.end(); j++)
        {
            std::string an = getActionGroup(*j);
            if (an == n)
            {
                std::string actionDescription = getActionDescription(*j);
                wxButton* b = new wxButton(actionsPanel, wxID_ANY, wxString::FromAscii(actionDescription.c_str()));
                gridSizer->Add(b);
                actionButtons.push_back(b);
                actionButtonsNames[b->GetId()] = *j;
            }
        }
        
    }

    {
        wxPanel* conversionsPanel = new wxPanel(notebook);
        notebook->AddPage(conversionsPanel, _T("colorspace"));

        std::vector<deColorSpace> colorSpaces;
        getSupportedColorSpaces(colorSpaces);

        wxSizer* gridSizer = new wxGridSizer(3);
        conversionsPanel->SetSizer(gridSizer);

        std::vector<deColorSpace>::iterator i;
        for (i = colorSpaces.begin(); i != colorSpaces.end(); i++)
        {
            std::string n = getColorSpaceName(*i);
            wxButton* b = new wxButton(conversionsPanel, wxID_ANY, wxString::FromAscii(n.c_str()));
            gridSizer->Add(b);
            convertButtons.push_back(b);
            convertButtonsColorSpaces[b->GetId()] = *i;
        }
    }


    wxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(hSizer, 0, wxCENTER);

    wxSizer* sizerD = new wxStaticBoxSizer(wxVERTICAL, this,  _T("delete"));
    hSizer->Add(sizerD, 1, wxCENTER);
    {
        deleteLayer = new wxButton(this, wxID_ANY, _T("delete top layer"));
        sizerD->Add(deleteLayer, 0, wxCENTER);
    }


    {

        wxSizer* sizerE = new wxStaticBoxSizer(wxHORIZONTAL, this,  _T("export"));
        mainSizer->Add(sizerE, 0, wxCENTER);

        {
            exportSingle = new wxButton(this, wxID_ANY, _T("one image"));
            sizerE->Add(exportSingle, 0);
        }
        {
            exportAll = new wxButton(this, wxID_ANY, _T("all images"));
            sizerE->Add(exportAll, 0);
        }
        {
            externalEditor = new wxButton(this, wxID_ANY, _T("send to GIMP"));
            sizerE->Add(externalEditor, 0);
        }
    }


    setConversions();

    Layout();
    Fit();

    Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deControlPanel::click));
}

deControlPanel::~deControlPanel()
{
}

void deControlPanel::setConversions()
{
    deViewManager& viewManager = project.getViewManager();
    int view = viewManager.getView();
    deLayerStack& layerStack = project.getLayerStack();
    deBaseLayer* layer = layerStack.getLayer(view);

    if (!layer)
    {
        return;
    }

    deColorSpace currentColorSpace = layer->getColorSpace();
    std::vector<wxButton*>::iterator i;

/*
    for (i = convertButtons.begin(); i != convertButtons.end(); i++)
    {
        wxButton* b = *i;
        int id = b->GetId();
        deColorSpace colorSpace = convertButtonsColorSpaces[id];

        bool valid = checkConversion(currentColorSpace, colorSpace);

        if ((currentColorSpace != colorSpace) && (valid))
        {
            b->Enable();
        }
        else
        {
            b->Disable();
        }
    }
    */


    for (i = actionButtons.begin(); i != actionButtons.end(); i++)
    {
        wxButton* b = *i;
        std::string action = actionButtonsNames[b->GetId()];

        if (isActionSupported(action, currentColorSpace))
        {
            b->Enable();
        }
        else
        {
            b->Disable();
        }

    }
}

void deControlPanel::onChangeView()
{
    setConversions();
}

bool deControlPanel::generateFinalImage(const std::string& app, const std::string& type, const std::string& name, bool saveAll, const std::string& dir)
{
    wxProgressDialog* progressDialog = new wxProgressDialog(_T("generate final image"), _T("generate final image"), 100, GetParent(), wxPD_AUTO_HIDE | wxPD_ELAPSED_TIME);

    bool result = project.exportFinalImage(app, type, name, progressDialog, saveAll, dir);

    delete progressDialog;

    return result;
}

void deControlPanel::click(wxCommandEvent &event)
{
    int id = event.GetId();

    if (deleteLayer->GetId() == id)
    {
        logInfo("delete layer clicked");
        deLayerStack& layerStack = project.getLayerStack();
        int index = layerStack.getSize() - 1;
        project.getLayerFrameManager().onDestroyLayer(index);
        operationProcessor.removeTopLayer();
        updateLayerGrid2();
    }

    if (exportSingle->GetId() == id)
    {
        std::string f = getSaveFile(this, "export TIFF", "tiff");

        if (!f.empty())
        {
            if (f.rfind(".tiff") != f.size() - 5)
            {
                f += ".tiff";
            }

            generateFinalImage("", "tiff", f, false, "");
        }            
    }

    if (exportAll->GetId() == id)
    {
        std::string f = getDir(this, "export all images");
        generateFinalImage("", "tiff", "", true, f);
    }

    if (externalEditor->GetId() == id)
    {
        generateFinalImage("gimp", "tiff", "", false, "");
    }

    std::map<int, deColorSpace>::iterator c = convertButtonsColorSpaces.find(id);
    if (c != convertButtonsColorSpaces.end())
    {
        deColorSpace colorSpace = c->second;

        project.addConversionLayer(colorSpace);
    }

    std::map<int, std::string>::iterator a = actionButtonsNames.find(id);
    if (a != actionButtonsNames.end())
    {
        std::string action = a->second;

        project.addActionLayer(action);
    }

}

void deControlPanel::onKey(int key)
{
    if (key == 'B')
    {
        project.addActionLayer("blur");
    }
    if (key == 'C')
    {
        project.addActionLayer("curves");
    }
    if (key == 'M')
    {
        project.addActionLayer("mixer");
    }
}


void deControlPanel::updateLayerGrid2()
{
    layerGridPanel->update();
}

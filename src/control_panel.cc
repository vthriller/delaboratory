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
#include "conversion_functions.h"
#include "layer.h"
#include "layer_factory.h"
#include "layer_grid_panel.h"
#include "file_dialogs.h"
#include "layer_processor.h"
#include "wx/notebook.h"

const int g_txt = 220;

deControlPanel::deControlPanel(wxWindow* parent, deProject& _project, deLayerProcessor& _processor,  deLayerGridPanel* _layerGridPanel)
:wxPanel(parent), project(_project), layerGridPanel(_layerGridPanel), layerProcessor(_processor)
{

    project.setControlPanel(this);

    mainSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(mainSizer);

    wxNotebook* notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, _T("notebook"));
    mainSizer->Add(notebook, 1, wxEXPAND);

    {
        wxPanel* actionsPanel = new wxPanel(notebook);
        notebook->AddPage(actionsPanel, _T("actions"));

        std::vector<std::string> actions;
        getSupportedActions(actions);

        wxSizer* gridSizer = new wxGridSizer(3);
        actionsPanel->SetSizer(gridSizer);

        std::vector<std::string>::iterator i;
        for (i = actions.begin(); i != actions.end(); i++)
        {
            std::string actionDescription = getActionDescription(*i);
            wxButton* b = new wxButton(actionsPanel, wxID_ANY, wxString::FromAscii(actionDescription.c_str()));
            gridSizer->Add(b);
            actionButtons.push_back(b);
            actionButtonsNames[b->GetId()] = *i;
        }
    }

    {
        wxPanel* conversionsPanel = new wxPanel(notebook);
        notebook->AddPage(conversionsPanel, _T("conversions"));

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
            exportJPEG = new wxButton(this, wxID_ANY, _T("JPEG"));
            sizerE->Add(exportJPEG, 0);
        }
        {
            exportTIFF = new wxButton(this, wxID_ANY, _T("16-bit TIFF"));
            sizerE->Add(exportTIFF, 0);
        }
        {
            externalEditor = new wxButton(this, wxID_ANY, _T("external editor"));
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
    deLayer* layer = layerStack.getLayer(view);
    deColorSpace currentColorSpace = layer->getColorSpace();

    std::vector<wxButton*>::iterator i;
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
}

void deControlPanel::onChangeView()
{
    setConversions();
}

void deControlPanel::generateFinalImage(const std::string& app, const std::string& type, const std::string& name)
{
    wxProgressDialog* progressDialog = new wxProgressDialog(_T("generate final image"), _T("generate final image"), 100, GetParent(), wxPD_AUTO_HIDE | wxPD_ELAPSED_TIME);

    project.exportFinalImage(app, type, name, progressDialog);

    delete progressDialog;
}

void deControlPanel::click(wxCommandEvent &event)
{
    int id = event.GetId();

    if (deleteLayer->GetId() == id)
    {
        deLayerStack& layerStack = project.getLayerStack();
        int index = layerStack.getSize() - 1;
        project.getLayerFrameManager().onDestroyLayer(index);
        layerProcessor.removeTopLayer();
        updateLayerGrid();
    }

    if (exportJPEG->GetId() == id)
    {
        std::string jpegFile = getSaveFile(this, "export JPEG", "jpeg");

        generateFinalImage("", "jpeg", jpegFile);
    }

    if (exportTIFF->GetId() == id)
    {
        std::string tiffFile = getSaveFile(this, "export TIFF", "tiff");

        generateFinalImage("", "tiff", tiffFile);
    }

    if (externalEditor->GetId() == id)
    {
        generateFinalImage("gimp", "tiff", "");
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

void deControlPanel::updateLayerGrid()
{
    layerGridPanel->clearRows();
    layerGridPanel->buildRows();
    layerGridPanel->Layout();
    layerProcessor.onGUIUpdate();
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


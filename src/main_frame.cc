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

#include "main_frame.h"
#include "layer_grid_panel.h"
#include "control_panel.h"
#include "image_area_panel.h"
#include "histogram_panel.h"
#include "view_mode_panel.h"
#include "help_color_spaces_frame.h"
#include "help_color_spaces_frame2.h"
#include "help_color_spaces_frame3.h"
#include "help_color_spaces_frame4.h"
#include "help_color_spaces_frame5.h"
#include "help_color_spaces_frame6.h"
#include "memory_info_frame.h"
#include "project.h"

enum
{
    ID_Quit = 1,
    ID_NewProject,
    ID_OpenProject,
    ID_SaveProject,
    ID_HelpColorSpaces,
    ID_HelpColorSpaces2,
    ID_HelpColorSpaces3,
    ID_HelpColorSpaces4,
    ID_HelpColorSpaces5,
    ID_LABColors1,
    ID_LABColors2,
    ID_LABColors5,
    ID_MemoryInfo
};

BEGIN_EVENT_TABLE(deMainFrame, wxFrame)
EVT_MENU(ID_Quit, deMainFrame::onQuit)
EVT_MENU(ID_NewProject, deMainFrame::onNewProject)
EVT_MENU(ID_OpenProject, deMainFrame::onOpenProject)
EVT_MENU(ID_SaveProject, deMainFrame::onSaveProject)
EVT_MENU(ID_HelpColorSpaces, deMainFrame::onHelpColorSpaces)
EVT_MENU(ID_HelpColorSpaces2, deMainFrame::onHelpColorSpaces2)
EVT_MENU(ID_HelpColorSpaces3, deMainFrame::onHelpColorSpaces3)
EVT_MENU(ID_HelpColorSpaces4, deMainFrame::onHelpColorSpaces4)
EVT_MENU(ID_HelpColorSpaces5, deMainFrame::onHelpColorSpaces5)
EVT_MENU(ID_LABColors1, deMainFrame::onLABColors1)
EVT_MENU(ID_LABColors2, deMainFrame::onLABColors2)
EVT_MENU(ID_LABColors5, deMainFrame::onLABColors5)
EVT_MENU(ID_MemoryInfo, deMainFrame::onMemoryInfo)
END_EVENT_TABLE()

deMainFrame::deMainFrame(const wxSize& size, deProject* _project)
: wxFrame() , project(_project)
{
    bool created = Create((wxFrame *)NULL, wxID_ANY, _T("delaboratory-svn (c) 2011 Jacek Poplawski"), wxDefaultPosition, size);

    mainSizer = new wxBoxSizer(wxVERTICAL);

    topPanel = new wxPanel(this);
//    topPanel->SetBackgroundColour(wxColour(100,100,100));
    topPanel->SetSize(300,20);
    mainSizer->Add(topPanel, 0, wxEXPAND);

    wxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
    topPanel->SetSizer(topSizer);

    wxPanel* viewModePanel = new deViewModePanel(topPanel, *project);
    topSizer->Add(viewModePanel);

    wxSizer* middleSizer = new wxBoxSizer(wxHORIZONTAL);

    leftPanel = new deImageAreaPanel(this, project);
//    leftPanel->SetBackgroundColour(wxColour(100,100,100));
    leftPanel->SetSize(300,300);
    middleSizer->Add(leftPanel, 1, wxEXPAND);

    wxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);
    hPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(280, 200));
//    hPanel->SetBackgroundColour(wxColour(100,100,100));
    wxSizer* sizerH = new wxStaticBoxSizer(wxVERTICAL, hPanel,  _T("histogram"));
    hPanel->SetSizer(sizerH);

    deHistogramPanel* histogramPanel = new deHistogramPanel(hPanel, project);
    sizerH->Add(histogramPanel, 0, wxCENTER);

    rightSizer->Add(hPanel, 0, wxEXPAND);

    layerGridPanel = new deLayerGridPanel(this, project);
//    layerGridPanel->SetBackgroundColour(wxColour(150,150,150));
    //layerGridPanel->SetSize(300,300);
    //rightSizer->Add(layerGridPanel, 0, wxEXPAND);
    //rightSizer->Add(layerGridPanel, 0, wxEXPAND);
    rightSizer->Add(layerGridPanel, 1, wxEXPAND);

    controlPanel = new deControlPanel(this, project, layerGridPanel);
//    controlPanel->SetBackgroundColour(wxColour(110,110,110));
//    controlPanel->SetSize(300,300);
    rightSizer->Add(controlPanel, 0, wxEXPAND);
    //rightSizer->Add(controlPanel, 1, wxEXPAND);

//    rightSizer->Layout();

    middleSizer->Add(rightSizer, 0, wxEXPAND);

    mainSizer->Add(middleSizer, 1, wxEXPAND);

    SetSizer(mainSizer);
//    mainSizer->Layout();

    full = false;

    wxMenu *menuFile = new wxMenu;
    menuFile->Append( ID_NewProject, _("New project") );
    menuFile->Append( ID_OpenProject, _("Open project") );
    menuFile->Append( ID_SaveProject, _("Save project") );
    menuFile->Append( ID_Quit, _("E&xit") );

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append( ID_HelpColorSpaces, _("channels") );
    menuHelp->Append( ID_HelpColorSpaces2, _("mix of channels") );
    menuHelp->Append( ID_HelpColorSpaces3, _("lightness/value in LAB/LCH/HSL/HSV") );
    menuHelp->Append( ID_HelpColorSpaces4, _("hue in LCH/HSL/HSV") );
    menuHelp->Append( ID_HelpColorSpaces5, _("CMYK skin colors") );
    menuHelp->Append( ID_LABColors1, _("LAB colors 1") );
    menuHelp->Append( ID_LABColors2, _("LAB colors 2") );
    menuHelp->Append( ID_LABColors5, _("LAB colors 5") );
    menuHelp->Append( ID_MemoryInfo, _("memory info") );

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, _("&File") );
    menuBar->Append( menuHelp, _("&Help") );

    SetMenuBar( menuBar );

    Layout();

    leftPanel->SetFocus();
}

void deMainFrame::hidePanels()
{
    topPanel->Hide();
    hPanel->Hide();
    layerGridPanel->Hide();
    controlPanel->Hide();
    mainSizer->Layout();
    full = true;
    leftPanel->SetFocus();
}

void deMainFrame::showPanels()
{
    topPanel->Show();
    hPanel->Show();
    layerGridPanel->Show();
    controlPanel->Show();
    mainSizer->Layout();
    full = false;
    leftPanel->SetFocus();
}

void deMainFrame::onKey(int key)
{
    if (key == 'F')
    {
        if (full)
        {
           showPanels();
        }        
        else
        {
           hidePanels();
        }        
    }
}

void deMainFrame::onQuit(wxCommandEvent& WXUNUSED(event))
{
	Close(TRUE);
}

void deMainFrame::onSaveProject(wxCommandEvent& WXUNUSED(event))
{
    project->save("project.delab");
}

void deMainFrame::onOpenProject(wxCommandEvent& WXUNUSED(event))
{
    project->load("project.delab");
}

void deMainFrame::onNewProject(wxCommandEvent& WXUNUSED(event))
{
    project->newProject();
}

void deMainFrame::onHelpColorSpaces(wxCommandEvent& event)
{
    wxFrame* help = new deHelpColorSpacesFrame(this);
    help->Show();
}

void deMainFrame::onHelpColorSpaces2(wxCommandEvent& event)
{
    wxFrame* help = new deHelpColorSpacesFrame2(this);
    help->Show();
}

void deMainFrame::onHelpColorSpaces3(wxCommandEvent& event)
{
    wxFrame* help = new deHelpColorSpacesFrame3(this);
    help->Show();
}

void deMainFrame::onHelpColorSpaces4(wxCommandEvent& event)
{
    wxFrame* help = new deHelpColorSpacesFrame4(this);
    help->Show();
}

void deMainFrame::onHelpColorSpaces5(wxCommandEvent& event)
{
    wxFrame* help = new deHelpColorSpacesFrame5(this);
    help->Show();
}

void deMainFrame::onLABColors1(wxCommandEvent& event)
{
    wxFrame* help = new deHelpColorSpacesFrame6(this, 1);
    help->Show();
}

void deMainFrame::onLABColors2(wxCommandEvent& event)
{
    wxFrame* help = new deHelpColorSpacesFrame6(this, 2);
    help->Show();
}

void deMainFrame::onLABColors5(wxCommandEvent& event)
{
    wxFrame* help = new deHelpColorSpacesFrame6(this, 5);
    help->Show();
}

void deMainFrame::onMemoryInfo(wxCommandEvent& event)
{
    wxFrame* help = new deMemoryInfoFrame(this, *project);
    help->Show();
}

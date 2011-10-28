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
#include "zoom_panel.h"
#include "histogram_mode_panel.h"
#include "help_color_spaces_frame.h"
#include "help_color_spaces_frame2.h"
#include "help_color_spaces_frame3.h"
#include "help_color_spaces_frame4.h"
#include "help_color_spaces_frame5.h"
#include "help_color_spaces_frame6.h"
#include "benchmark_frame.h"
#include "project.h"
#include "str.h"
#include "file_dialogs.h"
#include "delaboratory.h"

enum
{
    ID_Quit = 1,
    ID_NewProject,
    ID_TestImageSmall,
    ID_TestImageBig,
    ID_OpenImage,
    ID_OpenProject,
    ID_SaveProject,
    ID_OpenLayerStack,
    ID_SaveLayerStack,
    ID_HelpColorSpaces,
    ID_HelpColorSpaces2,
    ID_HelpColorSpaces3,
    ID_HelpColorSpaces4,
    ID_HelpColorSpaces5,
    ID_LABColors1,
    ID_LABColors2,
    ID_LABColors5,
    ID_MemoryInfo,
    ID_BenchmarkBlur,
    ID_BenchmarkColor
};

BEGIN_EVENT_TABLE(deMainFrame, wxFrame)
EVT_MENU(ID_Quit, deMainFrame::onQuit)
EVT_MENU(ID_NewProject, deMainFrame::onNewProject)
EVT_MENU(ID_TestImageSmall, deMainFrame::onTestImageSmall)
EVT_MENU(ID_TestImageBig, deMainFrame::onTestImageBig)
EVT_MENU(ID_OpenImage, deMainFrame::onOpenImage)
EVT_MENU(ID_OpenProject, deMainFrame::onOpenProject)
EVT_MENU(ID_SaveProject, deMainFrame::onSaveProject)
EVT_MENU(ID_OpenLayerStack, deMainFrame::onOpenLayerStack)
EVT_MENU(ID_SaveLayerStack, deMainFrame::onSaveLayerStack)
EVT_MENU(ID_HelpColorSpaces, deMainFrame::onHelpColorSpaces)
EVT_MENU(ID_HelpColorSpaces2, deMainFrame::onHelpColorSpaces2)
EVT_MENU(ID_HelpColorSpaces3, deMainFrame::onHelpColorSpaces3)
EVT_MENU(ID_HelpColorSpaces4, deMainFrame::onHelpColorSpaces4)
EVT_MENU(ID_HelpColorSpaces5, deMainFrame::onHelpColorSpaces5)
EVT_MENU(ID_LABColors1, deMainFrame::onLABColors1)
EVT_MENU(ID_LABColors2, deMainFrame::onLABColors2)
EVT_MENU(ID_LABColors5, deMainFrame::onLABColors5)
EVT_MENU(ID_MemoryInfo, deMainFrame::onMemoryInfo)
EVT_MENU(ID_BenchmarkBlur, deMainFrame::onBenchmarkBlur)
EVT_MENU(ID_BenchmarkColor, deMainFrame::onBenchmarkColor)
END_EVENT_TABLE()

deMainFrame::deMainFrame(const wxSize& size, deProject* _project)
: wxFrame() , project(_project)
{
    std::string s = getApplicationName() + " " + getVersion() + " " + getCopyright();

    Create((wxFrame *)NULL, wxID_ANY, wxString::FromAscii(s.c_str()), wxDefaultPosition, size);

    mainSizer = new wxBoxSizer(wxHORIZONTAL);

    wxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(leftSizer, 1, wxEXPAND);

    topPanel = new wxPanel(this);
    topPanel->SetSize(300,20);
    leftSizer->Add(topPanel, 0, wxEXPAND);

    wxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
    topPanel->SetSizer(topSizer);

    wxPanel* viewModePanel = new deViewModePanel(topPanel, *project);
    topSizer->Add(viewModePanel);

    wxPanel* zoomPanel = new deZoomPanel(topPanel, *project);
    topSizer->Add(zoomPanel);

    leftPanel = new deImageAreaPanel(this, project);
    leftPanel->SetSize(300,300);
    leftSizer->Add(leftPanel, 1, wxEXPAND);

    wxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);
    hPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(280, 200));
    wxSizer* sizerH = new wxStaticBoxSizer(wxVERTICAL, hPanel,  _T("histogram"));
    hPanel->SetSizer(sizerH);

    deHistogramPanel* histogramPanel = new deHistogramPanel(hPanel, project);
    sizerH->Add(histogramPanel, 0, wxCENTER);

    deHistogramModePanel* histogramModePanel = new deHistogramModePanel(hPanel, *project);
    sizerH->Add(histogramModePanel, 0, wxLEFT);

    rightSizer->Add(hPanel, 0, wxEXPAND);

    layerGridPanel = new deLayerGridPanel(this, project);
    rightSizer->Add(layerGridPanel, 1, wxEXPAND);

    controlPanel = new deControlPanel(this, project, layerGridPanel);
    rightSizer->Add(controlPanel, 0, wxEXPAND);

    mainSizer->Add(rightSizer, 0, wxEXPAND);

    SetSizer(mainSizer);

    full = false;

    wxMenu *menuFile = new wxMenu;
    menuFile->Append( ID_NewProject, _("New project") );
    menuFile->AppendSeparator();
    menuFile->Append( ID_OpenImage, _("Open image") );
    menuFile->Append( ID_TestImageSmall, _("Generate test image (small)") );
    menuFile->Append( ID_TestImageBig, _("Generate test image (big, slow)") );
    menuFile->AppendSeparator();
    menuFile->Append( ID_OpenLayerStack, _("Open layer stack") );
    menuFile->Append( ID_SaveLayerStack, _("Save layer stack") );
    menuFile->AppendSeparator();
    menuFile->Append( ID_OpenProject, _("Open project ( stack + image )") );
    menuFile->Append( ID_SaveProject, _("Save project ( stack + image )") );
    menuFile->AppendSeparator();
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

    wxMenu *menuInfo = new wxMenu;
    menuInfo->Append( ID_MemoryInfo, _("memory info") );
    menuInfo->AppendSeparator();
    menuInfo->Append( ID_BenchmarkColor, _("benchmark color conversion") );
    menuInfo->Append( ID_BenchmarkBlur, _("benchmark blur (slow!)") );

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, _("&File") );
    menuBar->Append( menuInfo, _("&Info") );
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
    std::string fileName = getSaveFile(this, "save project", "delab");
    project->save(fileName, true);
}

void deMainFrame::onSaveLayerStack(wxCommandEvent& WXUNUSED(event))
{
    std::string fileName = getSaveFile(this, "save layer stack", "delab");
    project->save(fileName, false);
}

void deMainFrame::onOpenProject(wxCommandEvent& WXUNUSED(event))
{
    std::string fileName = getOpenFile(this, "open project", "delab");
    project->open(fileName, true);
}

void deMainFrame::onOpenLayerStack(wxCommandEvent& WXUNUSED(event))
{
    std::string fileName = getOpenFile(this, "open layer stack", "delab");
    project->open(fileName, false);
}

void deMainFrame::onNewProject(wxCommandEvent& WXUNUSED(event))
{
    project->newProject();
}

void deMainFrame::onTestImageSmall(wxCommandEvent& WXUNUSED(event))
{
    project->setTestImage(900);
}

void deMainFrame::onTestImageBig(wxCommandEvent& WXUNUSED(event))
{
    project->setTestImage(1800);
}

void deMainFrame::onOpenImage(wxCommandEvent& WXUNUSED(event))
{
    std::string fileName = getOpenFile(this, "load source image", "image");
    project->openImage(fileName);
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
    project->openMemoryInfoFrame(this);
}

void deMainFrame::onBenchmarkBlur(wxCommandEvent& event)
{
    deBenchmarkFrame* help = new deBenchmarkFrame(this, "blur");
    help->Show();
    help->perform();
}

void deMainFrame::onBenchmarkColor(wxCommandEvent& event)
{
    deBenchmarkFrame* help = new deBenchmarkFrame(this, "color");
    help->Show();
    help->perform();
}

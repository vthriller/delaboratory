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
#include "samplers_panel.h"
#include "layer_processor.h"

#include "image_panel.h"

#include "wx/thread.h"
#include "wx/notebook.h"


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
EVT_MENU(DE_REPAINT_EVENT, deMainFrame::onRepaintEvent)
EVT_MENU(DE_RANDOM_EVENT, deMainFrame::onRandomEvent)
END_EVENT_TABLE()

deMainFrame::deMainFrame(const wxSize& size, deProject& _project, deLayerProcessor& _layerProcessor)
: wxFrame() , project(_project), layerProcessor(_layerProcessor)
{
    project.setMainFrame(this);

    project.log("opening main frame");

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

    wxPanel* viewModePanel = new deViewModePanel(topPanel, project);
    topSizer->Add(viewModePanel);

    multithreading = new wxCheckBox(topPanel, wxID_ANY, _T("multithreading"));
    multithreading->SetValue(1);
    topSizer->Add(multithreading);

    wxButton* testButton = new wxButton(topPanel, wxID_ANY, _T("random layers - for crash test"));
    topSizer->Add(testButton);

    imageAreaPanel = new deImageAreaPanel(this, &project);
    imageAreaPanel->SetSize(300,300);
    leftSizer->Add(imageAreaPanel, 1, wxEXPAND);

    wxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);
    hPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(280, 250));
    wxSizer* sizerH = new wxStaticBoxSizer(wxVERTICAL, hPanel,  _T("histogram"));
    hPanel->SetSizer(sizerH);

    histogramPanel = new deHistogramPanel(hPanel, &project);
    sizerH->Add(histogramPanel, 0, wxCENTER);

    deHistogramModePanel* histogramModePanel = new deHistogramModePanel(hPanel, project);
    sizerH->Add(histogramModePanel, 0, wxLEFT);

    rightSizer->Add(hPanel, 0, wxEXPAND);

    wxNotebook* notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, _T("notebook"));
    rightSizer->Add(notebook, 1, wxEXPAND);

    layerGridPanel = new deLayerGridPanel(notebook, project, _layerProcessor);

    notebook->AddPage(layerGridPanel, _T("layers"));
    samplersPanel = new deSamplersPanel(notebook, project);
    notebook->AddPage(samplersPanel, _T("samplers"));

    controlPanel = new deControlPanel(this, project, _layerProcessor, layerGridPanel);
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

    imageAreaPanel->SetFocus();
    Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deMainFrame::test));
    Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(deMainFrame::check));
}

void deMainFrame::hidePanels()
{
    topPanel->Hide();
    hPanel->Hide();
    layerGridPanel->Hide();
    controlPanel->Hide();
    mainSizer->Layout();
    full = true;
    imageAreaPanel->SetFocus();
}

deMainFrame::~deMainFrame()
{
    project.log("closing main frame");
    layerProcessor.stopWorkerThread();
    layerProcessor.lock();
    layerProcessor.unlock();
}

void deMainFrame::showPanels()
{
    topPanel->Show();
    hPanel->Show();
    layerGridPanel->Show();
    controlPanel->Show();
    mainSizer->Layout();
    full = false;
    imageAreaPanel->SetFocus();
}

void deMainFrame::rebuild()
{
    topPanel->Layout();
    topPanel->Update();
    topPanel->Refresh();
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
    std::cout << "onQuit" << std::endl;
	Close(TRUE);
}

void deMainFrame::onSaveProject(wxCommandEvent& WXUNUSED(event))
{
    std::string fileName = getSaveFile(this, "save project", "delab");
    project.save(fileName, true);
}

void deMainFrame::onSaveLayerStack(wxCommandEvent& WXUNUSED(event))
{
    std::string fileName = getSaveFile(this, "save layer stack", "delab");
    project.save(fileName, false);
}

void deMainFrame::onOpenProject(wxCommandEvent& WXUNUSED(event))
{
    std::string fileName = getOpenFile(this, "open project", "delab");
    project.open(fileName, true);
}

void deMainFrame::onOpenLayerStack(wxCommandEvent& WXUNUSED(event))
{
    std::string fileName = getOpenFile(this, "open layer stack", "delab");
    project.open(fileName, false);
}

void deMainFrame::onNewProject(wxCommandEvent& WXUNUSED(event))
{
    project.newProject();
}

void deMainFrame::onTestImageSmall(wxCommandEvent& WXUNUSED(event))
{
    project.setTestImage(900);
}

void deMainFrame::onTestImageBig(wxCommandEvent& WXUNUSED(event))
{
    project.setTestImage(1800);
}

void deMainFrame::onOpenImage(wxCommandEvent& WXUNUSED(event))
{
    std::string fileName = getOpenFile(this, "load source image", "image");
    project.openImage(fileName);
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
    project.openMemoryInfoFrame(this);
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

void deMainFrame::onRepaintEvent(wxCommandEvent& event)
{
    repaintMainFrame(true);
}

void deMainFrame::onRandomEvent(wxCommandEvent& event)
{
    project.addRandomLayer();
}

class deTestThread:public wxThread
{
    private:
        virtual void *Entry()
        {
            int i;
            int max = 100;
            for (i = 0; i < max; i++)
            {
                wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, DE_RANDOM_EVENT );
                wxPostEvent( frame, event );
                wxThread::Sleep(100 + rand() % 1000);
                if (TestDestroy())
                {
                    i = max;
                }
            }
            return NULL;
        }
        deMainFrame* frame;
        deProject& project;
    public:    
        deTestThread(deMainFrame* _frame, deProject& _project)
        :frame(_frame), project(_project)
        {
        }
        virtual ~deTestThread()
        {
        }
};

void deMainFrame::test(wxCommandEvent& event)
{
    deChannelManager& channelManager = project.getPreviewChannelManager();
    deChannel* channel = channelManager.getChannel(0);

    deTestThread* thread = new deTestThread(this, project);

    if ( thread->Create() != wxTHREAD_NO_ERROR )
    {
    }

    if ( thread->Run() != wxTHREAD_NO_ERROR )
    {
    }
}

void deMainFrame::repaintMainFrame(bool calcHistogram)
{
//    project.log("requested repaint main frame");
    if (!project.isSourceValid())
    {
        std::cout << "source not valid" << std::endl;
        return;
    }

    project.getLayerProcessor().lock();

    imageAreaPanel->getImagePanel()->repaintImagePanel();
    controlPanel->updateSamplerManagerFrame();
    samplersPanel->update();
    project.updateMemoryInfo();
    if (calcHistogram)
    {
        if (histogramPanel)
        {
            histogramPanel->generate();
            histogramPanel->paint();
        }
    }

    project.getLayerProcessor().unlock();

}

void deMainFrame::check(wxCommandEvent &event)
{
    bool checked = multithreading->IsChecked();
    project.getLayerProcessor().setMultithreadingEnabled(checked);
}

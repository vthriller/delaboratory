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
#include "color_matrix_frame.h"
#include "palette_frame.h"
#include "project.h"
#include "str.h"
#include "str_wx.h"
#include "file_dialogs.h"
#include "delaboratory.h"
#include "samplers_panel.h"
#include "layer_processor.h"
#include "zoom_panel.h"
#include "threads_panel.h"
#include "warning_panel.h"
#include "layer_stack.h"

#include "image_panel.h"

#include "wx/thread.h"
#include "wx/notebook.h"
#include "progress_dialog.h"

BEGIN_EVENT_TABLE(deMainFrame, wxFrame)
EVT_MENU(ID_Quit, deMainFrame::onQuit)
EVT_MENU(ID_NewProject, deMainFrame::onNewProject)
EVT_MENU(ID_TestImageSmall, deMainFrame::onTestImageSmall)
EVT_MENU(ID_TestImageBig, deMainFrame::onTestImageBig)
EVT_MENU(ID_OpenImage, deMainFrame::onOpenImage)
EVT_MENU(ID_OpenRawImageProPhoto, deMainFrame::onOpenRawImageProPhoto)
EVT_MENU(ID_HelpColorSpaces, deMainFrame::onHelpColorSpaces)
EVT_MENU(ID_HelpColorSpaces2, deMainFrame::onHelpColorSpaces2)
EVT_MENU(ID_HelpColorSpaces3, deMainFrame::onHelpColorSpaces3)
EVT_MENU(ID_HelpColorSpaces4, deMainFrame::onHelpColorSpaces4)
EVT_MENU(ID_HelpColorSpaces5, deMainFrame::onHelpColorSpaces5)
EVT_MENU(ID_LABColors1, deMainFrame::onLABColors1)
EVT_MENU(ID_LABColors2, deMainFrame::onLABColors2)
EVT_MENU(ID_LABColors5, deMainFrame::onLABColors5)
EVT_MENU(ID_ColorMatrix1, deMainFrame::onColorMatrix1)
EVT_MENU(ID_ColorMatrix2, deMainFrame::onColorMatrix2)
EVT_MENU(ID_ColorMatrix3, deMainFrame::onColorMatrix3)
EVT_MENU(ID_ColorMatrix4, deMainFrame::onColorMatrix4)
EVT_MENU(ID_ColorMatrix5, deMainFrame::onColorMatrix5)
EVT_MENU(ID_PaletteFrame, deMainFrame::onPaletteFrame)
EVT_MENU(DE_REPAINT_EVENT, deMainFrame::onRepaintEvent)
EVT_MENU(DE_IMAGE_LOAD_EVENT, deMainFrame::onImageLoadEvent)
EVT_MENU(DE_HISTOGRAM_EVENT, deMainFrame::onHistogramEvent)
EVT_MENU(DE_INFO_EVENT, deMainFrame::onInfoEvent)
EVT_MENU(DE_WARNING_EVENT, deMainFrame::onWarningEvent)
EVT_MENU(ID_ExportGIMP, deMainFrame::onExportGIMP)
EVT_MENU(ID_ExportTIFF, deMainFrame::onExportTIFF)
EVT_MENU(ID_ExportJPG, deMainFrame::onExportJPG)
EVT_MENU(ID_ExportAll, deMainFrame::onExportAll)
END_EVENT_TABLE()

deMainFrame::deMainFrame(const wxSize& size, deProject& _project, deLayerProcessor& _layerProcessor, deSamplerManager& _samplerManager, deZoomManager& _zoomManager, const std::string& dcraw_version, deOperationProcessor& _operationProcessor, deChannelManager& channelManager, deGUI& _gui)
: wxFrame() , project(_project), layerProcessor(_layerProcessor), imageSize(0,0), gui(_gui)
{
    logInfo("main frame constructor");

    imageName = "";

#ifdef __WXOSX_MAC__
    Create((wxFrame *)NULL, wxID_ANY, _T("main frame"), wxDefaultPosition, wxSize(1440, 800), wxDEFAULT_FRAME_STYLE);
#else
    Create((wxFrame *)NULL, wxID_ANY, _T("main frame"), wxDefaultPosition, size, wxDEFAULT_FRAME_STYLE | wxMAXIMIZE);
#endif    

    updateTitle();

    mainSizer = new wxBoxSizer(wxHORIZONTAL);

    wxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(leftSizer, 1, wxEXPAND);

    topPanel = new wxPanel(this);
    leftSizer->Add(topPanel, 0, wxEXPAND);

    wxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
    topPanel->SetSizer(topSizer);

    wxPanel* viewModePanel = new deViewModePanel(topPanel, project, gui);
    topSizer->Add(viewModePanel);

    threadsPanel = new deThreadsPanel(topPanel);
    topSizer->Add(threadsPanel, 0, wxEXPAND);

    deZoomPanel* zoomPanel = new deZoomPanel(topPanel, _zoomManager);
    topSizer->Add(zoomPanel);

    warningPanel = new deWarningPanel(topPanel);
    topSizer->Add(warningPanel, 0, wxEXPAND);

    wxSizer* sizerI = new wxStaticBoxSizer(wxVERTICAL, this,  _T("image"));
    leftSizer->Add(sizerI, 1, wxEXPAND);

    imageAreaPanel = new deImageAreaPanel(this, project, _samplerManager, _zoomManager, zoomPanel, gui);
    sizerI->Add(imageAreaPanel, 1, wxEXPAND);

    wxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);
    hPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(280, 250));
    wxSizer* sizerH = new wxStaticBoxSizer(wxVERTICAL, hPanel,  _T("histogram"));
    hPanel->SetSizer(sizerH);

    histogramPanel = new deHistogramPanel(hPanel, &project, 260, 2);
    sizerH->Add(histogramPanel, 0, wxCENTER);

    deHistogramModePanel* histogramModePanel = new deHistogramModePanel(hPanel, project, histogramPanel, gui);
    sizerH->Add(histogramModePanel, 0, wxLEFT);

    rightSizer->Add(hPanel, 0, wxEXPAND);

    wxNotebook* notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, _T("notebook"));
    rightSizer->Add(notebook, 1, wxEXPAND);

    layerGridPanel = new deLayerGridPanel(notebook, project, _layerProcessor, channelManager, gui);

    notebook->AddPage(layerGridPanel, _T("layers"));
    samplersPanel = new deSamplersPanel(notebook, project, _samplerManager);
    notebook->AddPage(samplersPanel, _T("samplers"));

    controlPanel = new deControlPanel(this, project, _layerProcessor, _operationProcessor, channelManager);
    rightSizer->Add(controlPanel, 0, wxEXPAND);

    mainSizer->Add(rightSizer, 0, wxEXPAND);

    SetSizer(mainSizer);

    full = false;

    wxMenu *menuFile = new wxMenu;
    menuFile->Append( ID_NewProject, _("New project") );
    menuFile->AppendSeparator();
    menuFile->Append( ID_OpenRawImageProPhoto, _("Open RAW image as ProPhoto RGB") );
    menuFile->Append( ID_OpenImage, _("Open TIFF/JPG image") );
    menuFile->AppendSeparator();
    menuFile->Append( ID_TestImageSmall, _("Generate test image (small)") );
    menuFile->Append( ID_TestImageBig, _("Generate test image (big, slow)") );
    menuFile->AppendSeparator();
    menuFile->Append( ID_Quit, _("E&xit") );

    wxMenu *menuExport = new wxMenu;
    menuExport->Append( ID_ExportGIMP, _("Send to GIMP") );
    menuExport->Append( ID_ExportTIFF, _("Export 16-bit TIFF") );
    menuExport->Append( ID_ExportAll, _("Export all layers to 16-bit TIFFs") );
    menuExport->Append( ID_ExportJPG, _("Export JPG") );

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append( ID_HelpColorSpaces, _("channels") );
    menuHelp->Append( ID_HelpColorSpaces2, _("mix of channels") );
    menuHelp->Append( ID_HelpColorSpaces3, _("lightness/value in LAB/LCH/HSL/HSV") );
    menuHelp->Append( ID_HelpColorSpaces4, _("hue in LCH/HSL/HSV") );
    menuHelp->Append( ID_HelpColorSpaces5, _("CMYK skin colors") );
    menuHelp->Append( ID_LABColors1, _("LAB colors 1") );
    menuHelp->Append( ID_LABColors2, _("LAB colors 2") );
    menuHelp->Append( ID_LABColors5, _("LAB colors 5") );

    wxMenu *menuPalette = new wxMenu;
    menuPalette->Append( ID_ColorMatrix1, _("LAB color matrix 40x40") );
    menuPalette->Append( ID_ColorMatrix2, _("LAB color matrix 20x20") );
    menuPalette->Append( ID_ColorMatrix3, _("color tiles 20") );
    menuPalette->Append( ID_ColorMatrix4, _("color tiles 40") );
    menuPalette->Append( ID_ColorMatrix5, _("color tiles 80") );
    menuPalette->Append( ID_PaletteFrame, _("palette frame") );

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, _("&File") );
    menuBar->Append( menuExport, _("&Export") );
    menuBar->Append( menuPalette, _("&Palette") );
    menuBar->Append( menuHelp, _("&Channels") );

    SetMenuBar( menuBar );

    Layout();

    imageAreaPanel->SetFocus();
    Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(deMainFrame::check));

    Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(deMainFrame::onCloseEvent));

    Connect(wxEVT_TIMER, wxTimerEventHandler(deMainFrame::onTimerEvent));

    threadsPanel->setInfoColor(DE_DCRAW_END);
}

#define TILE1 20
#define TILE2 40
#define TILE3 60
#define TILER1 30
#define TILER2 50
#define TILER3 80
#define PALRW 60
#define PALRH 40
#define PAL1 16
#define PAL2 12
#define PAL3 10
#define MAR1 0.03
#define MAR2 0.06


deMainFrame::~deMainFrame()
{
    logInfo("main frame destructor");
    layerProcessor.stopWorkerThread();
    layerProcessor.lockLayerProcessor();
    layerProcessor.unlockLayerProcessor();
}

void deMainFrame::rebuild()
{
    topPanel->Layout();
    topPanel->Update();
    topPanel->Refresh();
}

void deMainFrame::onKey(int key)
{
}

void deMainFrame::onQuit(wxCommandEvent& WXUNUSED(event))
{
    logInfo("main frame onQuit");
	Close(TRUE);
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
    std::string f = getOpenFile(this, "load source image", "image", openDirectory);
    if (!f.empty())
    {
        openDirectory = getPath(f);
        project.openImage(f, false, deColorSpaceRGB);
    }        
}

void deMainFrame::onOpenRawImageProPhoto(wxCommandEvent& WXUNUSED(event))
{
    std::string f = getOpenFile(this, "load source image", "raw", openDirectory);
    if (!f.empty())
    {
        openDirectory = getPath(f);
        project.openImage(f, true, deColorSpaceProPhoto);
    }        
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

void deMainFrame::onColorMatrix1(wxCommandEvent& event)
{
    dePalette3* palette = NULL;
    wxFrame* help = new deColorMatrixFrame2(this, project, 1, 2, 0, 32, 32, palette);
    help->Show();
}

void deMainFrame::onColorMatrix2(wxCommandEvent& event)
{
    dePalette3* palette = NULL;
    wxFrame* help = new deColorMatrixFrame2(this, project, 1, 2, 0, 16, 16, palette);
    help->Show();
}

void deMainFrame::onColorMatrix3(wxCommandEvent& event)
{
    wxFrame* help = new deColorMatrixFrame(this, project, 20, 20, 20, 20, 40, 40, 12, 0.1);
    help->Show();
}

void deMainFrame::onColorMatrix4(wxCommandEvent& event)
{
    wxFrame* help = new deColorMatrixFrame(this, project, 40, 40, 40, 40, 40, 40, 12, 0.1);
    help->Show();
}

void deMainFrame::onColorMatrix5(wxCommandEvent& event)
{
    wxFrame* help = new deColorMatrixFrame(this, project, 80, 80, 80, 80, 40, 40, 12, 0.1);
    help->Show();
}

void deMainFrame::onPaletteFrame(wxCommandEvent& event)
{
    wxFrame* help = new dePaletteFrame(this);
    help->Show();
}


void deMainFrame::onRepaintEvent(wxCommandEvent& event)
{
    repaintMainFrame(true);
}

void deMainFrame::onImageLoadEvent(wxCommandEvent& event)
{
    layerProcessor.onImageLoad();
}

void deMainFrame::onInfoEvent(wxCommandEvent& event)
{
    int i = event.GetInt();
    threadsPanel->setInfoColor(i);
}

void deMainFrame::onWarningEvent(wxCommandEvent& event)
{
    int view = project.getViewManager().getView();    
    deLayerStack& stack = project.getLayerStack();
    const deBaseLayer* layer = stack.startReadLayer(view);
    if (layer)
    {
        std::string warning = layer->getWarning();
        warningPanel->setWarning(warning);
    }
    stack.finishReadLayer(view);
}

void deMainFrame::repaintMainFrame(bool calcHistogram)
{
#ifdef DEBUG_LOG
    logInfo("repaint main frame");
#endif    
    if (!project.isSourceValid())
    {
        return;
    }

    imageAreaPanel->getImagePanel()->repaintImagePanel();
    samplersPanel->update();

}

void deMainFrame::check(wxCommandEvent &event)
{
}

void deMainFrame::onCloseEvent(wxCloseEvent& event)
{
    logInfo("deMainFrame::onCloseEvent");
    this->Destroy();
}

void deMainFrame::generateHistogram()
{
    if (histogramPanel)
    {
        histogramPanel->generateHistogram();
    }
}

void deMainFrame::onHistogramEvent(wxCommandEvent& event)
{
    if (histogramPanel)
    {
        histogramPanel->paintHistogram();
    }
}

void deMainFrame::updateTitle()
{
    std::string s = imageName + " " + str(imageSize.getW()) + "x" + str(imageSize.getH()) + " - " + getApplicationName() + " " + getVersion() + " " + getCopyright();

    SetTitle(str2wx(s));
}

void deMainFrame::setImageName(const std::string& _imageName, const deSize& _size)
{
    imageName = _imageName;
    imageSize = _size;
    updateTitle();
}    
    
void deMainFrame::onTimerEvent(wxTimerEvent& event)
{
    project.onTimerUpdate();
}

void deMainFrame::updateWarning()
{
}

void deMainFrame::forceUpdateSize()
{
    imageAreaPanel->updateSize(false);
}

void deMainFrame::onExportGIMP(wxCommandEvent& event)
{
    generateFinalImage("gimp", "tiff", "", false, "");
}

void deMainFrame::onExportAll(wxCommandEvent& event)
{
    std::string f = getDir(this, "export all images");
    generateFinalImage("", "tiff", "", true, f);
}    

void deMainFrame::onExportTIFF(wxCommandEvent& event)
{
    std::string f = getSaveFile(this, "export TIFF", "tiff", saveDirectory);

    if (!f.empty())
    {
        saveDirectory = getPath(f);

        if (f.rfind(".tiff") != f.size() - 5)
        {
            f += ".tiff";
        }

        generateFinalImage("", "tiff", f, false, "");
    }            
}    

void deMainFrame::onExportJPG(wxCommandEvent& event)
{
    std::string f = getSaveFile(this, "export JPG", "jpg", saveDirectory);

    if (!f.empty())
    {
        saveDirectory = getPath(f);

        if (f.rfind(".jpg") != f.size() - 4)
        {
            f += ".jpg";
        }

        generateFinalImage("", "jpg", f, false, "");
    }            
}    


bool deMainFrame::generateFinalImage(const std::string& app, const std::string& type, const std::string& name, bool saveAll, const std::string& dir)
{
    logInfo("generateFinalImage...");

    deProgressDialog dialog;

    bool result = project.exportFinalImage(app, type, name, dialog, saveAll, dir);

    return result;
}

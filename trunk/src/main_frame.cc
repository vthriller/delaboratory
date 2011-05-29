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
#include "right_panel.h"
#include "left_panel.h"
#include "top_panel.h"
#include "bottom_panel.h"
#include "center_panel.h"
#include "exception.h"
#include "project.h"
#include <iostream>

enum
{
    ID_Quit = 1,
    ID_Open,
    ID_About
};

BEGIN_EVENT_TABLE(deMainFrame, wxFrame)
EVT_MENU(ID_Quit, deMainFrame::OnQuit)
EVT_MENU(ID_Open, deMainFrame::OnOpen)
EVT_MENU(ID_About, deMainFrame::OnAbout)
END_EVENT_TABLE()

deMainFrame::deMainFrame(const wxSize& size, deProject* _project)
: wxFrame((wxFrame *)NULL, wxID_ANY, _T("delaboratory (c) 2011 Jacek Poplawski"), wxDefaultPosition, size) , project(_project)
{
        wxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

        wxPanel* topPanel = new deTopPanel(this, project->getGUI());
        mainSizer->Add(topPanel, 0, wxEXPAND);

        wxSizer* middleSizer = new wxBoxSizer(wxHORIZONTAL);

        wxPanel* leftPanel = new deLeftPanel(this, project);
        middleSizer->Add(leftPanel, 0, wxEXPAND);

        wxPanel* centerPanel = new deCenterPanel(this, project);
        middleSizer->Add(centerPanel, 1, wxEXPAND);

        wxPanel* rightPanel = new deRightPanel(this, project);
        middleSizer->Add(rightPanel, 0, wxEXPAND);

        mainSizer->Add(middleSizer, 1, wxEXPAND);

        wxPanel* bottomPanel = new deBottomPanel(this, project);
        mainSizer->Add(bottomPanel, 0, wxEXPAND);

        SetSizer(mainSizer);

        mainSizer->Layout();

        wxMenu *menuFile = new wxMenu;

        menuFile->Append( ID_Open, _("&Open source image") );
        menuFile->Append( ID_About, _("&About") );
        menuFile->AppendSeparator();
        menuFile->Append( ID_Quit, _("E&xit") );

        wxMenuBar *menuBar = new wxMenuBar;
        menuBar->Append( menuFile, _("&File") );

        SetMenuBar( menuBar );

}

void deMainFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	Close(TRUE);
}

void deMainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
}

void deMainFrame::OnOpen(wxCommandEvent& WXUNUSED(event))
{

    wxString type = _T("JPEG/TIFF files (*.jpg;*.jpeg;*.tiff;*.tif)|*.jpg;*.jpeg;*.tiff;*.tif");
    wxFileDialog openFileDialog(this, _T("Open source image"), _T(""), _T(""), type, wxFD_OPEN|wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
    {
        return;
    }

    wxString fileName = openFileDialog.GetPath();
    char cstring[1024];
    strncpy(cstring, (const char*)fileName.mb_str(wxConvUTF8), 1023);

    project->loadSourceImage(cstring);

}


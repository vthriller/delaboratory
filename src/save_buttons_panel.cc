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

#include "save_buttons_panel.h"
#include "project.h"
#include "final_image.h"

void deSaveButtonsPanel::saveJPEG(wxCommandEvent &event)
{
    wxString type = _T("JPEG files (*.jpg;*.jpeg) | *.jpg;*.jpeg");
    wxFileDialog saveFileDialog(this, _T("Save final image as JPEG"), _T(""), _T(""), type, wxFD_SAVE);

    if (saveFileDialog.ShowModal() == wxID_CANCEL)
    {
        return;
    }

    wxString fileName = saveFileDialog.GetPath();
    char cstring[1024];
    strncpy(cstring, (const char*)fileName.mb_str(wxConvUTF8), 1023);

    deFinalImage* finalImage = project->generateFinalImage();
    if (finalImage)
    {
        finalImage->saveJPEG(cstring);
        delete finalImage;
    }
}

void deSaveButtonsPanel::saveTIFF(wxCommandEvent &event)
{
    wxString type = _T("TIFF files (*.tiff;*.tif) | *.tiff;*.tif");
    wxFileDialog saveFileDialog(this, _T("Save final image as TIFF"), _T(""), _T(""), type, wxFD_SAVE);

    if (saveFileDialog.ShowModal() == wxID_CANCEL)
    {
        return;
    }

    wxString fileName = saveFileDialog.GetPath();
    char cstring[1024];
    strncpy(cstring, (const char*)fileName.mb_str(wxConvUTF8), 1023);

    deFinalImage* finalImage = project->generateFinalImage();
    if (finalImage)
    {
        finalImage->saveTIFF(cstring);
        delete finalImage;
    }        
}

void deSaveButtonsPanel::send(wxCommandEvent &event)
{
    const std::string fileName = "/tmp/delaboratory.tiff";

    deFinalImage* finalImage = project->generateFinalImage();
    if (finalImage)
    {
        finalImage->saveTIFF(fileName);
        delete finalImage;

        const wxString command = _T("gimp ") + wxString::FromAscii(fileName.c_str());
        wxExecute(command);
    }        
}

deSaveButtonsPanel::deSaveButtonsPanel(wxWindow* parent, deProject* _project)
:wxPanel(parent), project(_project)
{
    wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

    buttonSaveJPEG = new wxButton(this, wxID_ANY, _T("save 8-bit JPEG"));
    Connect(buttonSaveJPEG->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deSaveButtonsPanel::saveJPEG));
    sizer->Add(buttonSaveJPEG, 1, wxEXPAND);

    buttonSaveTIFF = new wxButton(this, wxID_ANY, _T("save 16-bit TIFF"));
    Connect(buttonSaveTIFF->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deSaveButtonsPanel::saveTIFF));
    sizer->Add(buttonSaveTIFF, 1, wxEXPAND);

    buttonSend = new wxButton(this, wxID_ANY, _T("send TIFF to GIMP"));
    Connect(buttonSend->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deSaveButtonsPanel::send));
    sizer->Add(buttonSend, 1, wxEXPAND);

    SetSizer(sizer);

}

deSaveButtonsPanel::~deSaveButtonsPanel()
{
}

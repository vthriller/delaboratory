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

#include "zoom_panel.h"
#include "zoom_manager.h"
#include "image_area_panel.h"

deZoomPanel::deZoomPanel(wxWindow* parent, deZoomManager& _zoomManager)
:wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize), zoomManager(_zoomManager)
{
    imageAreaPanel = NULL;

    wxSizer* sizerS = new wxStaticBoxSizer(wxHORIZONTAL, this, _T("zoom"));
    SetSizer(sizerS);

    zoomIn = new wxButton(this, wxID_ANY, _T("select"), wxDefaultPosition, wxSize(60,25));
    sizerS->Add(zoomIn);

    zoomOut = new wxButton(this, wxID_ANY, _T("full"), wxDefaultPosition, wxSize(60,25));
    sizerS->Add(zoomOut);

    Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deZoomPanel::click));

    updateButtons();

}

void deZoomPanel::setImageAreaPanel(deImageAreaPanel* _imageAreaPanel)
{
    imageAreaPanel = _imageAreaPanel;
}

deZoomPanel::~deZoomPanel()
{
}

void deZoomPanel::click(wxCommandEvent &event)
{
    int id = event.GetId();

    if (zoomIn->GetId() == id)
    {
        zoomManager.enableSelectionMode();        
    }      

    if (zoomOut->GetId() == id)
    {
        zoomManager.fullZoomOut();
        if (imageAreaPanel)
        {
            imageAreaPanel->updateSize(true);
        }
    }      

    updateButtons();

}   

void deZoomPanel::updateButtons()
{
    if (zoomManager.isInSelectionMode())
    {
        zoomIn->Disable();
        zoomOut->Disable();
        return;
    }

    if (zoomManager.isZoomed())
    {
        //zoomIn->Disable();
        zoomIn->Enable();
        zoomOut->Enable();
    }
    else
    {
        zoomIn->Enable();
        zoomOut->Disable();
    }

}

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

#include "image_area_panel.h"
#include "size.h"
#include "project.h"
#include "image_panel.h"
#include "channel_manager.h"
#include "layer_processor.h"
#include "zoom_panel.h"
#include "str.h"
#include "gui.h"

void deImageAreaPanel::resize(wxSizeEvent& event)
{
    logInfo("SIZE event in image area panel");
    updateSize(true);
    Refresh();
}

void deImageAreaPanel::updateSize(bool canSkip)
{
    wxSize s = GetSize();
    logInfo("image area panel update size " + str(s.GetWidth()) + "x" + str(s.GetHeight()));

    const deSize ps(s.GetWidth(), s.GetHeight());

    deSize fit = project.onImageAreaChangeSize(ps, canSkip);

    logInfo("set image panel size " + str(fit.getW()) + "x" + str(fit.getH()));
    imagePanel->SetMinSize(wxSize(fit.getW(), fit.getH()));

    Layout();
}

deImageAreaPanel::deImageAreaPanel(wxWindow* parent, deProject& _project, deSamplerManager& _samplerManager, deZoomManager& _zoomManager, deZoomPanel* zoomPanel, deGUI& gui)
:wxPanel(parent), project(_project)
{   
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    imagePanel = new deImagePanel(this, project, _samplerManager, _zoomManager, zoomPanel);
    sizer->Add(imagePanel, 0, wxCENTER);

    gui.setImageAreaPanel(this);

    zoomPanel->setImageAreaPanel(this);

    Connect(wxEVT_SIZE, wxSizeEventHandler(deImageAreaPanel::resize));
}

deImageAreaPanel::~deImageAreaPanel()
{
}

deImagePanel* deImageAreaPanel::getImagePanel()
{
    return imagePanel;
}

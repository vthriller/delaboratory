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

#include "add_layer_dialog.h"
#include "layer_list_panel.h"
#include "mixer_layer.h"
#include "blend_layer.h"
#include "curves_layer.h"
#include "project.h"
#include "exception.h"
#include "preview.h"
#include "conversion_layer.h"

void deAddLayerFrame::click(wxCommandEvent &event)
{
    try
    {
        Close();

        deLayerStack& stack = project->getLayerStack();

        int id = event.GetId();

        deLayer* layer = NULL;

        if (id == curvesButton->GetId())
        {
            layer = new deCurvesLayer("curves");
        }

        if (id == mixerButton->GetId())
        {
            layer = new deMixerLayer("mixer");
        }

        if (id == convertButton->GetId())
        {
            layer = new deConversionLayer("conversion");
        }

        if (id == blendButton->GetId())
        {
            layer = new deBlendLayer("blend");
        }

        if (layer)
        {
            layer->changeSourceLayer(project->getVisibleLayerID(), stack);
            project->addLayer(layer);
        }         

    }
    catch (deException& e)
    {
    }
}

deAddLayerFrame::deAddLayerFrame(deLayerListPanel* _panel, deProject* _project)
:wxFrame(_panel, wxID_ANY, _T("add layer"), wxDefaultPosition, wxDefaultSize), panel(_panel), project(_project)
{
    deLayer* source = project->getVisibleLayer();
    std::vector<deColorSpace> convertColorSpaces;
    if (source)
    {
        deColorSpace currentColorSpace = source->getColorSpace();
        getSupportedConversions(currentColorSpace, convertColorSpaces);
    }

    std::vector<deColorSpace> colorSpaces;
    getSupportedColorSpaces(colorSpaces);

    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    curvesButton = new wxButton(this, wxID_ANY, _T("curves"), wxDefaultPosition, wxSize(-1, 50));
    sizer->Add(curvesButton, 1, wxEXPAND);
    mixerButton = new wxButton(this, wxID_ANY, _T("mixer"), wxDefaultPosition, wxSize(-1, 50));
    sizer->Add(mixerButton, 1, wxEXPAND);
    convertButton = new wxButton(this, wxID_ANY, _T("convert"), wxDefaultPosition, wxSize(-1, 50));
    sizer->Add(convertButton, 1, wxEXPAND);
    blendButton = new wxButton(this, wxID_ANY, _T("blend"), wxDefaultPosition, wxSize(-1, 50));
    sizer->Add(blendButton, 1, wxEXPAND);

    Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deAddLayerFrame::click));

    SetSizer(sizer);

    Fit();
}

deAddLayerFrame::~deAddLayerFrame()
{
}

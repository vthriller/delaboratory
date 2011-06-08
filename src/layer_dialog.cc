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

#include "layer_dialog.h"
#include "layer.h"
#include "project.h"
#include <iostream>
#include "color_space_choice.h"
#include "layer_choice.h"
#include "action_frame.h"
#include <wx/sizer.h>

void deLayerDialog::clickApply(wxCommandEvent &event)
{
    Close(TRUE);

    wxString name = editName->GetValue();
    std::string s( name.mb_str() );
    layer.setName(s);

    if (sourceChoice)
    {
            int source = sourceChoice->GetCurrentSelection();
            int currentSource = layer.getSourceLayerID();
            if (source != currentSource)
            {
                layer.changeSourceLayer(source, project->getLayerStack());
            }
    }

    if (colorSpaceChoice)
    {
            deColorSpace currentColorSpace = layer.getColorSpace();
            int c = colorSpaceChoice->GetCurrentSelection();
            deColorSpace colorSpace = colorSpaces[c];
            if (colorSpace != currentColorSpace)
            {
                layer.changeColorSpace(colorSpace, project->getLayerStack());
            }
    }

    if (overlayChoice)
    {
            int overlay = overlayChoice->GetCurrentSelection();
            int currentOverlay = layer.getOverlayLayerID();
            if (overlay != currentOverlay)
            {
                layer.changeOverlayLayer(overlay, project->getLayerStack());
            }
    }

    project->getLayerStack().updateColorSpaces();
    project->getPreviewStack().updatePreviews(layerNumber);

    deLayer* layer = project->getVisibleLayer();
    if (!layer)
    {
        return;
    }
    const deColorSpace colorSpace = layer->getColorSpace();

    project->getGUI().updateAfterSetView(colorSpace);
    project->getGUI().refreshView();

}

void deLayerDialog::clickCancel(wxCommandEvent &event)
{
    Close(TRUE);

}

void deLayerDialog::clickAction(wxCommandEvent &event)
{
    Close(TRUE);
    deActionFrame* frame = layer.getActionFrame();
    if (!frame)
    {
        frame = layer.createActionFrame(parent, layerNumber, project);
    }        
    if (frame)
    {
        frame->Show();
    }        
}

deLayerDialog::deLayerDialog(wxWindow *_parent, deLayer& _layer, int _layerNumber, deProject* _project, const std::string& action)
:wxDialog(_parent, wxID_ANY, _T("layer"), wxDefaultPosition, wxDefaultSize), parent(_parent), layer(_layer), project(_project), layerNumber(_layerNumber)
{
    colorSpaceChoice = NULL;
    sourceChoice = NULL;
    overlayChoice = NULL;

    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    //gridSizer = new wxGridSizer(2, 0, 0, 0);
    gridSizer = new wxGridSizer(0, 2, 0, 0);

    wxStaticText* labelId = new wxStaticText(this, wxID_ANY, _T("layer id:") );
    gridSizer->Add(labelId);
    wxStaticText* valueId = new wxStaticText(this, wxID_ANY, wxString::Format(_T("%i"), layerNumber));
    gridSizer->Add(valueId);

    wxStaticText* labelType = new wxStaticText(this, wxID_ANY, _T("layer type:") );
    gridSizer->Add(labelType);
    wxStaticText* valueType = new wxStaticText(this, wxID_ANY, wxString::FromAscii(layer.getDescription().c_str()));
    gridSizer->Add(valueType);

    wxStaticText* labelName = new wxStaticText(this, wxID_ANY, _T("layer name:") );
    gridSizer->Add(labelName);
    editName =  new wxTextCtrl(this, wxID_ANY, wxString::FromAscii(layer.getName().c_str()), wxDefaultPosition, wxSize(200, -1));
    gridSizer->Add(editName);

    sizer->Add(gridSizer, 0, wxEXPAND);

    wxSizer* buttonsSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonApply = new wxButton(this, wxID_ANY, _T("Apply"));
    buttonCancel = new wxButton(this, wxID_ANY, _T("Cancel"));
    buttonsSizer->Add(buttonApply, 1, wxEXPAND);
    buttonsSizer->Add(buttonCancel, 1, wxEXPAND);
    sizer->Add(buttonsSizer, 0, wxEXPAND);

    if (action.size() > 0)
    {
        wxSizer* actionSizer = new wxBoxSizer(wxHORIZONTAL);
        buttonAction = new wxButton(this, wxID_ANY, wxString::FromAscii(action.c_str()));
        actionSizer->Add(buttonAction, 1, wxEXPAND);
        sizer->Add(actionSizer, 0, wxEXPAND);
        Connect(buttonAction->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deLayerDialog::clickAction));
    }        

    Connect(buttonApply->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deLayerDialog::clickApply));
    Connect(buttonCancel->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deLayerDialog::clickCancel));

    SetSizer(sizer);

    Fit();

}

deLayerDialog::~deLayerDialog()
{
}

void deLayerDialog::addColorSpaceChoice()
{
    wxStaticText* labelColorSpace = new wxStaticText(this, wxID_ANY, _T("color space:") );
    gridSizer->Add(labelColorSpace);

    deColorSpace currentColorSpace = layer.getColorSpace();

    colorSpaceChoice = makeColorSpaceChoice(this, colorSpaces, currentColorSpace);

    gridSizer->Add(colorSpaceChoice);

    Fit();
}

void deLayerDialog::addSourceChoice()
{
    wxStaticText* labelSource = new wxStaticText(this, wxID_ANY, _T("source layer:") );
    gridSizer->Add(labelSource);

    sourceChoice = makeLayerChoice(this, layerNumber, layer.getSourceLayerID());
    gridSizer->Add(sourceChoice);

    Fit();
}

void deLayerDialog::addOverlayChoice()
{
    wxStaticText* labelOverlay = new wxStaticText(this, wxID_ANY, _T("overlay layer:") );
    gridSizer->Add(labelOverlay);

    overlayChoice = makeLayerChoice(this, layerNumber, layer.getOverlayLayerID());
    gridSizer->Add(overlayChoice);

    Fit();
}

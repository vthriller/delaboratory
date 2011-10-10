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

#include "create_layer_dialog.h"
#include "layer_factory.h"
#include "layer.h"

deCreateLayerDialog::deCreateLayerDialog(wxWindow *parent, const std::string& _type, deProject& _project)
:wxDialog(parent, wxID_ANY, _T("create new layer"), wxDefaultPosition, wxDefaultSize),
 type(_type),
 project(_project)
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    wxSizer* sizerD = new wxStaticBoxSizer(wxVERTICAL, this,  _T(""));
    sizer->Add(sizerD, 0, wxALIGN_CENTER);

    std::string name = "layer";

    if (type == "curves")
    {
        sizerD->Add(new wxStaticText(this, wxID_ANY, _T("curves layer")), 0, wxALIGN_CENTER);
        sizerD->AddSpacer(20);
        sizerD->Add(new wxStaticText(this, wxID_ANY, _T("each channel will be processed by curve")), 0, wxALIGN_CENTER);
        sizerD->AddSpacer(20);
        sizerD->Add(new wxStaticText(this, wxID_ANY, _T("change contrast with L curve (in LAB)")), 0, wxALIGN_CENTER);
        sizerD->Add(new wxStaticText(this, wxID_ANY, _T("change color with A/B curves (in LAB)")), 0, wxALIGN_CENTER);
        sizerD->Add(new wxStaticText(this, wxID_ANY, _T("change saturation with S curve (in HSL/HSV)")), 0, wxALIGN_CENTER);
        sizerD->Add(new wxStaticText(this, wxID_ANY, _T("change everything with RGB curves")), 0, wxALIGN_CENTER);

        name = "curves";
    }

    if (type == "LAB")
    {
        sizerD->Add(new wxStaticText(this, wxID_ANY, _T("LAB conversion layer")), 0, wxALIGN_CENTER);
        sizerD->AddSpacer(20);
        sizerD->Add(new wxStaticText(this, wxID_ANY, _T("use this layer to start processing in LAB colorspace")), 0, wxALIGN_CENTER);
        sizerD->Add(new wxStaticText(this, wxID_ANY, _T("LAB separates color from luminance")), 0, wxALIGN_CENTER);

        name = "LAB";
    }

    if (type == "RGB")
    {
        sizerD->Add(new wxStaticText(this, wxID_ANY, _T("RGB conversion layer")), 0, wxALIGN_CENTER);
        sizerD->AddSpacer(20);
        sizerD->Add(new wxStaticText(this, wxID_ANY, _T("use this layer to go back into RGB colorspace")), 0, wxALIGN_CENTER);

        name = "RGB";
    }

    if (type == "HSV")
    {
        sizerD->Add(new wxStaticText(this, wxID_ANY, _T("HSV conversion layer")), 0, wxALIGN_CENTER);
        sizerD->AddSpacer(20);
        sizerD->Add(new wxStaticText(this, wxID_ANY, _T("use this layer to start processing in HSV colorspace")), 0, wxALIGN_CENTER);
        sizerD->Add(new wxStaticText(this, wxID_ANY, _T("HSV allows operations on hue and saturation")), 0, wxALIGN_CENTER);

        name = "HSV";
    }

    if (type == "HSL")
    {
        sizerD->Add(new wxStaticText(this, wxID_ANY, _T("HSL conversion layer")), 0, wxALIGN_CENTER);
        sizerD->AddSpacer(20);
        sizerD->Add(new wxStaticText(this, wxID_ANY, _T("use this layer to start processing in HSL colorspace")), 0, wxALIGN_CENTER);
        sizerD->Add(new wxStaticText(this, wxID_ANY, _T("HSL allows operations on hue and saturation")), 0, wxALIGN_CENTER);

        name = "HSL";
    }

    sizer->AddSpacer(20);

    deViewManager& viewManager = project.getViewManager();
    int view = viewManager.getView();
    view++;

    // GRID

    wxFlexGridSizer* gridSizer = new wxFlexGridSizer(2);
    gridSizer->SetFlexibleDirection(wxHORIZONTAL);
    sizer->Add(gridSizer, 0, wxALIGN_CENTER);

    wxStaticText* nameLabel = new wxStaticText(this, wxID_ANY, _T("name: ") );
    gridSizer->Add(nameLabel, 0, wxALIGN_CENTER);

    nameEdit = new wxTextCtrl(this, wxID_ANY, wxString::FromAscii(name.c_str()), wxDefaultPosition);
    gridSizer->Add(nameEdit, 0, wxEXPAND);

    wxStaticText* sourceLabel = new wxStaticText(this, wxID_ANY, _T("source layer: ") );
    gridSizer->Add(sourceLabel, 0, wxALIGN_CENTER);

    int i;
    wxString* layerStrings = new wxString [view];
    for (i = 0; i < view; i++)
    {
        layerStrings[i] = wxString::Format(_T("%i"), i);
    }        

    source = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(200, -1), view, layerStrings);
    source->SetSelection(view-1);

    delete [] layerStrings;

    gridSizer->Add(source, 0, wxEXPAND);

    sizer->AddSpacer(20);

    // OK / CANCEL

    wxSizer* sizerB = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(sizerB, 0, wxALIGN_CENTER);

    ok = new wxButton(this, wxID_ANY, _T("OK"));
    sizerB->Add(ok, 1, wxEXPAND);
    
    cancel = new wxButton(this, wxID_ANY, _T("Cancel"));
    sizerB->Add(cancel, 1, wxEXPAND);

    // FIT

    Fit();

    Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deCreateLayerDialog::click));
    Connect(wxEVT_CLOSE_WINDOW, wxCommandEventHandler(deCreateLayerDialog::close));

    project.disableKeys();
}

deCreateLayerDialog::~deCreateLayerDialog()
{
}

void deCreateLayerDialog::click(wxCommandEvent &event)
{
    int id = event.GetId();

    if (ok->GetId() == id)
    {
        doCreateLayer();
        project.enableKeys();
        EndModal(1);
    }

    if (cancel->GetId() == id)
    {
        project.enableKeys();
        EndModal(0);
    }

}

void deCreateLayerDialog::close(wxCommandEvent &event)
{
    project.enableKeys();
    EndModal(0);
}


void deCreateLayerDialog::doCreateLayer()
{
    int s = source->GetSelection();

    deLayerStack& layerStack = project.getLayerStack();

    deViewManager& viewManager = project.getViewManager();

    int view = viewManager.getView();
    deColorSpace colorSpace = layerStack.getLayer(s)->getColorSpace();

    deChannelManager& channelManager = project.getPreviewChannelManager();

    wxString valueStr = nameEdit->GetValue();
    std::string n(valueStr.mb_str());

    if (type == "curves")
    {
        deLayer* layer = createLayer("curves", s, colorSpace, layerStack, channelManager, viewManager, n);
        project.addLayer(layer);
    }

    if (type == "LAB")
    {
        deLayer* layer = createLayer("conversion", s, deColorSpaceLAB, layerStack, channelManager, viewManager, n);
        project.addLayer(layer);
    }

    if (type == "RGB")
    {
        deLayer* layer = createLayer("conversion", s, deColorSpaceRGB, layerStack, channelManager, viewManager, n);
        project.addLayer(layer);
    }

    if (type == "HSV")
    {
        deLayer* layer = createLayer("conversion", s, deColorSpaceHSV, layerStack, channelManager, viewManager, n);
        project.addLayer(layer);
    }

    if (type == "HSL")
    {
        deLayer* layer = createLayer("conversion", s, deColorSpaceHSL, layerStack, channelManager, viewManager, n);
        project.addLayer(layer);
    }

    project.setLastView();
}

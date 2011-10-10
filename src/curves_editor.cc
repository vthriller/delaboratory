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

#include "curves_editor.h"
#include "color_space.h"
#include "curves_layer.h"
#include "curves_panel.h"
#include "gradient_panel.h"

deCurvesEditor::deCurvesEditor(wxWindow *parent, deActionLayer& _layer)
:deActionFrame(parent, _layer)
{
    deCurvesLayer& curvesLayer = dynamic_cast<deCurvesLayer&>(_layer);

    deColorSpace colorSpace = layer.getColorSpace();

    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    int n = getColorSpaceSize(colorSpace);
    wxString* channelStrings = new wxString [n];
    int i;
    for (i = 0; i < n; i++)
    {
        channelStrings[i] = wxString::FromAscii(getChannelName(colorSpace,i).c_str());
    }        

    channelChoice =  new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(200, -1), n, channelStrings);
    channelChoice->SetSelection(0);

    delete [] channelStrings;

    sizer->Add(channelChoice);

    wxSizer* sizerSB = new wxStaticBoxSizer(wxHORIZONTAL, this, _T(""));
    sizer->Add(sizerSB);

    wxSizer* sizerC = new wxFlexGridSizer(2, 8, 8);
    sizerSB->Add(sizerC);

    curvesPanel = new deCurvesPanel(this, curvesLayer );

    int barSize = 16;

    leftBar = new deGradientPanel(this, wxSize(barSize, CURVES_PANEL_SIZE_Y), colorSpace, 0, -1, -1, -1, -1);
    bottomBar = new deGradientPanel(this, wxSize(CURVES_PANEL_SIZE_X, barSize), colorSpace, 0, -1, -1, -1, -1);

    //sizer->Add(curvesPanel);
    sizerC->Add(leftBar, 0, wxCENTER);
    sizerC->Add(curvesPanel, 0, wxCENTER);
    sizerC->Add(-1, 0, wxCENTER);
    sizerC->Add(bottomBar, 0, wxCENTER);

    wxSizer* sizerB = new wxGridSizer(5);
    sizer->Add(sizerB, 0, wxCENTER);


    reset = new wxButton(this, wxID_ANY, _T("reset"), wxDefaultPosition, wxSize(60,25));
    sizerB->Add(reset, 0);

    invert = new wxButton(this, wxID_ANY, _T("invert"), wxDefaultPosition, wxSize(60,25));
    sizerB->Add(invert, 0);

    const0 = new wxButton(this, wxID_ANY, _T("0"), wxDefaultPosition, wxSize(60,25));
    sizerB->Add(const0, 0);

    const05 = new wxButton(this, wxID_ANY, _T("0.5"), wxDefaultPosition, wxSize(60,25));
    sizerB->Add(const05, 0);

    const1 = new wxButton(this, wxID_ANY, _T("1"), wxDefaultPosition, wxSize(60,25));
    sizerB->Add(const1, 0);

    angle1 = new wxButton(this, wxID_ANY, _T("+ 1/4"), wxDefaultPosition, wxSize(60,25));
    sizerB->Add(angle1, 0);
    angle2 = new wxButton(this, wxID_ANY, _T("+ 2/4"), wxDefaultPosition, wxSize(60,25));
    sizerB->Add(angle2, 0);
    angle3 = new wxButton(this, wxID_ANY, _T("+ 3/4"), wxDefaultPosition, wxSize(60,25));
    sizerB->Add(angle3, 0);
    angle4 = new wxButton(this, wxID_ANY, _T("- 1/4"), wxDefaultPosition, wxSize(60,25));
    sizerB->Add(angle4, 0);
    angle5 = new wxButton(this, wxID_ANY, _T("- 2/4"), wxDefaultPosition, wxSize(60,25));
    sizerB->Add(angle5, 0);

    /*
    app1 = new wxButton(this, wxID_ANY, wxString::FromAscii(src1.c_str()), wxDefaultPosition, wxSize(60,25));
    sizerB->Add(app1, 0);

    app2 = new wxButton(this, wxID_ANY, wxString::FromAscii(src2.c_str()), wxDefaultPosition, wxSize(60,25));
    sizerB->Add(app2, 0);

    mix1 = new wxButton(this, wxID_ANY, _T("0.1"), wxDefaultPosition, wxSize(60,25));
    sizerB->Add(mix1, 0);

    mix2 = new wxButton(this, wxID_ANY, _T("0.3"), wxDefaultPosition, wxSize(60,25));
    sizerB->Add(mix2, 0);

    mix3 = new wxButton(this, wxID_ANY, _T("-0.3"), wxDefaultPosition, wxSize(60,25));
    sizerB->Add(mix3, 0);

    mix4 = new wxButton(this, wxID_ANY, _T("-0.5"), wxDefaultPosition, wxSize(60,25));
    sizerB->Add(mix4, 0);
    */

    sizer->Layout();
    Fit();

    Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deCurvesEditor::click));
    Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(deCurvesEditor::choose));
}

deCurvesEditor::~deCurvesEditor()
{
}

void deCurvesEditor::choose(wxCommandEvent &event)
{
    int b = channelChoice->GetCurrentSelection();
    curvesPanel->changeChannel(b);
    leftBar->changeChannel(b);
    bottomBar->changeChannel(b);
}

void deCurvesEditor::click(wxCommandEvent &event)
{
    int id = event.GetId();

    if (reset->GetId() == id)
    {
        curvesPanel->reset();
    }      

    if (invert->GetId() == id)
    {
        curvesPanel->invert();
    }      

    if (const0->GetId() == id)
    {
        curvesPanel->setConst(0.0);
    }      

    if (const05->GetId() == id)
    {
        curvesPanel->setConst(0.5);
    }      

    if (const1->GetId() == id)
    {
        curvesPanel->setConst(1);
    }      

    if (angle1->GetId() == id)
    {
        curvesPanel->setAngle(1);
    }      
    if (angle2->GetId() == id)
    {
        curvesPanel->setAngle(2);
    }      
    if (angle3->GetId() == id)
    {
        curvesPanel->setAngle(3);
    }      
    if (angle4->GetId() == id)
    {
        curvesPanel->setAngle(4);
    }      
    if (angle5->GetId() == id)
    {
        curvesPanel->setAngle(5);
    }      
}

void deCurvesEditor::onImageClick(deValue x, deValue y)
{
    curvesPanel->onImageClick(x, y);
}

void deCurvesEditor::onKey(int key)
{
    curvesPanel->onKey(key);
}

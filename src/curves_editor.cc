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
#include "color_space_utils.h"
#include "logger.h"
#include "opacity_slider.h"

deCurvesEditor::deCurvesEditor(wxWindow *parent, deLayer& _layer, deLayerProcessor& _layerProcessor, deLayerFrameManager& _frameManager, int _layerIndex, deChannelManager& _channelManager)
:deActionFrame(parent, _layer, _frameManager, _layerIndex)
{
    logMessage("creating curves editor...");
    deCurvesLayer& curvesLayer = dynamic_cast<deCurvesLayer&>(_layer);

    deColorSpace colorSpace = layer.getColorSpace();

    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);
    
    int range = 250;

    deLayerWithBlending& lwb  = dynamic_cast<deLayerWithBlending&>(_layer);
    deSlider* alphaSlider = new deOpacitySlider(this, range, lwb, _layerProcessor, layerIndex);
    sizer->Add(alphaSlider, 0);


    logMessage("creating curves editor... 2 ");

    int n = getColorSpaceSize(colorSpace);
    wxString* channelStrings = new wxString [n];
    int i;
    for (i = 0; i < n; i++)
    {
        channelStrings[i] = wxString::FromAscii(getChannelName(colorSpace,i).c_str());
    }        
    logMessage("creating curves editor... 3 ");

    channelChoice =  new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(200, -1), n, channelStrings);
    channelChoice->SetSelection(0);

    delete [] channelStrings;

    sizer->Add(channelChoice);

    wxSizer* sizerSB = new wxStaticBoxSizer(wxHORIZONTAL, this, _T(""));
    sizer->Add(sizerSB);

    wxSizer* sizerC = new wxFlexGridSizer(2, 8, 8);
    sizerSB->Add(sizerC);

    logMessage("creating curves editor... 4 ");

    curvesPanel = new deCurvesPanel(this, curvesLayer, _layerProcessor, _layerIndex, _channelManager);

    logMessage("creating curves editor... 5 ");

    int barSize = 16;

    leftBar = new deGradientPanel1(this, wxSize(barSize, CURVES_PANEL_SIZE_Y), colorSpace, 0, -1, -1, -1, -1);
    bottomBar = new deGradientPanel1(this, wxSize(CURVES_PANEL_SIZE_X, barSize), colorSpace, 0, -1, -1, -1, -1);

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

    s1 = new wxButton(this, wxID_ANY, _T("S 1"), wxDefaultPosition, wxSize(60,25));
    sizerB->Add(s1, 0);
    s2 = new wxButton(this, wxID_ANY, _T("S 2"), wxDefaultPosition, wxSize(60,25));
    sizerB->Add(s2, 0);
    s3 = new wxButton(this, wxID_ANY, _T("S 3"), wxDefaultPosition, wxSize(60,25));
    sizerB->Add(s3, 0);
    is1 = new wxButton(this, wxID_ANY, _T("invS 1"), wxDefaultPosition, wxSize(60,25));
    sizerB->Add(is1, 0);
    is2 = new wxButton(this, wxID_ANY, _T("invS 2"), wxDefaultPosition, wxSize(60,25));
    sizerB->Add(is2, 0);

    random1 = new wxButton(this, wxID_ANY, _T("rnd 1"), wxDefaultPosition, wxSize(60,25));
    sizerB->Add(random1, 0);
    random2 = new wxButton(this, wxID_ANY, _T("rnd 2"), wxDefaultPosition, wxSize(60,25));
    sizerB->Add(random2, 0);
    random3 = new wxButton(this, wxID_ANY, _T("rnd 3"), wxDefaultPosition, wxSize(60,25));
    sizerB->Add(random3, 0);
    random4 = new wxButton(this, wxID_ANY, _T("rnd 5"), wxDefaultPosition, wxSize(60,25));
    sizerB->Add(random4, 0);
    random5 = new wxButton(this, wxID_ANY, _T("rnd 9"), wxDefaultPosition, wxSize(60,25));
    sizerB->Add(random5, 0);

    sizer->Layout();
    Fit();

    int b = 0;
    curvesPanel->changeChannel(b);

    Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deCurvesEditor::click));
    Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(deCurvesEditor::choose));
    logMessage("created curves editor...");
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

    if (s1->GetId() == id)
    {
        curvesPanel->setS(1);
    }      
    if (s2->GetId() == id)
    {
        curvesPanel->setS(2);
    }      
    if (s3->GetId() == id)
    {
        curvesPanel->setS(3);
    }      
    if (is1->GetId() == id)
    {
        curvesPanel->setS(-1);
    }      
    if (is2->GetId() == id)
    {
        curvesPanel->setS(-2);
    }      

    if (random1->GetId() == id)
    {
        curvesPanel->addRandom(1);
    }      
    if (random2->GetId() == id)
    {
        curvesPanel->addRandom(2);
    }      
    if (random3->GetId() == id)
    {
        curvesPanel->addRandom(3);
    }      
    if (random4->GetId() == id)
    {
        curvesPanel->addRandom(5);
    }      
    if (random5->GetId() == id)
    {
        curvesPanel->addRandom(9);
    }      
}

bool deCurvesEditor::onImageClick(deValue x, deValue y)
{
    curvesPanel->onImageClick(x, y);
    return true;
}

void deCurvesEditor::onKey(int key)
{
    curvesPanel->onKey(key);
}

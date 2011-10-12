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

#include "blur_frame.h"
#include "blur_layer.h"
#include <iostream>

/*
#include "color_space.h"
#include "curves_layer.h"
#include "curves_panel.h"
#include "gradient_panel.h"
*/

class deBlurRadiusSlider2:public deSlider
{
    private:
        deBlurLayer& layer;

    public:
        deBlurRadiusSlider2(wxWindow *parent, int range, deBlurLayer& _layer)
        :deSlider(parent, "blur radius", range, 0.0, 0.05, 0.0), layer(_layer)
        {
            setValue(layer.getBlurRadius());
        }

        virtual ~deBlurRadiusSlider2()
        {
        }

        virtual void onValueChange(deValue value, bool finished)
        {
            if (finished)
            {
                layer.setBlurRadius(value);
            }                
        }
};        

deBlurFrame::deBlurFrame(wxWindow *parent, deActionLayer& _layer)
:deActionFrame(parent, _layer)
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    deBlurLayer& blurLayer = dynamic_cast<deBlurLayer&>(_layer);

    radius = new deBlurRadiusSlider2(this, 100, blurLayer);
    sizer->Add(radius);

/*

    deColorSpace colorSpace = layer.getColorSpace();


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

    //wxPanel* leftBar = new deGradientPanel(this, wxID_ANY, wxDefaultPosition, wxSize(barSize, CURVES_PANEL_SIZE_Y));
    leftBar = new deGradientPanel(this, wxSize(barSize, CURVES_PANEL_SIZE_Y), colorSpace, 0);
    bottomBar = new deGradientPanel(this, wxSize(CURVES_PANEL_SIZE_X, barSize), colorSpace, 0);

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

    sizer->Layout();

    Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deBlurFrame::click));
    Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(deBlurFrame::choose));
    */

    Fit();
}

deBlurFrame::~deBlurFrame()
{
}

/*
void deBlurFrame::choose(wxCommandEvent &event)
{
    int b = channelChoice->GetCurrentSelection();
    curvesPanel->changeChannel(b);
    leftBar->changeChannel(b);
    bottomBar->changeChannel(b);
}

void deBlurFrame::click(wxCommandEvent &event)
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

void deBlurFrame::onImageClick(deValue x, deValue y)
{
    curvesPanel->onImageClick(x, y);
}
*/

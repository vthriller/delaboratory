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

#include "sampler_manager_frame.h"
#include "control_panel.h"
#include "project.h"
#include "layer.h"
#include "convert_pixel.h"
#include <sstream>
#include <iostream>
#include "str.h"

deSamplerManagerFrame::deSamplerManagerFrame(deControlPanel *parent, deProject& _project)
:deFrame(parent, "samplers"), project(_project)
{
    controlPanel = parent;

    gridSizer = new wxFlexGridSizer(8);
    gridSizer->SetFlexibleDirection(wxHORIZONTAL);
    SetSizer(gridSizer);
    
    buildRows();
    update();

    Connect(wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(deSamplerManagerFrame::select));
    Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(deSamplerManagerFrame::choose));

}

deSamplerManagerFrame::~deSamplerManagerFrame()
{
    controlPanel->closeSamplerManagerFrame();
}

void deSamplerManagerFrame::buildRows()
{
    deSamplerManager& samplerManager = project.getSamplerManager();
    int n = samplerManager.getNumberOfSamplers();

    getSupportedColorSpaces(colorSpaces);

    wxString* colorSpaceStrings = new wxString [colorSpaces.size()];
    unsigned int i;
    int selectedColorSpace = -1;
    for (i = 0; i < colorSpaces.size(); i++)
    {
        colorSpaceStrings[i] = wxString::FromAscii(getColorSpaceName(colorSpaces[i]).c_str());
    }        

    int xyWidth = 50;
    int valueWidth = 70;

    for (i = 0; i < n; i++)
    {
        deSampler* sampler = samplerManager.getSampler(i);

        samplerRows.push_back(deSamplerRow());
        deSamplerRow& row = samplerRows.back();

        row.selected = new wxRadioButton(this, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize);
        gridSizer->Add(row.selected, 0, wxALIGN_CENTER);

        row.x = new wxStaticText(this, wxID_ANY, _T("x"), wxDefaultPosition, wxSize(xyWidth, -1));
        gridSizer->Add(row.x, 1, wxALIGN_CENTER);

        row.y = new wxStaticText(this, wxID_ANY, _T("y"), wxDefaultPosition, wxSize(xyWidth, -1));
        gridSizer->Add(row.y, 0, wxALIGN_CENTER);

        row.colorSpace = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, colorSpaces.size(), colorSpaceStrings);
        gridSizer->Add(row.colorSpace, 0, wxALIGN_CENTER);

        deColorSpace colorSpace = sampler->getColorSpace();
        int j;
        for (j = 0; j < colorSpaces.size(); j++)
        {
            if (colorSpaces[j] == colorSpace)
            {
                row.colorSpace->SetSelection(j);
            }
        }        

        row.v1 = new wxStaticText(this, wxID_ANY, _T("v1"), wxDefaultPosition, wxSize(valueWidth, -1));
        gridSizer->Add(row.v1, 0, wxALIGN_CENTER);
        row.v2 = new wxStaticText(this, wxID_ANY, _T("v2"), wxDefaultPosition, wxSize(valueWidth, -1));
        gridSizer->Add(row.v2, 0, wxALIGN_CENTER);
        row.v3 = new wxStaticText(this, wxID_ANY, _T("v3"), wxDefaultPosition, wxSize(valueWidth, -1));
        gridSizer->Add(row.v3, 0, wxALIGN_CENTER);
        row.v4 = new wxStaticText(this, wxID_ANY, _T("v4"), wxDefaultPosition, wxSize(valueWidth, -1));
        gridSizer->Add(row.v4, 0, wxALIGN_CENTER);
    }       
    delete []  colorSpaceStrings;

    Fit();
};

void deSamplerManagerFrame::update()
{
    const deViewManager& viewManager = project.getViewManager();
    int view = viewManager.getView();

    deLayerStack& layerStack = project.getLayerStack();
    deLayer* layer = layerStack.getLayer(view);

    const deImage& image = layer->getImage();

    deChannelManager& channelManager = project.getPreviewChannelManager();
    deSize channelSize = channelManager.getChannelSize();
    
    deSamplerManager& samplerManager = project.getSamplerManager();
    int n = samplerManager.getNumberOfSamplers();

    int i;

    std::ostringstream oss;
    oss.setf(std::ios_base::fixed);
    oss.precision(3);

    for (i = 0; i < n; i++)
    {
        deSampler* sampler = samplerManager.getSampler(i);
        deSamplerRow& row = samplerRows[i];
        deValue x = sampler->getX();
        deValue y = sampler->getY();

        if ((x >= 0) && (y >= 0) && (x <= 1) && (y<= 1))
        {
            row.x->Show();
            row.y->Show();

            {
                oss.str("");
                oss << x;

                row.x->SetLabel(wxString::FromAscii(oss.str().c_str()));
            }            

            {
                oss.str("");
                oss << y;

                row.y->SetLabel(wxString::FromAscii(oss.str().c_str()));
            }            

            int xx = channelSize.getW() * sampler->getX();
            int yy = channelSize.getH() * sampler->getY();
            int p = channelSize.getW() * yy + xx;

            deColorSpace colorSpace = sampler->getColorSpace();
            int n = getColorSpaceSize(colorSpace);

            deValue v1;
            deValue v2;
            deValue v3;
            deValue v4;

            convertPixel(image, p, colorSpace, v1, v2, v3, v4);

            {
                oss.str("");
                oss << getPresentationValue(colorSpace, 0, v1);

                row.v1->SetLabel(wxString::FromAscii(oss.str().c_str()));
            }            

            row.v1->Show();

            if (n == 1)
            {
                row.v2->Hide();
                row.v3->Hide();
                row.v4->Hide();
                row.v2->SetLabel(_T(""));
                row.v3->SetLabel(_T(""));
                row.v4->SetLabel(_T(""));
            }
            else
            {
                row.v2->Show();
                row.v3->Show();

                oss.str("");
                oss << getPresentationValue(colorSpace, 1, v2);

                row.v2->SetLabel(wxString::FromAscii(oss.str().c_str()));
                oss.str("");
                oss << getPresentationValue(colorSpace, 2, v3);

                row.v3->SetLabel(wxString::FromAscii(oss.str().c_str()));

                if (n == 3)
                {
                    row.v4->Hide();
                    row.v4->SetLabel(_T(""));
                }
                else
                {
                    row.v4->Show();
                    oss.str("");
                    oss << getPresentationValue(colorSpace, 3, v4);

                    row.v4->SetLabel(wxString::FromAscii(oss.str().c_str()));
                }
            }           
        }
        else
        {
            row.x->Hide();
            row.y->Hide();
            row.v1->Hide();
            row.v2->Hide();
            row.v3->Hide();
            row.v4->Hide();
        }
    }        

    Layout();

}

void deSamplerManagerFrame::select(wxCommandEvent &event)
{
    deSamplerManager& samplerManager = project.getSamplerManager();

    int id = event.GetId();
    std::vector<deSamplerRow>::const_iterator i;
    int index = 0;
    for (i = samplerRows.begin(); i != samplerRows.end(); i++)
    {
        const deSamplerRow& row = *i;
        if (row.selected->GetId() == id)
        {
            samplerManager.setSelected(index);
        }
        index++;
    }
}

void deSamplerManagerFrame::choose(wxCommandEvent &event)
{
    deSamplerManager& samplerManager = project.getSamplerManager();

    int id = event.GetId();
    std::vector<deSamplerRow>::const_iterator i;
    int index = 0;
    for (i = samplerRows.begin(); i != samplerRows.end(); i++)
    {
        const deSamplerRow& row = *i;
        if (row.colorSpace->GetId() == id)
        {
            int c = row.colorSpace->GetSelection();
            deColorSpace colorSpace = colorSpaces[c];
            deSampler* sampler = samplerManager.getSampler(index);
            sampler->setColorSpace(colorSpace);
        }
        index++;
    }

    update();
}

void deSamplerManagerFrame::dumpColor()
{
    deSamplerManager& samplerManager = project.getSamplerManager();

    std::vector<deSamplerRow>::const_iterator i;
    int index = 0;
    for (i = samplerRows.begin(); i != samplerRows.end(); i++)
    {
        const deSamplerRow& row = *i;
        if (row.selected->GetValue() == 1)
        {
            wxString v1 = row.v1->GetLabel();
            wxString v2 = row.v2->GetLabel();
            wxString v3 = row.v3->GetLabel();
            wxString v4 = row.v4->GetLabel();

            wxString s = v1 + _T(" ") + v2 + _T(" ") + v3 + _T(" ") + v4;

            std::cout << str(s) << std::endl;
        }
        index++;
    }
}

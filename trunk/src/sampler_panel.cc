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

#include "sampler_panel.h"
#include "sampler.h"
#include "project.h"
#include "layer.h"
#include "convert_pixel.h"
#include "image.h"
#include <sstream>

deSamplerPanel::deSamplerPanel(wxWindow* parent, deSampler& _sampler, deProject& _project)
:wxPanel(parent, wxID_ANY, wxDefaultPosition), sampler(_sampler), project(_project)
{
    //wxSizer* sizer = new wxStaticBoxSizer(wxVERTICAL, this, _T(""));
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    wxSizer* sizerG = new wxGridSizer(3);
    sizer->Add(sizerG);

    int xyWidth = 70;
    int valueWidth = 70;

    getSupportedColorSpaces(colorSpaces);

    wxString* colorSpaceStrings = new wxString [colorSpaces.size()];
    unsigned int i;
    for (i = 0; i < colorSpaces.size(); i++)
    {
        colorSpaceStrings[i] = wxString::FromAscii(getColorSpaceName(colorSpaces[i]).c_str());
    }        

    colorSpace = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, colorSpaces.size(), colorSpaceStrings);
    sizerG->Add(colorSpace);

    wxSizer* sizerS = new wxStaticBoxSizer(wxHORIZONTAL, this, _T(""));
    sizer->Add(sizerS);

    v1 = new wxStaticText(this, wxID_ANY, _T("v1"), wxDefaultPosition, wxSize(valueWidth, -1));
    sizerS->Add(v1);
    v2 = new wxStaticText(this, wxID_ANY, _T("v2"), wxDefaultPosition, wxSize(valueWidth, -1));
    sizerS->Add(v2);
    v3 = new wxStaticText(this, wxID_ANY, _T("v3"), wxDefaultPosition, wxSize(valueWidth, -1));
    sizerS->Add(v3);
    v4 = new wxStaticText(this, wxID_ANY, _T("v4"), wxDefaultPosition, wxSize(valueWidth, -1));
    sizerS->Add(v4);

    delete []  colorSpaceStrings;

}

deSamplerPanel::~deSamplerPanel()
{
}

void deSamplerPanel::update()
{
    const deViewManager& viewManager = project.getViewManager();
    int view = viewManager.getView();

    deLayerStack& layerStack = project.getLayerStack();
    deLayer* layer = layerStack.getLayer(view);

    const deImage& image = layer->getImage();

    deChannelManager& channelManager = project.getPreviewChannelManager();
    deSize channelSize = channelManager.getChannelSize();

    deValue x = sampler.getX();
    deValue y = sampler.getY();

    if ((x >= 0) && (y >= 0) && (x <= 1) && (y<= 1))
    {
        int xx = channelSize.getW() * x;
        int yy = channelSize.getH() * y;
        int p = channelSize.getW() * yy + xx;

        deColorSpace colorSpace = sampler.getColorSpace();
        int n = getColorSpaceSize(colorSpace);

        deValue vv1;
        deValue vv2;
        deValue vv3;
        deValue vv4;

        image.lockRead();

        convertPixel(image, p, colorSpace, vv1, vv2, vv3, vv4);

        image.unlockRead();

        std::ostringstream oss;
        oss.setf(std::ios_base::fixed);
        oss.precision(3);

        {
            oss.str("");
            oss << getPresentationValue(colorSpace, 0, vv1);
            v1->SetLabel(wxString::FromAscii(oss.str().c_str()));
        }            
        {
            oss.str("");
            oss << getPresentationValue(colorSpace, 0, vv2);
            v2->SetLabel(wxString::FromAscii(oss.str().c_str()));
        }            
        {
            oss.str("");
            oss << getPresentationValue(colorSpace, 0, vv3);
            v3->SetLabel(wxString::FromAscii(oss.str().c_str()));
        }            
        {
            oss.str("");
            oss << getPresentationValue(colorSpace, 0, vv4);
            v4->SetLabel(wxString::FromAscii(oss.str().c_str()));
        }            
    }

}

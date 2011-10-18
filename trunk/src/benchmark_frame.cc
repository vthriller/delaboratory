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

#include "benchmark_frame.h"
#include "benchmarks.h"
#include "str.h"

void deBenchmarkFrame::addBenchmark(const std::string& s, std::string name, int value)
{
    wxStaticText* ls = new wxStaticText(this, wxID_ANY, wxString::FromAscii(s.c_str()));
    sizer->Add(ls);
    wxStaticText* l = new wxStaticText(this, wxID_ANY, wxString::FromAscii(name.c_str()));
    sizer->Add(l);

    int vv = value / 10;
    deValue vvv = vv / 100.0;

    wxStaticText* r = new wxStaticText(this, wxID_ANY, _T(""));

    r->SetMinSize(wxSize(100, -1));

    sizer->Add(r);
    results.push_back(r);
}

void deBenchmarkFrame::addBenchmarkBlur(deSize size, deValue radius, deBlurType type)
{
    std::string name = getBlurTypeName(type) + " r=" + str(radius);

    benchmarks.push_back(new deBenchmarkBlur(size, radius, type));
    int r = 0;

    std::string s = str(size.getW()) + "x" + str(size.getH());
    addBenchmark(s, name, r);
}

void deBenchmarkFrame::addBenchmarkBlurs( deSize size, deBlurType type)
{
    addBenchmarkBlur(size, 0.005, type);
    addBenchmarkBlur(size, 0.01, type);
    addBenchmarkBlur(size, 0.05, type);
    addBenchmarkBlur(size, 0.1, type);
}

void deBenchmarkFrame::addBenchmarkColor(deColorSpace colorSpace)
{
    int size = 10000000;
    std::string s = str(size);
    int r = 0;

    {
        std::string name = "RGB -> " + getColorSpaceName(colorSpace);
        benchmarks.push_back(new deBenchmarkColor(size, deColorSpaceRGB, colorSpace));
        addBenchmark(s, name, r);
    }
    {
        std::string name = getColorSpaceName(colorSpace) + " -> RGB";
        benchmarks.push_back(new deBenchmarkColor(size, colorSpace, deColorSpaceRGB));
        addBenchmark(s, name, r);
    }
}

void deBenchmarkFrame::addBenchmarkColor()
{
    std::vector<deColorSpace> colorSpaces;

    getSupportedColorSpaces(colorSpaces);
    int i;
    for (i = 0; i < colorSpaces.size(); i++)
    {
        deColorSpace colorSpace = colorSpaces[i];
        if (colorSpace != deColorSpaceRGB)
        {
            addBenchmarkColor(colorSpace);
        }
    }
}

deBenchmarkFrame::deBenchmarkFrame(wxWindow *parent, const std::string& type)
:deHelpFrame(parent, "benchmark")
{
    sizer = new wxFlexGridSizer(3, 0, 50);
    SetSizer(sizer);

    if (type == "blur")
    {
        addBenchmarkBlurs(deSize(1024, 768), deBoxBlur);
        addBenchmarkBlurs(deSize(4000, 3000), deBoxBlur);
        addBenchmarkBlurs(deSize(1024, 768), deGaussianBlur);
        addBenchmarkBlurs(deSize(4000, 3000), deGaussianBlur);
    }        
    if (type == "color")
    {
        addBenchmarkColor();
    }        

    Layout();
    Fit();
}

deBenchmarkFrame::~deBenchmarkFrame()
{
    int i;
    for (i = 0; i < benchmarks.size(); i++)
    {
        delete benchmarks[i];
    }
}

void deBenchmarkFrame::perform()
{
    int i;
    Update();
    Refresh();
    for (i = 0; i < benchmarks.size(); i++)
    {
        int r = benchmarks[i]->perform();
        int vv = r / 10;
        deValue vvv = vv / 100.0;

        results[i]->SetLabel(wxString::FromAscii(str(vvv).c_str()));

        Update();
        Refresh();
    }
}

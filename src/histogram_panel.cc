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

#include "histogram_panel.h"
#include "project.h"
#include "color_space.h"
#include "preview.h"
#include "channel.h"
#include <sstream>

BEGIN_EVENT_TABLE(deHistogramPanel, wxPanel)
EVT_PAINT(deHistogramPanel::paintEvent)
END_EVENT_TABLE()

deHistogramPanel::deHistogramPanel(wxWindow* parent, deProject* _project)
:wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(256, 200)), project(_project)
{
    SetBackgroundColour(*wxBLACK);
    project->getGUI().setHistogramPanel(this);
}

deHistogramPanel::~deHistogramPanel()
{
    destroyHistograms();
}

void deHistogramPanel::paintEvent(wxPaintEvent & evt)
{
	wxPaintDC dc(this);
	render(dc);
}

void deHistogramPanel::paint()
{
    wxClientDC dc(this);
	render(dc);
}

void deHistogramPanel::render(wxDC& dc)
{
    int histogramMode = project->getGUI().getHistogramMode();
    if (histogramMode < 0)
    {
        return;
    }

    if (!histogram)
    {
        updateHistograms();
    }

    wxStopWatch sw;

    const dePreview* preview = project->getVisiblePreview();
    if (!preview)
    {
        return;
    }
    int i = histogramMode;
    int max = histogram->getMax();
    deColorSpace colorSpace = preview->getColorSpace();
    {
        wxColour colour = getChannelwxColour(colorSpace, i);
        wxPen* pen = new wxPen(colour);
        dc.SetPen(*pen);
        renderHistogram(histogram, dc, max);
    }

    long t = sw.Time();

    std::ostringstream oss;
    oss << "rh: " << t << std::endl;
    project->getGUI().setInfo(5, oss.str());
}

void deHistogramPanel::renderHistogram(deHistogram* histogram, wxDC& dc, int max)
{
    int xx;
    int yy;
    GetSize(&xx, &yy);

    int n = histogram->getSize();
    if (max <=0)
    {
        return;
    }
    int i;
    int height = yy;
    for (i = 0; i < n; i++)
    {
        float v = histogram->get(i);
        int h = height * (1 - v / max);
        dc.DrawLine(i, height, i, h);
    }
}



void deHistogramPanel::updateHistograms()
{
    wxStopWatch sw;

    destroyHistograms();    

    int histogramMode = project->getGUI().getHistogramMode();
    if (histogramMode < 0)
    {
        return;
    }

    const dePreview* preview = project->getVisiblePreview();
    if (!preview)
    {
        return;
    }

    const deSize& size = preview->getSize();
    int n = size.getN();

    int xx;
    int yy;
    GetSize(&xx, &yy);

    int bars = xx - 1 ;

    histogram = new deHistogram(bars);
    const deBaseChannel* channel = preview->getChannel(histogramMode);
    histogram->calc(channel, n);

    long t = sw.Time();

    std::ostringstream oss;
    oss << "ch: " << t << std::endl;
    project->getGUI().setInfo(4, oss.str());
}

void deHistogramPanel::destroyHistograms()
{
    if (histogram)
    {
        delete histogram;
        histogram = NULL;
    }
}

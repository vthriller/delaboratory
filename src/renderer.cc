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

#include "renderer.h"
#include <wx/wx.h>
#include "project.h"
#include "preview.h"
#include "layer.h"
#include "channels.h"
#include "channel.h"
#include "convert_color.h"
#include "converter.h"
#include "color_space.h"
#include <sstream>

deRenderer::deRenderer(deProject* _project)
:project(_project), size(0,0)
{
    project->getGUI().setRenderer(this);
    image = new wxImage(0,0);
#ifdef DE_PROFILER
    renders = 0;
#endif    
}

deRenderer::~deRenderer()
{
    delete image;
}

bool deRenderer::render(wxDC& dc)
{
#ifdef DE_PROFILER
    wxStopWatch sw;
#endif

    const dePreview* preview = project->getVisiblePreview();

    if (!preview)
    {
        return false;
    }
    const deSize& s = preview->getSize();

    int w = s.getW();
    int h = s.getH();
    if (s != size)
    {
        delete image;
        image = new wxImage(w,h);
        size = s;
    }

    unsigned char* data = image->GetData();

    deConverter converter;
    converter.setSource(preview);
    converter.setData(data);

    if (viewMode == deViewSingleChannel)
    {
        converter.storeSingleChannel(viewChannel);
    }
    else
    {
        converter.storeAllChannels();
    }

    wxBitmap bitmap(*image);
    dc.DrawBitmap(bitmap, 0, 0, false);

#ifdef DE_PROFILER
    long t = sw.Time();

    std::ostringstream oss;
    oss << "r: " << t << std::endl;
    project->getGUI().setInfo(1, oss.str());

    renders++;
    oss.str("");
    oss << renders << " pr" << std::endl;
    project->getGUI().setInfo(3, oss.str());
#endif

    return true;
}

void deRenderer::setViewMode(const deViewMode& mode, int channel)
{
    viewMode = mode;
    viewChannel = channel;
}


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

#ifndef _DE_HISTOGRAM_PANEL_H
#define _DE_HISTOGRAM_PANEL_H

#include <wx/wx.h>
#include "histogram.h"
class deProject;
#include "rendered_image.h"
#include "mutex.h"

class deHistogramPanel:public wxPanel
{
    private:
        deProject* project;
        deHistogram histogram;
        deRenderedImage renderedImage;
        int channel;
        bool generated;
        int width;
        int margin;
        deMutex mutex;

        void paintEvent(wxPaintEvent & evt);
        void render(deCanvas& canvas);

        DECLARE_EVENT_TABLE()

    public:
        deHistogramPanel(wxWindow* parent, deProject* _project, int _width, int _margin);
        virtual ~deHistogramPanel();

        void generateHistogram();
        void paintHistogram();

        void setChannel(int _channel);

        int getChannel() const;
};

#endif

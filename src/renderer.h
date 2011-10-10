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

#ifndef _DE_RENDERER_H
#define _DE_RENDERER_H

class deProject;
class wxDC;
class wxImage;
#include "size.h"

/*
class dePreview;
#include "view_mode.h"
*/

class deRenderer
{
    private:
        wxImage* image;
        deProject* project;
        deSize size;
    /*
        deViewMode viewMode;
        int viewChannel;
#ifdef DE_PROFILER        
        int renders;
#endif        
*/
    public:
        deRenderer(deProject* _project);
        virtual ~deRenderer();

        bool render(wxDC& dc);

/*
        void setViewMode(const deViewMode& mode, int channel);
        */
};

#endif

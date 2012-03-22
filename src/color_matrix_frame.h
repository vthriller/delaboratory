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

#ifndef _DE_COLOR_MARTIX_FRAME6_H
#define _DE_COLOR_MATRIX_FRAME6_H

#include "help_frame.h"
#include "palette.h"
class deProject;

class deColorMatrixFrame:public deHelpFrame
{
    private:
        deProject& project;
        dePalette3* palette;
        dePalette3* palette2;
    public:
        deColorMatrixFrame(wxWindow *parent, deProject& _project);
        virtual ~deColorMatrixFrame();
};

#endif

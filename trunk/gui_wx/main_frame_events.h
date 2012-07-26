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

#ifndef _DE_MAIN_FRAME_EVENTS_H
#define _DE_MAIN_FRAME_EVENTS_H

enum
{
    ID_Quit = 1,
    ID_NewProject,
    ID_TestImageSmall,
    ID_TestImageBig,
    ID_OpenImage,
    ID_OpenRawImageProPhoto,
    ID_HelpColorSpaces,
    ID_HelpColorSpaces2,
    ID_HelpColorSpaces3,
    ID_HelpColorSpaces4,
    ID_HelpColorSpaces5,
    ID_LABColors1,
    ID_LABColors2,
    ID_LABColors5,
    ID_ColorMatrix1,
    ID_ColorMatrix2,
    ID_ColorMatrix3,
    ID_ColorMatrix4,
    ID_ColorMatrix5,
    ID_ExportGIMP,
    ID_ExportTIFF,
    ID_ExportJPG,
    ID_ExportAll,
    DE_REPAINT_EVENT,
    DE_IMAGE_LOAD_EVENT,
    DE_HISTOGRAM_EVENT,
    DE_WARNING_EVENT,
    DE_INFO_EVENT
};

#endif

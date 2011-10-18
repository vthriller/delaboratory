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

#ifndef _DE_FILE_DIALOGS_H
#define _DE_FILE_DIALOGS_H

#include <string>
#include <wx/wx.h>

std::string getSaveFile(wxWindow* parent, const std::string& info, const std::string& t);
std::string getOpenFile(wxWindow* parent, const std::string& info, const std::string& t);

#endif

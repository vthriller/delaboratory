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

#ifndef _DE_STR_WX_H
#define _DE_STR_WX_H

#include <string>
#include "value.h"
#include <wx/wx.h>

/*
std::string getBaseName(const std::string& s);

std::string getExtension(const std::string& s);

std::string removePathAndExtension(const std::string& fileName);

std::string str(deValue n);
std::string str(int n);
std::string str(long n);
std::string str(unsigned int n);
std::string str(bool b);

deValue getValue(const std::string& s);
int getInt(const std::string& s);
bool getBool(const std::string& s);

std::string getTmp();

std::string insertIndex(const std::string& s, int index);

std::string getUserConfigDir();

*/

wxString str2wx(const std::string& s);
std::string str(wxString& ws);

#endif

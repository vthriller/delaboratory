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

#include "str.h"

std::string getBaseName(const std::string& s)
{
    int pos = s.rfind(".");    
    return s.substr(0, pos);
}

std::string getExtension(const std::string& s)
{
    int pos = s.rfind(".");    
    return s.substr(pos + 1, s.size() - pos - 1);
}

/*
std::string replaceExtension(const std::string& s, const std::string& ext)
{
    return getBaseName(s) + "." + ext;
}
*/

std::string removePathAndExtension(const std::string& fileName)
{
    size_t posDot = fileName.rfind(".");
    size_t posSlash = fileName.rfind("/");
    int posStart;
    if (posSlash > fileName.size())
    {
        posStart= 0;
    }
    else
    {
        posStart = posSlash + 1;
    }
    return fileName.substr(posStart, posDot - posStart );
}

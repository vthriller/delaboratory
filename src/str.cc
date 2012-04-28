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
#include <sstream>
#include <wx/stdpaths.h>

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

std::string removePathAndExtension(const std::string& fileName)
{
    size_t posDot = fileName.rfind(".");
#ifdef _WIN32
    size_t posSlash = fileName.rfind("\\");
#else
    size_t posSlash = fileName.rfind("/");
#endif    
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

std::string str(int n)
{
    std::ostringstream oss;
    oss << n;
    return oss.str();
}    

std::string str(unsigned int n)
{
    std::ostringstream oss;
    oss << n;
    return oss.str();
}    

std::string str(deValue n)
{
    std::ostringstream oss;
    oss << n;
    return oss.str();
}    

deValue getValue(const std::string& s)
{
    deValue v;
    std::istringstream iss(s);
    iss >> v;
    return v;
}    

int getInt(const std::string& s)
{
    int v;
    std::istringstream iss(s);
    iss >> v;
    return v;
}    

bool getBool(const std::string& s)
{
    if (s == "true")
    {
        return true;
    }
    else
    {
        // TODO some kind of assert of exception in other case?
        return false;
    }
}

std::string str(bool b)
{
    if (b)
    {
        return "true";
    }
    else
    {
        return "false";
    }
}


std::string str(wxString& ws)
{
    char cstring[1024];
    strncpy(cstring, (const char*)ws.mb_str(wxConvUTF8), 1023);

    return cstring;
}    


std::string getTmp()
{
    wxString temp;
    if (wxGetEnv(_T("TEMP"), &temp))
    {
        // on Windows $TEMP should be set
        return str(temp);
    }
    else
    {
        return "/tmp";
    }            
}        

std::string insertIndex(const std::string& s, int index)
{
    std::string b = getBaseName(s);
    std::string e = getExtension(s);

    return b + "_" + str(index) + "." + e;
}

std::string getUserConfigDir()
{
    wxStandardPathsBase& paths = wxStandardPaths::Get();

    wxString ucd = paths.GetUserConfigDir();
    std::string userConfigDir = str(ucd);
#ifdef _WIN32
    userConfigDir += "\\delaboratory";
#else
    userConfigDir += "/.delaboratory";
#endif

    return userConfigDir;
}

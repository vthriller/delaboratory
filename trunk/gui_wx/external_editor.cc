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

#include "external_editor.h"
#include <wx/wx.h>
#include "str.h"
#include "str_wx.h"
#include <stdio.h>
#include <stdlib.h>

#define WINDOWS_GIMP_EXE "gimp-2.8.exe"

void executeExternalEditor(const std::string& fileName, const std::string& app)
{
    const char* c = fileName.c_str();
    wxString s(c, wxConvUTF8);

    std::string executable;

#ifdef _WIN32
	char * editor = getenv( "GR_EDITOR" );
	if( editor != NULL )
	{
        logInfo("using editor variable");
		executable = editor;
	} 
    else
	{
        logInfo("editor variable not found, using GIMP");
		executable = WINDOWS_GIMP_EXE;
	}

#else
    executable = app;
#endif    


    const wxString command = str2wx(executable) + _T(" ") + s;
    wxExecute(command);

}


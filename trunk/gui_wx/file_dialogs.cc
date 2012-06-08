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

#include "file_dialogs.h"
#include "str.h"
#include "str_wx.h"

wxString getFileType(const std::string& t)
{
    if (t == "image")
    {
        return _T("TIFF / JPEG (*.tiff;*.tif;*.jpeg;*.jpg) | ;*.tiff;*.tif;*.jpeg;*.jpg");
    }
    if (t == "raw")
    {
        return _T("RAW (*.*) | ;*.*;");
    }
    if (t == "tiff")
    {
        return _T("TIFF (*.tiff;*.tif) | ;*.tiff;*.tif");
    }
    if (t == "jpeg")
    {
        return _T("JPEG (*.jpeg;*.jpg) | ;*.jpeg;*.jpg");
    }
    if (t == "delab")
    {
        return _T("delab (*.delab) | ;*.delab");
    }

    return _T("");
}


std::string getSaveFile(wxWindow* parent, const std::string& info, const std::string& t)
{
    wxString type = getFileType(t);

    wxFileDialog saveFileDialog(parent, wxString::FromAscii(info.c_str()), _T(""), _T(""), type, wxFD_SAVE);

    if (saveFileDialog.ShowModal() == wxID_CANCEL)
    {
        return "";
    }

    wxString path = saveFileDialog.GetPath();
    return str(path);
}

std::string getOpenFile(wxWindow* parent, const std::string& info, const std::string& t)
{

    wxString type = getFileType(t);

    wxFileDialog openFileDialog(parent, wxString::FromAscii(info.c_str()), _T(""), _T(""), type, wxFD_OPEN);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
    {
        return "";
    }

    wxString path = openFileDialog.GetPath();
    return str(path);
}

std::string getDir(wxWindow* parent, const std::string& info)
{
    wxDirDialog saveFileDialog(parent, wxString::FromAscii(info.c_str()));

    if (saveFileDialog.ShowModal() == wxID_CANCEL)
    {
        return "";
    }

    wxString path = saveFileDialog.GetPath();
    return str(path);
}

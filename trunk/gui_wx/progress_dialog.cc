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

#include "progress_dialog.h"
#include <wx/progdlg.h>

deProgressDialog::deProgressDialog()
{
    progressDialog = new wxProgressDialog(_T("generate final image"), _T("generate final image"), 100, NULL, wxPD_AUTO_HIDE | wxPD_ELAPSED_TIME);
}    

deProgressDialog::~deProgressDialog()
{
    delete progressDialog;
}

void deProgressDialog::update(int progress, const std::string& s)
{
    progressDialog->Update(progress, wxString::FromAscii(s.c_str()));
}        

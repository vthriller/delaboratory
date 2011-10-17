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

#include "wx/wx.h"
#include "project.h"
#include "main_frame.h"
#include "str.h"

class deLaboratory: public wxApp
{	
    public:
        deLaboratory()
        :wxApp()
        {
        }

        ~deLaboratory()
        {
        }

    private:
    	virtual bool OnInit();
        deProject project;
        deMainFrame* frame;

        virtual int FilterEvent(wxEvent& event);

};


IMPLEMENT_APP(deLaboratory)

int deLaboratory::FilterEvent(wxEvent& event)
{
    if (project.shouldReceiveKeys())
    {
        if  (event.GetEventType()==wxEVT_KEY_DOWN )
        {
             project.onKey(((wxKeyEvent&)event).GetKeyCode());
             frame->onKey(((wxKeyEvent&)event).GetKeyCode());
             return true;
        }
    }

    return -1;
}

bool deLaboratory::OnInit()
{
    wxInitAllImageHandlers();

	//int width = 1024;
	//int height = 880;
	int width = 1200;
	int height = 960;

	frame = new deMainFrame( wxSize(width,height), &project);

    if (argc > 1)
    {
        wxString a = argv[1];
        project.init(str(a));
    }

	frame->Show(TRUE);
	SetTopWindow(frame);

	return TRUE;
} 


	

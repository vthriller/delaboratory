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
#include "rgb2xyz2lab.h"

class deThread:public wxThread
{
    private:
        virtual void *Entry()
        {
            bool w = true;
            while (w)
            {
                wxThread::Sleep(1000);
                if (TestDestroy())
                {
                    w = false;
                }
            }
            return NULL;
        }
    public:    
        deThread()
        {
        }
        virtual ~deThread()
        {
        }
};

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
    	virtual int OnExit();
        deProject project;
        deMainFrame* frame;

        deThread* thread;

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


int deLaboratory::OnExit()
{
    thread->Delete();
    return 0;
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

    initLAB();

    thread = new deThread();

    if ( thread->Create() != wxTHREAD_NO_ERROR )
    {
    }

    if ( thread->Run() != wxTHREAD_NO_ERROR )
    {
    }

	return TRUE;
} 


	

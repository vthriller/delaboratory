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
#include "str.h"
#include "rgb2xyz2lab.h"
#include "layer_processor.h"
#include "logger.h"
#include "channel_manager.h"
#include "layer_stack.h"
#include "layer_frame_manager.h"
#include "static_image.h"
#include "raw_module.h"
#include "zoom_manager.h"
#include "operation_processor.h"
#include "main_window.h"

#include <wx/stdpaths.h>

const std::string LOG_FILE_NAME = "debug.log";
const std::string LOG_LOCKS_FILE_NAME = "locks.log";


class deLaboratory: public wxApp
{	
    public:
        deLaboratory()
        :wxApp(),
         sourceImage(),
         processor(previewChannelManager, layerStack, layerFrameManager, mainWindow),
         operationProcessor(processor),
         project(processor, previewChannelManager, layerStack, layerFrameManager, sourceImage, rawModule, zoomManager, operationProcessor, mainWindow)
        {
            deLogger::getLogger().setFile(LOG_FILE_NAME);
            deLogger::getLogger().setLocksFile(LOG_LOCKS_FILE_NAME);

        }

        ~deLaboratory()
        {
        }

    private:
    	virtual bool OnInit();
    	virtual int OnExit();
        deMainWindow mainWindow;
        deSamplerManager samplerManager;
        deLayerStack layerStack;
        deLayerFrameManager layerFrameManager;
        deChannelManager previewChannelManager;
        deStaticImage sourceImage;
        deLayerProcessor processor;
        deRawModule rawModule;
        deZoomManager zoomManager;
        deOperationProcessor operationProcessor;
        deProject project;

        virtual int FilterEvent(wxEvent& event);

};


IMPLEMENT_APP(deLaboratory)

int deLaboratory::FilterEvent(wxEvent& event)
{
    if (project.shouldReceiveKeys())
    {
        if  (event.GetEventType()==wxEVT_KEY_DOWN )
        {
            int key = ((wxKeyEvent&)event).GetKeyCode();
            if (key == WXK_RETURN)
            {
                logMessage("WXK_RETURN detected");
                return true;
            }

            project.onKey(key);
            mainWindow.onKey(key);
            return true;
        }
    }

    return -1;
}


int deLaboratory::OnExit()
{
    project.log("OnExit");
    return 0;
}   

bool deLaboratory::OnInit()
{
    logMessage("deLaboratory::OnInit");

    wxStandardPathsBase& paths = wxStandardPaths::Get();

    wxString configDir = paths.GetConfigDir();
    wxString docsDir = paths.GetDocumentsDir();
    wxString exePath = paths.GetExecutablePath();
    wxString localDataDir = paths.GetLocalDataDir();
    wxString tempDir = paths.GetTempDir();
    wxString userConfigDir = paths.GetUserConfigDir();
    wxString userDataDir = paths.GetUserDataDir();

    logMessage("config dir: " + str(configDir)); 
    logMessage("docs dir: " + str(docsDir)); 
    logMessage("exe path: " + str(exePath)); 
    logMessage("local data dir: " + str(localDataDir)); 
    logMessage("temp dir: " + str(tempDir)); 
    logMessage("user config dir: " + str(userConfigDir)); 
    logMessage("user data dir: " + str(userDataDir)); 


    rawModule.onInit();

    std::string dcraw_version = rawModule.getVersion();

    if (dcraw_version == "")
    {
        dcraw_version = "dcraw not found";
    }
    else
    {
        dcraw_version = "dcraw version " + dcraw_version;
    }

    wxInitAllImageHandlers();

	int width = 1200;
	int height = 960;

	mainWindow.init( width, height, project, processor, samplerManager, zoomManager, dcraw_version, operationProcessor, previewChannelManager);

    logMessage("show main frame");

    mainWindow.show();

    logMessage("set top level");

    mainWindow.setTopWindow();

    logMessage("initLAB");

    initLAB();

    logMessage("startWorkerThread...");

    processor.startWorkerThread();

    logMessage("process argc/argv...");

    if (argc > 1)
    {
        wxString a = argv[1];
        project.init(str(a));
    }

    logMessage("OnInit done");


	return TRUE;
} 


	

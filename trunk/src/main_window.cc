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

#include "main_window.h"
#include "main_frame.h"
#include <wx/wx.h>

class deMainWindowImpl
{
    private:
        deMainFrame* frame;
        wxTimer* rawTimer;

    public:
        deMainWindowImpl()
        {
            frame = NULL;
        }

        ~deMainWindowImpl()
        {
        }
    
        void init(int width, int height, deProject& project, deLayerProcessor& layerProcessor, deSamplerManager& samplerManager, deZoomManager& zoomManager, const std::string& dcrawVersion, deOperationProcessor& operationProcessor)
        {
        	frame = new deMainFrame( wxSize(width,height), project, layerProcessor, samplerManager, zoomManager, dcrawVersion, operationProcessor);

            rawTimer = new wxTimer(frame, wxID_ANY);
        }            

        void show()
        {
            if (frame)
            {
            	frame->Show(TRUE);
            }                
        }            

        void setTopWindow()
        {
            if (frame)
            {
                //SetTopWindow(frame);
            }                
        }            

        void postEvent(int e, int arg)
        {
            if (frame)
            {
                wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, e );
                event.SetInt(arg);
                wxPostEvent( frame, event );
            }
        }

        void generateHistogram()
        {
            if (frame)
            {
                frame->generateHistogram();
            }                
        }            

        void onKey(int key)
        {
            if (frame)
            {
                frame->onKey(key);
            }
        }

        void rebuild()
        {
            if (frame)
            {
                frame->rebuild();
            }
        }

        void setImageName(const std::string& _imageName, const deSize& _size)
        {
            if (frame)
            {
                frame->setImageName(_imageName, _size);
            }
        }

        void startRawTimer()
        {
            rawTimer->Start(500);
        }            

        void stopRawTimer()
        {
            rawTimer->Stop();
        }            


};

deMainWindow::deMainWindow()
{
    impl = new deMainWindowImpl();
}

deMainWindow::~deMainWindow()
{
    delete impl;
}

void deMainWindow::init(int width, int height, deProject& project, deLayerProcessor& layerProcessor, deSamplerManager& samplerManager, deZoomManager& zoomManager, const std::string& dcrawVersion, deOperationProcessor& operationProcessor)
{
    impl->init( width, height, project, layerProcessor, samplerManager, zoomManager, dcrawVersion, operationProcessor);
}            

void deMainWindow::show()
{
    impl->show();
}

void deMainWindow::setTopWindow()
{
    impl->setTopWindow();
}

void deMainWindow::postEvent(int event, int arg)
{
    impl->postEvent(event, arg);
}

void deMainWindow::generateHistogram()
{
    impl->generateHistogram();
}

void deMainWindow::onKey(int key)
{
    impl->onKey(key);
}

void deMainWindow::rebuild()
{
    impl->rebuild();
}

void deMainWindow::setImageName(const std::string& _imageName, const deSize& _size)
{
    impl->setImageName(_imageName, _size);
}

void deMainWindow::startRawTimer()
{
    impl->startRawTimer();
}

void deMainWindow::stopRawTimer()
{
    impl->stopRawTimer();
}

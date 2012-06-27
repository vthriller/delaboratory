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

#ifndef _DE_CENTER_PANEL_H
#define _DE_CENTER_PANEL_H

#include <wx/wx.h>
class deProject;
class deImagePanel;
class deSamplerManager;
class deZoomManager;
class deZoomPanel;
class deGUI;
#include "mutex.h"

class deImageAreaPanel:public wxPanel
{
    private:
        void resize(wxSizeEvent &event);
        deProject& project;

        deImagePanel* imagePanel;

        deMutex sizeMutex;

        bool sizeLocked;

    public:
        deImageAreaPanel(wxWindow* parent, deProject& _project, deSamplerManager& _samplerManager, deZoomManager& _zoomManager, deZoomPanel* zoomPanel, deGUI& gui);
        virtual ~deImageAreaPanel();
        
        void updateSize(bool canSkip);

        void lockSize();
        void unlockSize();

        deImagePanel* getImagePanel();

};

#endif

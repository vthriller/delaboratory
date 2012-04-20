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

#ifndef _DE_MAIN_WINDOW_H
#define _DE_MAIN_WINDOW_H

class deMainWindowImpl;
class deProject;
class deLayerProcessor;
class deSamplerManager;
class deZoomManager;
class deOperationProcessor;
class deSize;
class deChannelManager;
#include <string>

class deMainWindow
{
    private:
        deMainWindowImpl* impl;

    public:
        deMainWindow();
        virtual ~deMainWindow();

        void init(int width, int height, deProject& project, deLayerProcessor& layerProcessor, deSamplerManager& samplerManager, deZoomManager& zoomManager, const std::string& dcrawVersion, deOperationProcessor& operationProcessor, deChannelManager& channelManager);
        void show();
        void setTopWindow();
        void postEvent(int event, int arg);
        void generateHistogram();
        void onKey(int key);
        void rebuild();
        void setImageName(const std::string& _imageName, const deSize& _size);
        void startRawTimer();
        void stopRawTimer();

};

#endif

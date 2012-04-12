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

#ifndef _DE_PREVIEW_MANAGER_H
#define _DE_PREVIEW_MANAGER_H

class deProject;
class deLayerProcessor;
class deZoomManager;
#include "color_space.h"

class deViewManager
{
    private:
        int view;
        deProject& project;
        deLayerProcessor& layerProcessor;
        deZoomManager& zoomManager;
        bool single;
        int channel;

    public:
        deViewManager(deProject& _project, deLayerProcessor& _processor, deZoomManager& _zoomManager);
        virtual ~deViewManager();

        void setView(int v);
        int getView() const;

        void setSingleChannel(int _channel);
        void setNormal();

        deColorSpace getColorSpace() const;

        bool isSingleChannel() const {return single;};
        int getChannel() const {return channel;};

        void setHistogramChannel(int channel);

        deValue getRealScale() const;

        void getZoom(deValue& _x1, deValue& _y1, deValue& _x2, deValue& _y2);

};

#endif

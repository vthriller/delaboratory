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

#ifndef _DE_RENDERER_H
#define _DE_RENDERER_H

class wxDC;
class wxImage;
class deChannelManager;
class deViewManager;
class deLayerProcessor;
class deLayerStack;
#include <wx/wx.h>
#include "size.h"

class deRenderer
{
    private:
        wxImage* image;
        deSize size;
        deChannelManager& channelManager;
        wxMutex mutex;

        unsigned char* getCurrentImageData();

    public:
        deRenderer(deChannelManager& _channelManager);
        virtual ~deRenderer();

        bool render(wxDC& dc);
        bool prepareImage(const deViewManager& viewManager, deLayerProcessor& layerProcessor, deLayerStack& layerStack);
};

#endif

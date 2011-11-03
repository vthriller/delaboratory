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

#ifndef _DE_LAYER_PROCESSOR_H
#define _DE_LAYER_PROCESSOR_H

class deMainFrame;
class deLayerStack;
class deViewManager;
class deChannelManager;
class wxProgressDialog;
class deMemoryInfoFrame;
#include <map>

class deLayerProcessor
{
    private:
        deMainFrame* mainFrame;
        deLayerStack* stack;
        deViewManager* viewManager;

    public:
        deLayerProcessor();
        virtual ~deLayerProcessor();

        void setMainFrame(deMainFrame* _mainFrame);
        void setLayerStack(deLayerStack* _layerStack);
        void setViewManager(deViewManager* _viewManager);

        void repaintImage(bool calcHistogram);
        void updateAllImages(bool calcHistogram);
        void updateImages(int a, int b);
        void updateImagesSmart(deChannelManager& channelManager, int view, wxProgressDialog* progressDialog, deMemoryInfoFrame* memoryInfoFrame);
        void generateChannelUsage(std::map<int, int>& channelUsage);

};

#endif

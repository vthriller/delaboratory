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

#ifndef _DE_LAYER_STACK_H
#define _DE_LAYER_STACK_H

#include <vector>
#include <map>
#include <libxml/parser.h>
#include <wx/progdlg.h>

class deLayer;
class deChannelManager;
class deMemoryInfoFrame;

class deLayerStack
{
    private:
        std::vector<deLayer*> layers;

    public:
        deLayerStack();
        virtual ~deLayerStack();

        void clear();
        void removeTopLayer();

        void addLayer(deLayer* layer);

        int getSize() const;
        deLayer* getLayer(int id) const;

        void updateAllImages();
        void updateImages(int a, int b);

        void updateImagesSmart(deChannelManager& channelManager, int view, wxProgressDialog* progressDialog, deMemoryInfoFrame* memoryInfoFrame);

        void generateChannelUsage(std::map<int, int>& channelUsage);

        void onKey(int key);

        void save(xmlNodePtr node);

};

#endif

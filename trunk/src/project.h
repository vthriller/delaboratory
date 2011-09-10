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

#ifndef _DE_PROJECT_H
#define _DE_PROJECT_H

#include "size.h"
#include "layer_stack.h"
#include "sampler_list.h"
#include "preview_stack.h"
#include "source_image.h"
#include <string>
#include "gui.h"
#include "logger.h"
#include "layer_factory.h"
#include "channel_manager.h"

class deFinalImage;

class deProject
{
    private:
        deSize sourceImageSize;
        deSize previewSize;
        deLayerStack layerStack;
        dePreviewStack previewStack;
        deSamplerList samplerList;
        int view;
        deSourceImage sourceImage;
        deGUI gui;
        std::string sourceFileName;
        deLogger logger;
        deLayerFactory layerFactory;

        std::string sourceImageFileName;

        deChannelManager channelManager;

        deProject(const deProject& project);

    public:
        deProject();
        virtual ~deProject();

        void setSourceImageSize(const deSize& size);
        const deSize& getSourceImageSize() const;

        void setPreviewSize(const deSize& size);
        const deSize& getPreviewSize() const;

        void addLayer(deLayer* layer);
        deLayer* getVisibleLayer();
        dePreview* getVisiblePreview();

        void removeTopLayer();

        const deLayerStack& getLayerStack() const;
        deLayerStack& getLayerStack();
        
        const dePreviewStack& getPreviewStack() const;
        dePreviewStack& getPreviewStack();

        int getVisibleLayerID() const;

        deFinalImage* generateFinalImage();

        void setView(int v);

        deSamplerList& getSamplerList();

        void start();

        void init(const std::string& fileName);
        void loadSourceImage(const std::string& fileName);

        deGUI& getGUI();

        const std::string getSourceFileName() const;

        void logMessage(const std::string& message);

        deLayerFactory& getLayerFactory() {return layerFactory;};

        void open(const std::string& fileName, bool image);
        void save(const std::string& fileName);

        void resetLayerStack();

        void onKey(int key);

};

#endif

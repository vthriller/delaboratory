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

#include "flatten_layers.h"

#include "progress_dialog.h"
#include "logger.h"
#include "layer_stack.h"
#include "base_layer.h"
#include "channel_manager.h"
#include "image_io.h"
#include "str.h"
#include <map>

bool flattenLayers(int view, deProgressDialog& progressDialog, const std::string& fileName, const std::string& type, bool saveAll, deLayerStack& layerStack, deChannelManager& previewChannelManager)
{
    bool result = true;
    logInfo("flattenLayers");

    std::map<int, int> channelUsage;
    int i;
    int n = layerStack.getSize();
    for (i = 0; i < n; i++)
    {
        const deBaseLayer* layer = layerStack.startReadLayer(i);

        layer->updateChannelUsage(channelUsage, i);

        layerStack.finishReadLayer(i);
    }

    unsigned int index;
    int progress = 0;
    for (index = 0; index <= (unsigned int)view; index++)
    {
        logInfo("flattenLayers index: " + str(index));
        std::map<int, int>::iterator i;
        int previous = index - 1;
        if (previous >= 0)
        {
            for (i = channelUsage.begin(); i != channelUsage.end(); i++)
            {
                int c = i->first;
                int l = i->second;
                if (l == previous)
                {
                    logInfo("deallocate " + str(c));
                    previewChannelManager.tryDeallocateChannel(c);
                }
            }
        }

        deBaseLayer* layer = layerStack.getLayer(index);

        std::string label = str(index);

        progressDialog.update(progress, label);

        layer->allocateChannels();

        logInfo("flattenLayers process index: " + str(index));
        bool r = layer->processFull();
        if (r)
        {
            if (saveAll)
            {
                const deImage& image = layer->getLayerImage();
                const std::string f = insertIndex(fileName, index);
                saveImage(f, image, type, previewChannelManager);
            }                
        }
        else
        {
            logError("flattenLayers - error on layer " + str(index));
            result = false;
            // stop loop
            index = view + 1;
        }

        if (view > 0)
        {
            progress = 100 * index / view;
        }
        else
        {
            progress = 100;
        }

        progressDialog.update(progress, label);
    }

    progressDialog.update(100, "finished");

    if ((result) && (!saveAll))
    {
        logInfo("flattenLayers save final image");
        // take the final image
        deBaseLayer* layer = layerStack.getLayer(view);
        const deImage& image = layer->getLayerImage();

        // save it
        result = saveImage(fileName, image, type, previewChannelManager);
    }

    logInfo("flattenLayers DONE");

    return result;
}


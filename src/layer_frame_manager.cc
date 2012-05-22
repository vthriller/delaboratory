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

#include "layer_frame_manager.h"
#include "layer_frame.h"
#include "str.h"
#include "logger.h"

deLayerFrameManager::deLayerFrameManager()
{
}

deLayerFrameManager::~deLayerFrameManager()
{
    destroyAllFrames();
}

void deLayerFrameManager::destroyAllFrames()
{
    while (layerFrames.size() > 0)
    {
        std::map<int, deLayerFrame*>::iterator i = layerFrames.begin();
        layerFrames.erase(i);
        delete i->second;
    }
}


bool deLayerFrameManager::addLayerFrame(int index, deLayerFrame* frame)
{
    std::map<int, deLayerFrame*>::iterator i = layerFrames.find(index);
    if (i != layerFrames.end())
    {
        return false;
    }
    layerFrames[index] = frame;
    return true;
}

bool deLayerFrameManager::removeLayerFrame(int index)
{
    logInfo("remove layer frame " + str(index));
    std::map<int, deLayerFrame*>::iterator i = layerFrames.find(index);
    if (i == layerFrames.end())
    {
        return false;
    }
    layerFrames.erase(i);
    logInfo("remove layer frame " + str(index) + " DONE");
    return true;
}

bool deLayerFrameManager::destroyLayerFrame(int index)
{
    logInfo("destroy layer frame " + str(index));
    std::map<int, deLayerFrame*>::iterator i = layerFrames.find(index);
    if (i == layerFrames.end())
    {
        return false;
    }
    deLayerFrame* frame = i->second;
    logInfo("b destroy layer frame " + str(index));
    if (frame)
    {
        delete frame;
    }
    else
    {
        logError("broken layer frame detected!");
    }
    logInfo("c destroy layer frame " + str(index));
    return true;
}

void deLayerFrameManager::onDestroyLayer(int index)
{
    logInfo("c onDestroyLayer " + str(index));
    destroyLayerFrame(index);
    logInfo("onDestroyLayer " + str(index) + " DONE");
}

bool deLayerFrameManager::onImageClick(deValue x, deValue y)
{
    {
        std::map<int, deLayerFrame*>::iterator i = layerFrames.begin();
        for (i = layerFrames.begin(); i != layerFrames.end(); i++)
        {
            deLayerFrame* layerFrame = i->second;
            if (layerFrame)
            {
                bool result = layerFrame->onImageClick(x, y);
                if (result)
                {
                    return result;
                }
            }            
        }
    }        
    return false;
}

void deLayerFrameManager::onKey(int key)
{
}

void deLayerFrameManager::onUpdateProperties()
{
}

bool deLayerFrameManager::checkLayerFrame(int index)
{
    std::map<int, deLayerFrame*>::iterator i = layerFrames.find(index);
    if (i != layerFrames.end())
    {
        return true;
    }
    return false;
}


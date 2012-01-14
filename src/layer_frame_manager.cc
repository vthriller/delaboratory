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

deLayerFrameManager::deLayerFrameManager()
{
}

deLayerFrameManager::~deLayerFrameManager()
{
    destroyAllFrames();
}

void deLayerFrameManager::destroyAllFrames()
{
    while (actionFrames.size() > 0)
    {
        std::list<deLayerFrame*>::iterator i = actionFrames.begin();
        actionFrames.remove(*i);
        delete *i;
    }
    while (blendFrames.size() > 0)
    {
        std::list<deLayerFrame*>::iterator i = blendFrames.begin();
        blendFrames.remove(*i);
        delete *i;
    }
}

void deLayerFrameManager::addActionFrame(deFrame* frame)
{
    deLayerFrame* lf = dynamic_cast<deLayerFrame*>(frame);
    if (lf)
    {
        actionFrames.push_back(lf);
    }
}

void deLayerFrameManager::removeActionFrame(deFrame* frame)
{
    deLayerFrame* lf = dynamic_cast<deLayerFrame*>(frame);
    if (lf)
    {
        actionFrames.remove(lf);
    }
}

bool deLayerFrameManager::checkActionFrame(int index)
{
    std::list<deLayerFrame*>::const_iterator i;
    for (i = actionFrames.begin(); i != actionFrames.end(); i++)
    {
        if ((*i)->checkIndex(index))
        {
            return true;
        }
    }
    return false;
}

void deLayerFrameManager::addBlendFrame(deFrame* frame)
{
    deLayerFrame* lf = dynamic_cast<deLayerFrame*>(frame);
    if (lf)
    {
        blendFrames.push_back(lf);
    }
}

void deLayerFrameManager::removeBlendFrame(deFrame* frame)
{
    deLayerFrame* lf = dynamic_cast<deLayerFrame*>(frame);
    if (lf)
    {
        blendFrames.remove(lf);
    }
}

bool deLayerFrameManager::checkBlendFrame(int index)
{
    std::list<deLayerFrame*>::const_iterator i;
    for (i = blendFrames.begin(); i != blendFrames.end(); i++)
    {
        if ((*i)->checkIndex(index))
        {
            return true;
        }
    }
    return false;
}

void deLayerFrameManager::onDestroyLayer(int index)
{
    destroyActionFrame(index);
    destroyBlendFrame(index);
}

void deLayerFrameManager::destroyActionFrame(int index)
{
    std::list<deLayerFrame*>::const_iterator i;
    for (i = actionFrames.begin(); i != actionFrames.end(); i++)
    {
        if ((*i)->checkIndex(index))
        {
            actionFrames.remove(*i);
            delete *i;
            return;
        }
    }
}

void deLayerFrameManager::destroyBlendFrame(int index)
{
    std::list<deLayerFrame*>::const_iterator i;
    for (i = blendFrames.begin(); i != blendFrames.end(); i++)
    {
        if ((*i)->checkIndex(index))
        {
            blendFrames.remove(*i);
            delete *i;
            return;
        }
    }
}

bool deLayerFrameManager::onImageClick(deValue x, deValue y)
{
    std::list<deLayerFrame*>::const_iterator i;
    for (i = actionFrames.begin(); i != actionFrames.end(); i++)
    {
        deActionFrame* trueActionFrame = dynamic_cast<deActionFrame*>(*i);
        if (trueActionFrame)
        {
            trueActionFrame->onImageClick(x, y);
            return true;    
        }            
    }
    return false;
}

void deLayerFrameManager::onKey(int key)
{
    std::list<deLayerFrame*>::const_iterator i;
    for (i = actionFrames.begin(); i != actionFrames.end(); i++)
    {
        deActionFrame* trueActionFrame = dynamic_cast<deActionFrame*>(*i);
        if (trueActionFrame)
        {
            trueActionFrame->onKey(key);
        }            
    }
}


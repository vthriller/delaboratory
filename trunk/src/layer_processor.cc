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

#include "layer_processor.h"
#include "main_frame.h"
#include "layer_stack.h"
#include "view_manager.h"
#include <string>
#include "layer.h"
#include "channel_manager.h"
#include "str.h"
#include "memory_info_frame.h"

deLayerProcessor::deLayerProcessor()
{
    mainFrame = NULL;
    stack = NULL;
    viewManager = NULL;
}

deLayerProcessor::~deLayerProcessor()
{
}

void deLayerProcessor::setMainFrame(deMainFrame* _mainFrame)
{
    mainFrame = _mainFrame;
}

void deLayerProcessor::setLayerStack(deLayerStack* _layerStack)
{
    stack = _layerStack;
}

void deLayerProcessor::setViewManager(deViewManager* _viewManager)
{
    viewManager = _viewManager;
}

void deLayerProcessor::repaintImage(bool calcHistogram)
{
    if (mainFrame)
    {
        mainFrame->repaint(calcHistogram);
    }
}

void deLayerProcessor::updateAllImages(bool calcHistogram)
{
    if (stack)
    {
        int view = viewManager->getView();
        updateImages(0, view);
    }        
    repaintImage(calcHistogram);
}    

void deLayerProcessor::updateImages(int a, int b)
{
    unsigned int i;
    assert((unsigned int)b < stack->getSize() );
    for (i = (unsigned int)a; i <= (unsigned int)b; i++)
    {
        deLayer* layer = stack->getLayer(i);
        if (layer)
        {
            layer->updateImage();
        }            
    }
}

void deLayerProcessor::updateImagesSmart(deChannelManager& channelManager, int view, wxProgressDialog* progressDialog, deMemoryInfoFrame* memoryInfoFrame)
{
    channelManager.lock();

    std::map<int, int> channelUsage;
    generateChannelUsage(channelUsage);

    unsigned int index;
    int progress = 0;
    assert((unsigned int)view < layers.size());
    for (index = 0; index <= (unsigned int)view; index++)
    {
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
                    channelManager.tryDeallocateChannel(c);
                }
            }
        }

        deLayer* layer = stack->getLayer(index);
        std::string label = str(index) + " " + layer->getName();

        progressDialog->Update(progress, wxString::FromAscii(label.c_str()));

        layer->updateImage();

        if (memoryInfoFrame)
        {
            memoryInfoFrame->update();
        }

        if (view > 0)
        {
            progress = 100 * index / view;
        }
        else
        {
            progress = 100;
        }

        progressDialog->Update(progress, wxString::FromAscii(label.c_str()));
    }

    progressDialog->Update(100, _T("finished"));

    channelManager.unlock();
}

void deLayerProcessor::generateChannelUsage(std::map<int, int>& channelUsage)
{
    channelUsage.clear();
    int i;
    int n = stack->getSize();
    for (i = 0; i < n; i++)
    {
        deLayer* layer = stack->getLayer(i);
        layer->updateChannelUsage(channelUsage);
    }
}


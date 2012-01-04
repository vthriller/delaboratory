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
#include <wx/progdlg.h>
#include <iostream>
#include "action_layer.h"

static wxMutex updateImagesMutex;

#define LAYER_PROCESSING_ON_THREAD 1

class deUpdateImagesThread:public wxThread
{
    private:
        virtual void *Entry()
        {
            layerProcessor.updateImagesThreadCall(a, b, channel, blend, action);
            return NULL;
        }
        deLayerProcessor& layerProcessor;
        int a;
        int b;
        int channel;
        bool blend;
        bool action;
    public:    
        deUpdateImagesThread(deLayerProcessor& _layerProcessor, int _a, int _b, int _channel, bool _blend, bool _action)
        :layerProcessor(_layerProcessor),
         a(_a),
         b(_b),
         channel(_channel),
         blend(_blend),
         action(_action)
        {

        }
        virtual ~deUpdateImagesThread()
        {
        }
};


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

void deLayerProcessor::repaintImageInLayerProcessor(bool calcHistogram)
{
    updateImagesMutex.Lock();
    if (mainFrame)
    {
        wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, DE_REPAINT_EVENT );
        wxPostEvent( mainFrame, event );
    }
    updateImagesMutex.Unlock();
}

void deLayerProcessor::updateAllImages(bool calcHistogram)
{
    if (stack)
    {
        int view = viewManager->getView();
        updateImages(0, view, -1, false, true);
    }        
}    

void deLayerProcessor::updateImages(int a, int b, int channel, bool blend, bool action)
{
#if LAYER_PROCESSING_ON_THREAD 
    deUpdateImagesThread* thread = new deUpdateImagesThread(*this, a, b, channel, blend, action);

    if ( thread->Create() != wxTHREAD_NO_ERROR )
    {
        std::cout << "creating thread... CREATE ERROR" << std::endl;
    }

    if ( thread->Run() != wxTHREAD_NO_ERROR )
    {
        std::cout << "creating thread... RUN ERROR" << std::endl;
    }
#else
    updateImagesThreadCall(a, b, channel, blend, action);
#endif

}    

void deLayerProcessor::updateImagesThreadCall(int a, int b, int channel, bool blend, bool action)
{
    if (!stack)
    {
        return;
    }

    updateImagesMutex.Lock();
    stack->lock();

    unsigned int i;
    assert((unsigned int)b < stack->getSize() );
    for (i = (unsigned int)a; i <= (unsigned int)b; i++)
    {
        deLayer* layer = stack->getLayer(i);
        if (layer)
        {
            bool nextAction = false;

            if (blend)
            {
                deActionLayer* alayer = dynamic_cast<deActionLayer*>(layer);
                alayer->updateImageInActionLayer(false, true, channel);
                blend = false;
                nextAction = true;
            }                    

            if (action)
            {
                if (channel >= 0)
                {
                    deActionLayer* alayer = dynamic_cast<deActionLayer*>(layer);
                    alayer->updateImageInActionLayer(action, true, channel);
                    channel = -1;
                }
                else
                {
                    layer->updateImageThreadCall();
                }
            }

            if (nextAction)
            {
                action = true;
            }
        }            
    }

    stack->unlock();
    updateImagesMutex.Unlock();

    repaintImageInLayerProcessor(true);

}

void deLayerProcessor::updateImagesSmart(deChannelManager& channelManager, int view, wxProgressDialog* progressDialog, deMemoryInfoFrame* memoryInfoFrame)
{
    updateImagesMutex.Lock();

//    channelManager.lock();
    stack->lock();

    std::map<int, int> channelUsage;
    generateChannelUsage(channelUsage);

    unsigned int index;
    int progress = 0;
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

        layer->updateImageThreadCall();

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

    stack->unlock();
//    channelManager.unlock();
    updateImagesMutex.Unlock();
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

void deLayerProcessor::markUpdateSingleChannel(int index, int channel)
{
    if (viewManager)
    {
        updateImages(index, viewManager->getView(), channel, false, true);
    }        
}

void deLayerProcessor::markUpdateAllChannels(int index)
{
    if (stack)
    {
        deLayer* layer = stack->getLayer(index);
        if (layer)
        {
            layer->onUpdateProperties();
        }        
    }        

    if (viewManager)
    {
        updateImages(index, viewManager->getView(), -1, false, true);
    }
}

void deLayerProcessor::markUpdateBlendAllChannels(int index)
{
    if (viewManager)
    {
        updateImages(index, viewManager->getView(), -1, true, false);
    }
}

void deLayerProcessor::onChangeView(int a, int b)
{
    updateImages(a + 1, b, -1, false, true);
}   

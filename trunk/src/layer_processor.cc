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
#include "logger.h"

static wxMutex updateImagesMutex(wxMUTEX_RECURSIVE);

class deLayerProcessorWorkerThread:public wxThread
{
    private:
        void performTasks()
        {
            while (true)
            {
                processor.log("worker thread before wait");
                semaphore.Wait();
                processor.log("worker thread after wait");

                if (TestDestroy())
                {
                    return;
                }

                processor.tickWork();

                if (TestDestroy())
                {
                    return;
                }
            }
        }

        virtual void *Entry()
        {
            performTasks();
            processor.log("worker thread finished");
            return NULL;
        }
        deLayerProcessor& processor;
        wxSemaphore& semaphore;

    public:    
        deLayerProcessorWorkerThread(deLayerProcessor& _processor, wxSemaphore& _semaphore)
        :processor(_processor),
         semaphore(_semaphore)
        {
        }
        virtual ~deLayerProcessorWorkerThread()
        {
        }
};


deLayerProcessor::deLayerProcessor()
{
    mainFrame = NULL;
    stack = NULL;
    viewManager = NULL;

    logger = NULL;

    workerThread = NULL;

    firstLayerToUpdate = 0;
    lastValidLayer = -1;

    multithreadingEnabled = true;
    //multithreadingEnabled = false;

    closing = false;

}

void deLayerProcessor::onDestroyAll()
{
    lock();

    lastValidLayer = -1;

    unlock();
}

deLayerProcessor::~deLayerProcessor()
{
    log("destroying layer processor");
    stopWorkerThread();
    lock();
    unlock();
}

void deLayerProcessor::setMainFrame(deMainFrame* _mainFrame)
{
    mainFrame = _mainFrame;
}

void deLayerProcessor::stopWorkerThread()
{
    log("stop worker thread");
    closing = true;
    workerThread->Delete();
}

void deLayerProcessor::startWorkerThread()
{
    workerThread = new deLayerProcessorWorkerThread(*this, workerSemaphore);

    if ( workerThread->Create() != wxTHREAD_NO_ERROR )
    {
    }

    if ( workerThread->Run() != wxTHREAD_NO_ERROR )
    {
    }
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
    if (closing)
    {
        return;
    }

    if (mainFrame)
    {
        wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, DE_REPAINT_EVENT );
        wxPostEvent( mainFrame, event );
    }
}

void deLayerProcessor::updateAllImages(bool calcHistogram)
{
    if (stack)
    {
        updateImages(0, -1, false, true);
    }        
}    

void deLayerProcessor::updateImages(int a, int channel, bool blend, bool action)
{
    if (closing)
    {
        return;
    }

    log("requested update images " + str(a) + " " + str(channel) + " " + str(blend) + " " + str(action));

    if (isMultithreadingEnabled())
    {
        lock();

        if (a < firstLayerToUpdate)
        {
            firstLayerToUpdate = a;
        }

        channelUpdate = channel;
        blendUpdate = blend;
        actionUpdate = action;

        checkUpdateImagesRequest();

        unlock();
    }
    else
    {
        updateImagesThreadCall(a, channel, blend, action);
    }        
}    

void deLayerProcessor::updateImage(int i, int& channel, bool& blend, bool& action)
{
    log("update image " + str(i) + " " + str(channel) + " " + str(blend) + " " + str(action));

    if (i >= firstLayerToUpdate) 
    {
        firstLayerToUpdate = i+1;
    }

    deLayer* layer = stack->getLayer(i);
    if (layer)
    {
        bool nextAction = false;

        if (blend)
        {
            deActionLayer* alayer = dynamic_cast<deActionLayer*>(layer);
            if (alayer)
            {
                alayer->updateImageInActionLayer(false, true, channel);
                blend = false;
                nextAction = true;
            }                
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

    lastValidLayer = i;
}

void deLayerProcessor::updateImagesThreadCall(int a, int channel, bool blend, bool action)
{
    if (!stack)
    {
        return;
    }

    lock();

    unsigned int i;
    int b = getLastLayerToUpdate();
    assert((unsigned int)b < stack->getSize() );
    for (i = (unsigned int)a; i <= (unsigned int)b; i++)
    {
        updateImage(i, channel, blend, action);
    }

    unlock();

    repaintImageInLayerProcessor(true);

}

void deLayerProcessor::updateImagesSmart(deChannelManager& channelManager, int view, wxProgressDialog* progressDialog, deMemoryInfoFrame* memoryInfoFrame)
{
    lock();

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

    unlock();
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
    log("markUpdateSingleChannel");
    updateImages(index, channel, false, true);
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

    updateImages(index, -1, false, true);
}

void deLayerProcessor::markUpdateBlendAllChannels(int index)
{
    updateImages(index,  -1, true, false);
}

void deLayerProcessor::onChangeView(int a)
{
    updateImages(a + 1, -1, false, true);
}   

void deLayerProcessor::lock()
{
    log("layer processor lock");
    updateImagesMutex.Lock();
}

void deLayerProcessor::unlock()
{
    log("layer processor unlock");
    updateImagesMutex.Unlock();
}

void deLayerProcessor::tickWork()
{
    lock();

    bool ok = true;

    if (firstLayerToUpdate > getLastLayerToUpdate())
    {
        ok = false;
    }

    if (!stack)
    {
        ok = false;
    }

    if (ok)
    {
        updateImage(firstLayerToUpdate, channelUpdate, blendUpdate, actionUpdate);
    }

    checkUpdateImagesRequest();

    unlock();

    if (ok)
    {
        repaintImageInLayerProcessor(true);
    }        


}

void deLayerProcessor::onChangeViewMode()
{
    repaintImageInLayerProcessor(true);
}    

void deLayerProcessor::onGUIUpdate()
{
    repaintImageInLayerProcessor(true);
}    

void deLayerProcessor::removeTopLayer()
{
    int index = stack->getSize() - 1;
    log("requested remove top layer " + str(index));
    if (index > 0)
    {
        lock();
        stack->removeTopLayer();
        int view = viewManager->getView();
        if (view >= stack->getSize())
        {
            viewManager->setView( stack->getSize() - 1 );
        }
        repaintImageInLayerProcessor(true);
        unlock();
    }
}    

void deLayerProcessor::addLayer(deLayer* layer)
{
    lock();

    log("add layer " + str(layer->getIndex()) + " " +  layer->getName());

    if (stack)
    {
        stack->addLayer(layer);

        int index = layer->getIndex();
        markUpdateAllChannels(index);
    }

    unlock();
}    

void deLayerProcessor::setLogger(deLogger* _logger)
{
    logger = _logger;
}

void deLayerProcessor::log(const std::string& message)
{
    if (logger)
    {
        logger->log(message);
    }
}

void deLayerProcessor::checkUpdateImagesRequest()
{
    if (firstLayerToUpdate > getLastLayerToUpdate())
    {
        return;
    }

    if (!stack)
    {
        return;
    }

    workerSemaphore.Post();
}

int deLayerProcessor::getLastLayerToUpdate()
{
    if (!viewManager)
    {
        return 0;
    }
    else
    {
        int n = viewManager->getView();
        return n;
    }        
}

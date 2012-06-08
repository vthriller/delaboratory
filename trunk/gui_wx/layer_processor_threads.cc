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

#include "layer_processor_threads.h"
#include "layer_processor.h"
#include "main_window.h"
#include "main_frame.h"
#include "layer_frame_manager.h"
#include "layer_stack.h"
#include "view_manager.h"
#include <string>
#include "base_layer.h"
#include "channel_manager.h"
#include "str.h"
#include <wx/progdlg.h>
#include <iostream>
#include "logger.h"
#include "renderer.h"
#include "image_io.h"

class deLayerProcessorWorkerThread:public wxThread
{
    private:
        void performTasks()
        {
            while (true)
            {
                if (processor.isClosing())
                {
                    return;
                }

                logInfo("worker thread wait...");
                semaphore.wait();
                Sleep(10);

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
            logInfo("worker thread finished");
            return NULL;
        }
        deLayerProcessor& processor;
        deSemaphore& semaphore;

    public:    
        deLayerProcessorWorkerThread(deLayerProcessor& _processor, deSemaphore& _semaphore)
        :processor(_processor),
         semaphore(_semaphore)
        {
            logInfo("worker thread created");
        }
        virtual ~deLayerProcessorWorkerThread()
        {
        }
};

class deRenderWorkerThread:public wxThread
{
    private:
        void performTasks()
        {
            while (true)
            {
                if (processor.isClosing())
                {
                    return;
                }

                logInfo("render thread wait...");
                semaphore.wait();
                Sleep(10);

                if (TestDestroy())
                {
                    return;
                }

                if (processor.prepareImage())
                {
                    processor.sendRepaintEvent();
                }                    

                if (TestDestroy())
                {
                    return;
                }
            }
        }

        virtual void *Entry()
        {
            performTasks();
            logInfo("render thread finished");
            return NULL;
        }

        deLayerProcessor& processor;
        deSemaphore& semaphore;

    public:    
        deRenderWorkerThread(deLayerProcessor& _processor, deSemaphore& _semaphore)
        :processor(_processor),
         semaphore(_semaphore)
        {
            logInfo("render thread created");
        }
        virtual ~deRenderWorkerThread()
        {
        }
};

class deHistogramWorkerThread:public wxThread
{
    private:
        void performTasks()
        {
            while (true)
            {
                if (processor.isClosing())
                {
                    return;
                }

                logInfo("histogram thread wait...");
                semaphore.wait();
                Sleep(10);

                if (TestDestroy())
                {
                    return;
                }

                processor.onGenerateHistogram();
                processor.sendHistogramEvent();

                if (TestDestroy())
                {
                    return;
                }
            }
        }

        virtual void *Entry()
        {
            performTasks();
            logInfo("histogram thread finished");
            return NULL;
        }

        deLayerProcessor& processor;
        deSemaphore& semaphore;

    public:    
        deHistogramWorkerThread(deLayerProcessor& _processor, deSemaphore& _semaphore)
        :processor(_processor),
         semaphore(_semaphore)
        {
            logInfo("histogram thread created");
        }
        virtual ~deHistogramWorkerThread()
        {
        }
};



void deLayerProcessorThreads::stopWorkerThread()
{
    workerSemaphore.post();
    workerThread->Delete();

    renderWorkerSemaphore.post();
    renderWorkerThread->Delete();

    histogramWorkerSemaphore.post();
    histogramWorkerThread->Delete();

}

void deLayerProcessorThreads::startWorkerThread()
{
    workerThread = new deLayerProcessorWorkerThread(layerProcessor, workerSemaphore);

    if ( workerThread->Create() != wxTHREAD_NO_ERROR )
    {
    }

    if ( workerThread->Run() != wxTHREAD_NO_ERROR )
    {
    }

    renderWorkerThread = new deRenderWorkerThread(layerProcessor, renderWorkerSemaphore);

    if ( renderWorkerThread->Create() != wxTHREAD_NO_ERROR )
    {
    }

    if ( renderWorkerThread->Run() != wxTHREAD_NO_ERROR )
    {
    }

    histogramWorkerThread = new deHistogramWorkerThread(layerProcessor, histogramWorkerSemaphore);

    if ( histogramWorkerThread->Create() != wxTHREAD_NO_ERROR )
    {
    }

    if ( histogramWorkerThread->Run() != wxTHREAD_NO_ERROR )
    {
    }
}


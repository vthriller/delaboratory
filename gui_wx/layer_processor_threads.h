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

#ifndef _DE_LAYER_PROCESSOR_THREADS_H
#define _DE_LAYER_PROCESSOR_THREADS_H

class deMainWindow;
class deLayerStack;
class deViewManager;
class deChannelManager;
class wxProgressDialog;
class deLayer;
class deLogger;
class deLayerFrameManager;
class wxThread;
#include <map>
#include "size.h"
#include "renderer.h"
#include "semaphore.h"
#include "base_layer.h"


class deLayerProcessorThreads
{
    private:
        deLayerProcessor& layerProcessor;
        wxThread* workerThread;
        wxThread* renderWorkerThread;
        wxThread* histogramWorkerThread;
        deSemaphore workerSemaphore;
        deSemaphore renderWorkerSemaphore;
        deSemaphore histogramWorkerSemaphore;
    public:
        deLayerProcessorThreads(deLayerProcessor& _layerProcessor)
        :layerProcessor(_layerProcessor),
        workerSemaphore(1,1),
        renderWorkerSemaphore(1, 1),
        histogramWorkerSemaphore(1, 1)
        {
            histogramWorkerThread = NULL;
            workerThread = NULL;
            renderWorkerThread = NULL;

        };
        virtual ~deLayerProcessorThreads()
        {
        };
        void startWorkerThread();
        void stopWorkerThread();
        void renderPost()
        {
            renderWorkerSemaphore.post();
        }
        void histogramPost()
        {
            histogramWorkerSemaphore.post();
        }            
        void workerPost()
        {
            workerSemaphore.post();
        }

};


#endif
